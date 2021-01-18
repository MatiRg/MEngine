#include "Renderer3D.hpp"
#include "Graphics.hpp"
#include "../Core/Log.hpp"

CRenderer3D::CRenderer3D(IGraphics* aGraphics):
    IEngineModule( "Renderer3D" ),
    Graphics(aGraphics)
{
}

CRenderer3D::~CRenderer3D()
{
}

bool CRenderer3D::Init(const SEngineParams&)
{
    LOG( ESeverity::Info ) << "Renderer3D - Init\n";
    return true;
}

void CRenderer3D::Exit()
{
    LOG( ESeverity::Info ) << "Renderer3D - Exit\n";
}

void CRenderer3D::Render()
{
    bool SavedDepth = Graphics->IsDepthActive();
    EDepthMode SavedDepthMode = Graphics->GetDepthFunction();
    bool SavedCull = Graphics->IsCullActive();
    ECullMode SavedCullMode = Graphics->GetCullMode();
    EFrontFace SavedFrontFace = Graphics->GetFrontFace();
    EPolygonMode SavedPolygonMode = Graphics->GetPolygonMode();
    Color SavedClearColor = Graphics->GetClearColor();
    bool SavedBlendActive = Graphics->IsBlendActive();
    EBlendMode SavedBlendMode = Graphics->GetBlendMode();
    //
    Graphics->SetDepthActive(true);
    Graphics->SetDepthFunction(EDepthMode::Less);
    Graphics->SetCullActive(true);
    Graphics->SetCullMode(ECullMode::Back);
    Graphics->SetFrontFace(EFrontFace::CCW);
    Graphics->SetPolygonMode(EPolygonMode::Fill);
    Graphics->SetClearColor( Color(0.0f, 1.0f) );
    Graphics->SetBlendActive(true);
    Graphics->SetBlendMode(EBlendMode::None);
    //
    for (const auto& i : Renderables)
    {
        CMaterial* Material = i->GetMaterial();
        IShader* Shader = i->GetMaterial()->GetShader();
        IVertexBuffer* Buffer = i->GetVertexBuffer();
        Material->Bind();
        // Constants
        if (Shader->HasUniform("Model"))
        {
            Shader->SetMatrix4("Model", i->GetMatrix());
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
        //
        Buffer->Bind();
        Buffer->Draw(EPrimitiveMode::Triangles);
        Buffer->UnBind();
        //
        Material->UnBind();
    }
    Renderables.clear();
    Lights.clear();
    //
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

void CRenderer3D::AddRenderable(CRenderable3D* aRenderable)
{
    if (aRenderable->GetRenderableType() != ERenderableType::Light)
    {
        if (aRenderable->HasMaterial() && aRenderable->HasVertexBuffer() && aRenderable->GetMaterial()->HasShader())
        {
            Renderables.push_back(aRenderable);
        }
        else
        {
            LOG(ESeverity::Warning) << "Adding Invalid CRenderable3D\n";
        }
    }
    else
    {
        Lights.push_back( static_cast<CLight*>(aRenderable) );
    }
}