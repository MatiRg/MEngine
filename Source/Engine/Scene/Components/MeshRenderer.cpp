#include "MeshRenderer.hpp"
#include "../Entity.hpp"
#include "../Transform.hpp"
#include "../../Core/XML.hpp"
#include "../../Engine/Engine.hpp"
#include "../../Graphics/Renderer3D.hpp"

CMeshRenderer::CMeshRenderer(CEngine* aEngine): 
    IComponent(aEngine),
    Renderable(ERenderableType::Solid)
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
    Renderable.SetMaterial(Mesh ? Mesh->GetMaterial() : nullptr );
    Renderable.SetVertexBuffer(Mesh ? Mesh->GetVertexBuffer() : nullptr);
}