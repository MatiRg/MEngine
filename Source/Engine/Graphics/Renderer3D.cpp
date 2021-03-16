#include "Renderer3D.hpp"
#include "Graphics.hpp"
#include "../Resources/Resources.hpp"
#include "../Core/Log.hpp"
#include <algorithm>

CRenderer3D::CRenderer3D(IGraphics* aGraphics, CResources* aResources):
    IEngineModule( "Renderer3D" ),
    Graphics(aGraphics),
    Resources(aResources)
{
}

CRenderer3D::~CRenderer3D()
{
}

bool CRenderer3D::Init(const SEngineParams&)
{
    MSAASamples = 4;
    MSAAFrameBuffer = Graphics->CreateMSAAFrameBuffer(MSAASamples);
    if (!MSAAFrameBuffer)
    {
        LOG(ESeverity::Fatal) << "Renderer3D: Invalid MSAA Frame Buffer\n";
        return false;
    }
 
    SolidFrameBuffer = Graphics->CreateFrameBuffer();
    if (!SolidFrameBuffer)
    {
        LOG(ESeverity::Fatal) << "Renderer3D: Invalid Solid Frame Buffer\n";
        return false;
    }

    TransparentFrameBuffer = Graphics->CreateFrameBuffer();
    if (!TransparentFrameBuffer)
    {
        LOG(ESeverity::Fatal) << "Renderer3D: Invalid Transparent Frame Buffer\n";
        return false;
    }

    std::vector<Vector3> ScreenPositions = {
        {-1.0f,  1.0f, 0.0f },
        {-1.0f, -1.0f, 0.0f },
        { 1.0f, -1.0f, 0.0f },
        {-1.0f,  1.0f, 0.0f },
        { 1.0f, -1.0f, 0.0f },
        { 1.0f,  1.0f, 0.0f }
    };
    std::vector<Vector2> ScreenTexCoords = {
        { 0.0f, 1.0f },
        { 0.0f, 0.0f },
        { 1.0f, 0.0f },
        { 0.0f, 1.0f },
        { 1.0f, 0.0f },
        { 1.0f, 1.0f }
    };
    QuadVertexBuffer = Graphics->CreateVertexBuffer({ EVertexElement::Position, EVertexElement::TexCoord0 }, false);
    if (!QuadVertexBuffer)
    {
        LOG(ESeverity::Fatal) << "Renderer3D: Invalid Screen Vertex Buffer\n";
        return false;
    }
    QuadVertexBuffer->SetData(EVertexElement::Position, ScreenPositions);
    QuadVertexBuffer->SetData(EVertexElement::TexCoord0, ScreenTexCoords);

    ScreenShader = Resources->CreateResource<IShader>("Screen.shader");
    if (!ScreenShader)
    {
        LOG(ESeverity::Fatal) << "Renderer3D: Invalid 'Screen.shader' Shader\n";
        return false;
    }

    LOG( ESeverity::Info ) << "Renderer3D - Init\n";
    return true;
}

void CRenderer3D::Exit()
{
    Effects.clear();
    QuadVertexBuffer.reset();
    TransparentFrameBuffer.reset();
    SolidFrameBuffer.reset();
    MSAAFrameBuffer.reset();
    LOG( ESeverity::Info ) << "Renderer3D - Exit\n";
}

void CRenderer3D::OnUpdate(const float DT)
{
    Time += DT;
}

void CRenderer3D::SetProjectionParams(const float Near, const float Far)
{ 
    ProjectionParams = { -1.0f, Near, Far, 1.0f / Far }; 
    float x = 1.0f - ProjectionParams.z / ProjectionParams.y;
    float y = ProjectionParams.z / ProjectionParams.y;
    float z = x / ProjectionParams.z;
    float w = y / ProjectionParams.z;
    ZBufferParams = { x, y, z, w };
}

CPostEffect* CRenderer3D::CreatePostEffect(const std::string& ShaderName, const int Order)
{
    PostEffectPtr Ptr = std::make_unique<CPostEffect>(Graphics, Resources, Order);
    if( !Ptr->Create(ShaderName) )
    {
        LOG(ESeverity::Error) << "Unable to Create Post Effect for: '" << ShaderName << "' Shader\n";
        return nullptr;
    }
    CPostEffect* RawPtr = Ptr.get();
    Effects.push_back( std::move(Ptr) );
    return RawPtr;
}

