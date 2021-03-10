#include "MeshRenderer.hpp"
#include "../Entity.hpp"
#include "../../Math/Transform.hpp"
#include "../../Core/XML.hpp"
#include "../../Engine/Engine.hpp"
#include "../../Graphics/Renderer3D.hpp"

CMeshRenderer::CMeshRenderer(CEngine* aEngine): 
    IComponent(aEngine),
    Renderable(ERenderableType::Object)
{
}

bool CMeshRenderer::OnLoad(CXMLElement* Root)
{
    return true;
}

bool CMeshRenderer::OnSave(CXMLElement* Root)
{
    return true;
}

void CMeshRenderer::OnCreate()
{
}

void CMeshRenderer::OnRender()
{
    Renderable.SetMatrix( GetOwner()->GetTransform().GetWorldMatrix() );
    GetEngine()->GetRenderer3D()->AddRenderable(&Renderable);
}

void CMeshRenderer::SetMesh(CMesh* aMesh)
{ 
    Mesh = aMesh; 
    if (Material)
    {
        Renderable.SetMaterial(Material);
    }
    else
    {
        Renderable.SetMaterial(Mesh ? Mesh->GetMaterial() : nullptr);
    }
    Renderable.SetVertexBuffer(Mesh ? Mesh->GetVertexBuffer() : nullptr);
}

void CMeshRenderer::SetMaterial(CMaterial* aMaterial)
{
    Material = aMaterial; 
    if (Material)
    {
        Renderable.SetMaterial(Material);
    }
    else
    {
        Renderable.SetMaterial(Mesh ? Mesh->GetMaterial() : nullptr);
    }
}