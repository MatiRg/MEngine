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
    // TO DO: Graphics Settings
    // TO DO: Save Old Setting
    Graphics->SetDepthActive(true);
    for (const auto& i : Renderables)
    {
        CMaterial* Material = i->GetMaterial();
        IShader* Shader = Material ? i->GetMaterial()->GetShader() : nullptr;
        IVertexBuffer* Buffer = i->GetVertexBuffer();
        if ( Material && Shader && Buffer )
        {
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
            if (Shader->HasUniform("CameraPosition")) // ViewPos
            {
                Shader->SetVector3("CameraPosition", CameraPosition);
            }
            // Lights: TO DO
            //
            Buffer->Bind();
            Buffer->Draw(EPrimitiveMode::Triangles);
            Buffer->UnBind();
            //
            Material->UnBind();
        }
        else
        {
            LOG(ESeverity::Error) << "Invalid IRenderable3D\n";
        }
    }
    Renderables.clear();
}

void CRenderer3D::AddRenderable(IRenderable3D* aRenderable)
{
    Renderables.push_back(aRenderable);
}