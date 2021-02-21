#include "Camera.hpp"
#include "../Entity.hpp"
#include "../../Core/XML.hpp"
#include "../../Engine/Engine.hpp"
#include "../../Graphics/Renderer3D.hpp"
#include "../../Graphics/Window.hpp"

CCamera::CCamera(CEngine* aEngine):
    IComponent(aEngine) 
{
}

bool CCamera::OnLoad(CXMLElement* Root)
{
    SetFOV( XML::LoadFloat(Root, "Fov", 75.0f) );
    SetAspect( XML::LoadFloat(Root, "Aspect", GetEngine()->GetWindow()->GetAspectRatio() ) );
    SetFarClipPlane( XML::LoadFloat(Root, "Far", 1000.0f) );
    SetNearClipPlane( XML::LoadFloat(Root, "Near", 0.03f) );
    ProjectionDirty = XML::LoadBool(Root, "ProjectionDirty", true);
    ProjectionMatrix = XML::LoadMatrix4(Root, "ProjectionMatrix", MATRIX4_IDENTITY);
    ViewDirty = XML::LoadBool(Root, "ViewDirty", true);
    ViewMatrix = XML::LoadMatrix4(Root, "ViewMatrix", MATRIX4_IDENTITY);
    return true;
}

bool CCamera::OnSave(CXMLElement* Root)
{
    XML::SaveFloat(Root, "Fov", Fov);
    XML::SaveFloat(Root, "Aspect", Aspect);
    XML::SaveFloat(Root, "Far", Far);
    XML::SaveFloat(Root, "Near", Near);
    XML::SaveBool(Root, "ProjectionDirty", ProjectionDirty);
    XML::SaveMatrix4(Root, "ProjectionMatrix", ProjectionMatrix);
    XML::SaveBool(Root, "ViewDirty", ViewDirty);
    XML::SaveMatrix4(Root, "ViewMatrix", ViewMatrix);
    return true;
}

void CCamera::OnCreate()
{
    GetOwner()->GetTransform().AddChangedCallback(this, [&](CTransform*) {
        ViewDirty = true;
    });
    //
    SetAspect( GetEngine()->GetWindow()->GetAspectRatio() );
}

void CCamera::OnDestroy()
{
    GetOwner()->GetTransform().RemoveChangedCallback(this);
}

void CCamera::OnLateUpdate(const float)
{
    Engine->GetRenderer3D()->SetViewMatrix( GetView() );
    Engine->GetRenderer3D()->SetProjectionMatrix( GetProjection() );
    Engine->GetRenderer3D()->SetCameraPosition( GetOwner()->GetTransform().GetWorldPosition() );
}

void CCamera::SetFOV(const float Value)
{
    Fov = Value;
    ProjectionDirty = true;
}

void CCamera::SetAspect(const float Value)
{
    Aspect = Value;
    ProjectionDirty = true;
}

void CCamera::SetFarClipPlane(const float Value)
{
    Far = Value;
    ProjectionDirty = true;
}

void CCamera::SetNearClipPlane(const float Value)
{
    Near = Value;
    ProjectionDirty = true;
}

const Matrix4& CCamera::GetView() const
{
    if (ViewDirty)
    {
        const auto& Transform = GetOwner()->GetTransform();
        ViewMatrix = Math::Inverse(Math::Translation(Transform.GetWorldPosition()) * Math::ToMatrix4(Transform.GetWorldRotation()));
        ViewDirty = false;
    }
    return ViewMatrix;
}

const Matrix4& CCamera::GetProjection() const
{
    if (ProjectionDirty)
    {
        ProjectionMatrix = Math::Perspective(Fov, Aspect, Near, Far);
        ProjectionDirty = false;
    }
    return ProjectionMatrix;
}