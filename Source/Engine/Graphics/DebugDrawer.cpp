#include "DebugDrawer.hpp"
#include "Renderer3D.hpp"
#include "Graphics.hpp"
#include "../Resources/Resources.hpp"
#include "../Core/Log.hpp"

CDebugDrawer::CDebugDrawer(CRenderer3D* aRenderer3D, IGraphics* aGraphics, CResources* aResources):
    IEngineModule( "DebugDrawer" ),
    Renderer3D(aRenderer3D),
    Graphics(aGraphics),
    Resources(aResources),
    LineRenderable(ERenderableType::Object)
{
}

CDebugDrawer::~CDebugDrawer()
{
}

void CDebugDrawer::AddLine(const Vector3& Start, const Vector3& End, const Color& DrawColor, const float Time)
{
    Lines.emplace_back( Start, End, DrawColor, Time );
}

bool CDebugDrawer::Init(const SEngineParams&)
{
    Buffer = Graphics->CreateVertexBuffer({ {EVertexElement::Position}, {EVertexElement::Color} }, true);
    if (!Buffer)
    {
        LOG(ESeverity::Fatal) << "Debug Drawer: Invalid Vertex Buffer\n";
        return false;
    }

    Shader = Resources->CreateResource<IShader>("Debug.shader");
    if (!Shader)
    {
        LOG(ESeverity::Fatal) << "Debug Drawer: Invalid 'Debug' Shader\n";
        return false;
    }

    Material = std::make_unique<CMaterial>();
    Material->SetShader(Shader);

    LineRenderable.SetMaterial(Material.get());
    LineRenderable.SetVertexBuffer(Buffer.get());
    LineRenderable.SetPrimitiveType(EPrimitiveType::Lines);
    LineRenderable.SetMatrix(Matrix4::IDENTITY);

    LOG( ESeverity::Info ) << "DebugDrawer - Init\n";
    return true;
}

void CDebugDrawer::OnBeginFrame()
{
    Lines.erase(std::remove_if(Lines.begin(), Lines.end(), [](const SLine& i) {
        return i.Time <= 0.0f;
    }), Lines.end());
}

void CDebugDrawer::OnUpdate(const float DT)
{
    for (auto& i: Lines)
    {
        i.Time -= DT;
    }
}

void CDebugDrawer::OnRender()
{
    std::vector<Vector3> Positions;
    std::vector<Color> Colors;
    for (const auto& i : Lines)
    {
        Positions.push_back(i.Start);
        Positions.push_back(i.End);
        Colors.push_back(i.DrawColor);
        Colors.push_back(i.DrawColor);
    }
    if (!Positions.empty())
    {
        Buffer->SetData(EVertexElement::Position, Positions);
        Buffer->SetData(EVertexElement::Color, Colors);
        Renderer3D->AddRenderable(&LineRenderable);
    }
}

void CDebugDrawer::Exit()
{
    Material.reset();
    Buffer.reset();
    LOG( ESeverity::Info ) << "DebugDrawer - Exit\n";
}
