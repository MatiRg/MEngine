#include "BoxCollider2D.hpp"
#include "PhysicsWorld2D.hpp"
#include "../../Physics/PhysicsWorld2D.hpp"
#include "../../Core/XML.hpp"
#include "../../Math/Transform.hpp"
#include "../Entity.hpp"
#include "../World.hpp"

CBoxCollider2D::CBoxCollider2D(CEngine* aEngine): 
    IComponent(aEngine)
{
}

CBoxCollider2D::~CBoxCollider2D()
{
    GetOwner()->GetTransform().RemoveScaleCallback(this);

    World->GetWorld()->DestroyCollider(Shape);
}

void CBoxCollider2D::OnCreate()
{
    World = GetOwner()->GetWorld()->GetComponent<CPhysicsWorld2D>();
    Shape = World->GetWorld()->CreateBoxCollider(Vector2::ONE, Vector2::ZERO);
    Shape->SetUserData(this);

    GetOwner()->GetTransform().AddScaleCallback(this, [&](const Vector3& /*NewScale*/) {
        SetSize( GetSize() );
    });
}

bool CBoxCollider2D::OnLoad(CXMLElement* Root)
{
    SetSize( XML::LoadVector2( Root, "Size", Vector2::ONE) );
    //
    SetFriction( XML::LoadFloat(Root, "Friction", 0.2f) );
    SetRestitution( XML::LoadFloat( Root, "Restitution", 0.0f) );
    SetDensity( XML::LoadFloat( Root, "Density", 1.0f) );
    SetOffset(XML::LoadVector2( Root, "Offset", Vector2::ZERO));
    //
    SetSensor(XML::LoadBool(Root, "Sensor", false));
    return true;
}

bool CBoxCollider2D::OnSave(CXMLElement* Root)
{
    XML::SaveVector2( Root, "Size", GetSize() );
    //
    XML::SaveFloat( Root, "Friction", GetFriction() );
    XML::SaveFloat( Root, "Restitution", GetRestitution() );
    XML::SaveFloat( Root, "Density", GetDensity());
    XML::SaveVector2( Root, "Offset", GetOffset() );
    XML::SaveBool( Root, "Sensor", GetSensor() );
    return true;
}

float CBoxCollider2D::GetFriction() const
{
    return Shape->GetFriction();
}

float CBoxCollider2D::GetRestitution() const
{
    return Shape->GetRestitution();
}

float CBoxCollider2D::GetDensity() const
{
    return Shape->GetDensity();
}

Vector2 CBoxCollider2D::GetSize() const
{
    return Shape->GetSize();
}

Vector2 CBoxCollider2D::GetOffset() const
{
    return Shape->GetOffset();
}

bool CBoxCollider2D::GetSensor() const
{
    return Shape->GetSensor();
}

void CBoxCollider2D::SetSize(const Vector2& aSize) 
{ 
    Shape->SetSize(aSize);
}

void CBoxCollider2D::SetFriction(const float Value)
{
    Shape->SetFriction(Value);
}

void CBoxCollider2D::SetRestitution(const float Value)
{
    Shape->SetRestitution(Value);
}

void CBoxCollider2D::SetDensity(const float Value)
{
    Shape->SetDensity(Value);
}

void CBoxCollider2D::SetOffset(const Vector2& Value)
{
    Shape->SetOffset(Value);
}

void CBoxCollider2D::SetSensor(const bool Value)
{
    Shape->SetSensor(Value);
}
