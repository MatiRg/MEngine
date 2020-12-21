#include "RigidBody2D.hpp"
#include "PhysicsWorld2D.hpp"
#include "BoxCollider2D.hpp"
#include "../../Physics/PhysicsWorld2D.hpp"
#include "../../Engine/Engine.hpp"
#include "../../Core/Log.hpp"
#include "../../Core/XML.hpp"
#include "../../Math/Functions.hpp"
#include "../Entity.hpp"
#include "../World.hpp"

CRigidBody2D::CRigidBody2D(CEngine* aEngine):
    IComponent(aEngine)
{
}

CRigidBody2D::~CRigidBody2D()
{
    GetOwner()->GetTransform().RemovePositionCallback(this);
    GetOwner()->GetTransform().RemoveRotationCallback(this);
    World->GetWorld()->DestroyRigidBody(Body);
}

void CRigidBody2D::OnCreate()
{
    CBoxCollider2D* Collider = GetOwner()->GetComponent<CBoxCollider2D>();
    if (!Collider)
    {
        Collider = GetOwner()->CreateComponent<CBoxCollider2D>();
    }
    World = GetOwner()->GetWorld()->GetComponent<CPhysicsWorld2D>();
    Body = World->GetWorld()->CreateRigidBody(Collider->GetShape());

    const CTransform& Ts = Owner->GetTransform();

    Body->SetPosition( Ts.GetWorldPosition2D() );
    Body->SetAngle( Ts.GetWorldRotation2D() );

    Body->SetUserData(this);

    GetOwner()->GetTransform().AddPositionCallback(this, [&](const Vector3& /*NewPosition*/) {
        Vector2 Tmp = GetOwner()->GetTransform().GetWorldPosition2D();
        Body->SetPosition(Tmp);
        });
    GetOwner()->GetTransform().AddRotationCallback(this, [&](const Quaternion& /*NewAngle*/) {
        Body->SetAngle(GetOwner()->GetParent()->GetTransform().GetWorldRotation2D());
        });

}

bool CRigidBody2D::OnLoad(CXMLElement* Root)
{
    Body->SetBodyType(XML::LoadEnum( Root, "BodyType", ERigidBodyType2D::Dynamic) );
    //
	Body->SetAngle( XML::LoadFloat( Root, "Angle", 0.0f ) );
	Body->SetPosition( XML::LoadVector2( Root, "Position", Vector2::ZERO ) );
    //
    Body->SetFixedRotation(XML::LoadBool( Root, "FixedRotation", false));

    Body->SetLinearVelocity(XML::LoadVector2( Root, "LinearVelocity", Vector2::ZERO ));

    Body->SetLinearDamping(XML::LoadFloat( Root, "LinearDamping", 0.0f ));

    Body->SetAngularVelocity(XML::LoadFloat( Root, "AngularVelocity", 0.0f));

	Body->SetAngularDamping(XML::LoadFloat( Root, "AngularDamping", 0.0f));
    //
    Body->SetEnabled( XML::LoadBool( Root, "IsEnabled", true) );
    return true;
}

bool CRigidBody2D::OnSave(CXMLElement* Root)
{
    XML::SaveEnum( Root, "BodyType", Body->GetBodyType());
    //
    XML::SaveFloat( Root, "Angle", Body->GetAngle());
    XML::SaveVector2( Root, "Position", Body->GetPosition());
    //
    XML::SaveBool( Root, "FixedRotation", Body->HasFixedRotation());

    XML::SaveVector2( Root, "LinearVelocity", Body->GetLinearVelocity());

    XML::SaveFloat( Root, "LinearDamping", Body->GetLinearDamping());

    XML::SaveFloat( Root, "AngularVelocity", Body->GetAngularVelocity());

    XML::SaveFloat( Root, "AngularDamping", Body->GetAngularDamping() );
    //
    XML::SaveBool( Root, "IsEnabled", Body->IsEnabled());
    return true;
}

void CRigidBody2D::OnEnabled()
{
    Body->SetEnabled(true);
}

void CRigidBody2D::OnDisabled()
{
    Body->SetEnabled(false);
}

void CRigidBody2D::OnBeginFrame()
{
    float Angle = Body->GetAngle();
    Vector2 Position = Body->GetPosition();

    Owner->GetTransform().SetWorldPosition2D(Position, true);
    Owner->GetTransform().SetWorldRotation2D(Angle, true);
}

Vector2 CRigidBody2D::GetLinearVelocity() const
{
    return Body->GetLinearVelocity();
}

float CRigidBody2D::GetAngularVelocity() const
{
    return Body->GetAngularVelocity();
}

void CRigidBody2D::SetBodyType(const ERigidBodyType2D aType)
{
    Body->SetBodyType( aType );
}

void CRigidBody2D::SetFixedRotation(const bool Value)
{
    Body->SetFixedRotation(Value);
}

void CRigidBody2D::AddForce(const Vector2& Force)
{
    Body->AddForce( Force );
}

void CRigidBody2D::AddTorque(const float Torque)
{
    Body->AddTorque( Torque );
}

void CRigidBody2D::AddLinearImpulse(const Vector2& Impulse)
{
    Body->AddLinearImpulse( Impulse );
}

void CRigidBody2D::AddAngularImpulse(const float Impulse)
{
    Body->AddAngularImpulse( Impulse );
}

float CRigidBody2D::GetMass() const
{
    return Body->GetMass();
}

void CRigidBody2D::SetLinearVelocity(const Vector2& Velocity)
{
    Body->SetLinearVelocity( Velocity );
}

void CRigidBody2D::SetAngularVelocity(const float Velocity)
{
    Body->SetAngularVelocity( Velocity );
}

void CRigidBody2D::SetLinearDamping(const float Damping)
{
    Body->SetLinearDamping(Damping);
}

void CRigidBody2D::SetAngularDamping(const float Damping)
{
    Body->SetAngularDamping(Damping);
}

float CRigidBody2D::GetLinearDamping() const
{
    return Body->GetLinearDamping();
}

float CRigidBody2D::GetAngularDamping() const
{
    return Body->GetAngularDamping();
}

ERigidBodyType2D CRigidBody2D::GetBodyType() const
{
    return Body->GetBodyType();
}

bool CRigidBody2D::HasFixedRotation() const
{
    return Body->HasFixedRotation();
}

void CRigidBody2D::SetMass(const float x)
{
    Body->SetMass(x);
}