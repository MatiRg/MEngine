#include "MeshRenderer.hpp"
#include "../Entity.hpp"
#include "../Transform.hpp"
#include "../../Core/XML.hpp"
#include "../../Engine/Engine.hpp"
#include "../../Graphics/Renderer3D.hpp"

CMeshRenderer::CMeshRenderer(CEngine* aEngine): 
    IComponent(aEngine),
    IRenderable3D(ERenderableType::Solid)
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
    //Material = 
}

void CMeshRenderer::OnLateUpdate(const float)
{
    SetMatrix(GetOwner()->GetTransform().GetWorldMatrix());
}

void CMeshRenderer::OnRender()
{
    GetEngine()->GetRenderer3D()->AddRenderable(this);
}

IVertexBuffer* CMeshRenderer::GetVertexBuffer() const
{
    return Mesh->GetBuffer();
}