void CRenderer3D::Render()
{
    // Save State
    bool SavedDepth = Graphics->IsDepthActive();
    EDepthMode SavedDepthMode = Graphics->GetDepthFunction();
    bool SavedCull = Graphics->IsCullActive();
    ECullMode SavedCullMode = Graphics->GetCullMode();
    EFrontFace SavedFrontFace = Graphics->GetFrontFace();
    EPolygonMode SavedPolygonMode = Graphics->GetPolygonMode();
    Color SavedClearColor = Graphics->GetClearColor();
    bool SavedBlendActive = Graphics->IsBlendActive();
    EBlendMode SavedBlendMode = Graphics->GetBlendMode();
    /////////////////////////////////////////////////////////////
    DrawCalls = 0;
    // Set Starting State
    Graphics->SetDepthActive(true);
    Graphics->SetDepthFunction(EDepthMode::Less);
    Graphics->SetCullActive(true);
    Graphics->SetCullMode(ECullMode::Back);
    Graphics->SetFrontFace(EFrontFace::CCW);
    Graphics->SetPolygonMode(Wireframe ? EPolygonMode::Line : EPolygonMode::Fill);
    Graphics->SetClearColor(Color(0.0f, 1.0f));
    // Render Solids
    RenderSolid();
    // Render Transparent
    RenderTransparent();
    /////////////////////////////////////////////////////////////
    // Post Process and Final Render
    RenderPostEffect();
    /////////////////////////////////////////////////////////////
    // Clear Rendered Objects
    TransparentQueue.clear();
    SolidQueue.clear();
    Lights.clear();
    // ReCreate State
    Graphics->SetCullActive(SavedCull);
    Graphics->SetCullMode(SavedCullMode);
    Graphics->SetFrontFace(SavedFrontFace);
    Graphics->SetPolygonMode(SavedPolygonMode);
    Graphics->SetClearColor(SavedClearColor);
    Graphics->SetBlendMode(SavedBlendMode);
    Graphics->SetBlendActive(SavedBlendActive);
    Graphics->SetDepthFunction(SavedDepthMode);
    Graphics->SetDepthActive(SavedDepth);
}

void CRenderer3D::RenderSolid()
{
    SolidFrameBuffer->Bind();
    //
    Graphics->Clear();
    //
    // Solid Pass
    Graphics->SetBlendActive(false);
    Graphics->SetBlendMode(EBlendMode::None);
    //
    RenderRenderablesVector(SolidQueue);
    //
    SolidFrameBuffer->UnBind();
}

void CRenderer3D::RenderTransparent()
{
    SolidFrameBuffer->Blit(TransparentFrameBuffer.get());
    TransparentFrameBuffer->Bind();
    // Transparent Pass
    Graphics->SetDepthActive(true);
    Graphics->SetBlendActive(true);
    Graphics->SetBlendMode(EBlendMode::Alpha);
    //
    std::sort(TransparentQueue.begin(), TransparentQueue.end(), [&](CRenderable3D* rhs, CRenderable3D* lhs) {
        float D1 = rhs->GetPosition().DistanceSquared(CameraPosition);
        float D2 = lhs->GetPosition().DistanceSquared(CameraPosition);
        return D1 > D2;
    });
    RenderRenderablesVector(TransparentQueue);
    //
    TransparentFrameBuffer->UnBind();
}

void CRenderer3D::RenderPostEffect()
{
    Graphics->SetDepthActive(false);
    Graphics->SetBlendActive(false);
    Graphics->SetPolygonMode(EPolygonMode::Fill);
    // Post Process
    std::sort(Effects.begin(), Effects.end(), [](const PostEffectPtr& rhs, const PostEffectPtr& lhs) {
        return rhs->GetOrder() < lhs->GetOrder();
    });
    IFrameBuffer* LastFrameBuffer = TransparentFrameBuffer.get();
    ITexture2D* FinalOutputTexture = LastFrameBuffer->GetColorAttachment();
    for (const auto& i : Effects)
    {
        if (i->IsEnabled())
        {
            i->Bind(LastFrameBuffer);
            QuadVertexBuffer->Bind();
            QuadVertexBuffer->Draw(EPrimitiveType::Triangles, 6u);
            QuadVertexBuffer->UnBind();
            i->UnBind();
            //
            LastFrameBuffer = i->GetFrameBuffer();
            FinalOutputTexture = i->GetColorAttachment();
        }
    }
    //
    // Final Draw
    Graphics->Clear();
    //
    ScreenShader->Bind();
    ScreenShader->SetTexture("ScreenTexture", FinalOutputTexture, 0);
    ScreenShader->SetFloat("GammaCorrection", GammaCorrection);
    ScreenShader->SetFloat("Exposure", Exposure);
    //
    QuadVertexBuffer->Bind();
    QuadVertexBuffer->Draw(EPrimitiveType::Triangles, 6u);
    QuadVertexBuffer->UnBind();
    //
    ScreenShader->UnBind();
}

