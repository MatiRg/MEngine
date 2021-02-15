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
    return true;
}

bool CCamera::OnSave(CXMLElement* Root)
{
    XML::SaveFloat(Root, "Fov", Fov);
    XML::SaveFloat(Root, "Aspect", Aspect);
    XML::SaveFloat(Root, "Far", Far);
    XML::SaveFloat(Root, "Near", Near);
    return true;
}

void CCamera::OnCreate()
{
    SetAspect( GetEngine()->GetWindow()->GetAspectRatio() );
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
    //ProjectionUpdate = true;
}

void CCamera::SetAspect(const float Value)
{
    Aspect = Value;
    //ProjectionUpdate = true;
}

void CCamera::SetFarClipPlane(const float Value)
{
    Far = Value;
    //ProjectionUpdate = true;
}

void CCamera::SetNearClipPlane(const float Value)
{
    Near = Value;
    //ProjectionUpdate = true;
}

Matrix4 CCamera::GetView() const
{
    const auto& Transform = GetOwner()->GetTransform();
    // -1 For Forward+
    return Math::Transform(Transform.GetWorldPosition(), Transform.GetWorldRotation(), Vector3(1.0f, 1.0f, 1.0f)).Inverse();
    /*if( ViewUpdate )
    {
        const auto& Transform = GetOwner()->GetTransform();
        ViewMatrix = Math::Transform(Transform.GetWorldPosition(), Transform.GetWorldRotation(), Vector3(1.0f) ).Inverse();
        //ViewUpdate = false;
    }
    return ViewMatrix;*/
}

Matrix4 CCamera::GetProjection() const
{
    return Math::Perspective(Fov, Aspect, Near, Far);;
    /*if (ProjectionUpdate)
    {
        ProjectionMatrix = Math::Perspective(Fov, Aspect, Near, Far);
        ProjectionUpdate = false;
    }
    return ProjectionMatrix;*/
}