void CRenderer3D::SetupMaterialShaderParameters(CRenderable3D* Renderable)
{
    IShader* Shader = Renderable->GetMaterial()->GetShader();
    CMaterial* Material = Renderable->GetMaterial();
    // Constants
    if (Shader->HasUniform("Model"))
    {
        Shader->SetMatrix4("Model", Renderable->GetMatrix());
    }
    if (Shader->HasUniform("View"))
    {
        Shader->SetMatrix4("View", ViewMatrix);
    }
    if (Shader->HasUniform("Projection"))
    {
        Shader->SetMatrix4("Projection", ProjectionMatrix);
    }
    if (Shader->HasUniform("ViewProjection"))
    {
        Shader->SetMatrix4("ViewProjection", ProjectionMatrix * ViewMatrix);
    }
    if (Shader->HasUniform("CameraPosition")) // ViewPos
    {
        Shader->SetVector3("CameraPosition", CameraPosition);
    }
    if (Shader->HasUniform("ProjectionParams"))
    {
        Shader->SetVector4("ProjectionParams", ProjectionParams);
    }
    if (Shader->HasUniform("ZBufferParams"))
    {
        Shader->SetVector4("ZBufferParams", ZBufferParams);
    }
    if (Shader->HasUniform("Time"))
    {
        Shader->SetFloat("Time", Time);
    }
    // Global
    if (Shader->HasUniform("AmbientColor"))
    {
        Shader->SetColor("AmbientColor", AmbientColor);
    }
    // Lights
    bool HasLights = false;
    if (Shader->HasUniform("LightCount"))
    {
        HasLights = true;
        Shader->SetInteger("LightCount", static_cast<int>(Lights.size()));
    }
    //
    if (HasLights)
    {
        for (std::size_t i = 0u; i < Lights.size(); ++i)
        {
            std::string PostFix = "[" + std::to_string(static_cast<int>(i)) + "]";
            Shader->SetInteger("LightType" + PostFix, static_cast<int>(Lights[i]->GetLightType()));
            Shader->SetColor("LightColor" + PostFix, Lights[i]->GetColor());
            if (Lights[i]->GetLightType() == ELightType::Direction)
            {
                Shader->SetVector4("LightParam1" + PostFix, { Lights[i]->GetDirection(), 0.0f });
            }
        }
    }
    // Depth
    if (Material->GetPassType() == EPassType::Transparent)
    {
        if (Shader->HasUniform("DepthTexture"))
        {
            Shader->SetTexture("DepthTexture", SolidFrameBuffer->GetDepthAttachment(), Material->GetLastTextureIndex() + 1);
        }
    }
}

void CRenderer3D::RenderRenderablesVector(const Renderable3DVec& Data)
{
    for (const auto& i : Data)
    {
        CMaterial* Material = i->GetMaterial();
        IVertexBuffer* Buffer = i->GetVertexBuffer();
        Material->Bind();
        // Constants
        SetupMaterialShaderParameters(i);
        //
        Buffer->Bind();
        Buffer->Draw(i->GetPrimitiveType());
        Buffer->UnBind();
        //
        Material->UnBind();
        //
        ++DrawCalls;
    }
}

void CRenderer3D::AddRenderable(CRenderable3D* aRenderable)
{
    if (!aRenderable)
    {
        LOG(ESeverity::Warning) << "Adding Invalid CRenderable3D\n";
        return;
    }
    //
    if (aRenderable->GetRenderableType() == ERenderableType::Light)
    {
        Lights.push_back(static_cast<CLight*>(aRenderable));
    }
    else
    {
        if (aRenderable->HasMaterial() && aRenderable->HasVertexBuffer() && aRenderable->GetMaterial()->HasShader())
        {
            EPassType Pass = aRenderable->GetMaterial()->GetPassType();
            if (Pass == EPassType::Solid)
            {
                SolidQueue.push_back(aRenderable);
            }
            else
            {
                TransparentQueue.push_back(aRenderable);
            }
        }
        else
        {
            LOG(ESeverity::Warning) << "Adding Invalid CRenderable3D\n";
        }
    }
}

void CRenderer3D::SetMSAASamples(int Samples)
{
    Samples = Math::Clamp(Samples, 1, 16);
    auto TmpFrameBuffer = Graphics->CreateMSAAFrameBuffer(Samples);
    if (!TmpFrameBuffer)
    {
        LOG(ESeverity::Fatal) << "Renderer3D: Unable to ReCreate MSAA FrameBuffer with " << Samples << "\n";
        return;
    }
    MSAASamples = Samples;
    MSAAFrameBuffer.reset();
    MSAAFrameBuffer = std::move(TmpFrameBuffer);
}