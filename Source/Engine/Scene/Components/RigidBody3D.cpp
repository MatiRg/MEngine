#include "RigidBody3D.hpp"
#include "PhysicsWorld3D.hpp"
#include "BoxCollider3D.hpp"
#include "../../Physics/PhysicsWorld3D.hpp"
#include "../../Engine/Engine.hpp"
#include "../../Core/Log.hpp"
#include "../../Core/XML.hpp"
#include "../../Math/Functions.hpp"
#include "../Entity.hpp"
#include "../World.hpp"

CRigidBody3D::CRigidBody3D(CEngine* aEngine):
    IComponent(aEngine)
{
}

CRigidBody3D::~CRigidBody3D()
{
    GetOwner()->GetTransform().RemovePositionCallback(this);
    GetOwner()->GetTransform().RemoveRotationCallback(this);
    PhysicsWorld3D->GetWorld()->DestroyBody(Body);
}

void CRigidBody3D::OnCreate()
{
    CWorld* World = GetOwner()->GetWorld();
    //
    PhysicsWorld3D = World->GetComponent<CPhysicsWorld3D>();
    if (!PhysicsWorld3D)
    {
        PhysicsWorld3D = World->CreateComponent<CPhysicsWorld3D>();
    }
    CBoxCollider3D* Collider = GetOwner()->GetComponent<CBoxCollider3D>();
    if (!Collider)
    {
        Collider = GetOwner()->CreateComponent<CBoxCollider3D>();
    }
    Body = PhysicsWorld3D->GetWorld()->CreateBody( Collider->GetShape() );

    const CTransform& Ts = Owner->GetTransform();
    Body->SetPosition( Ts.GetWorldPosition() );
    Body->SetRotation( Ts.GetWorldRotation() );

    Body->SetUserData(this);

    GetOwner()->GetTransform().AddPositionCallback(this, [&](const Vector3& /*NewPosition*/) {
        Body->SetPosition(Ts.GetWorldPosition());
    });
    GetOwner()->GetTransform().AddRotationCallback(this, [&](const Quaternion& /*NewAngle*/) {
        Body->SetRotation(Ts.GetWorldRotation());
    });
}

bool CRigidBody3D::OnLoad(CXMLElement* Root)
{
 //   Body->SetBodyType(XML::LoadEnum( Root, "BodyType", ERigidBodyType2D::Dynamic) );
 //   //
	//Body->SetAngle( XML::LoadFloat( Root, "Angle", 0.0f ) );
	//Body->SetPosition( XML::LoadVector2( Root, "Position", Vector2::ZERO ) );
 //   //
 //   Body->SetFixedRotation(XML::LoadBool( Root, "FixedRotation", false));

 //   Body->SetLinearVelocity(XML::LoadVector2( Root, "LinearVelocity", Vector2::ZERO ));

 //   Body->SetLinearDamping(XML::LoadFloat( Root, "LinearDamping", 0.0f ));

 //   Body->SetAngularVelocity(XML::LoadFloat( Root, "AngularVelocity", 0.0f));

	//Body->SetAngularDamping(XML::LoadFloat( Root, "AngularDamping", 0.0f));
 //   //
 //   Body->SetEnabled( XML::LoadBool( Root, "IsEnabled", true) );
    return true;
}

bool CRigidBody3D::OnSave(CXMLElement* Root)
{
    //XML::SaveEnum( Root, "BodyType", Body->GetBodyType());
    ////
    //XML::SaveFloat( Root, "Angle", Body->GetAngle());
    //XML::SaveVector2( Root, "Position", Body->GetPosition());
    ////
    //XML::SaveBool( Root, "FixedRotation", Body->HasFixedRotation());

    //XML::SaveVector2( Root, "LinearVelocity", Body->GetLinearVelocity());

    //XML::SaveFloat( Root, "LinearDamping", Body->GetLinearDamping());

    //XML::SaveFloat( Root, "AngularVelocity", Body->GetAngularVelocity());

    //XML::SaveFloat( Root, "AngularDamping", Body->GetAngularDamping() );
    ////
    //XML::SaveBool( Root, "IsEnabled", Body->IsEnabled());
    return true;
}

void CRigidBody3D::OnEnabled()
{
    Body->SetEnabled(true);
}

void CRigidBody3D::OnDisabled()
{
    Body->SetEnabled(false);
}

void CRigidBody3D::OnUpdate(const float)
{
    Owner->GetTransform().SetWorldPosition(Body->GetPosition(), true);
    Owner->GetTransform().SetWorldRotation(Body->GetRotation(), true);
}

void CRigidBody3D::SetCollisionLayer(int Layer)
{
    Body->SetCollisionLayer(Layer);
}

int CRigidBody3D::GetCollisionLayer() const
{
    return Body->GetCollisionLayer();
}

void CRigidBody3D::SetCollisionMask(int Mask)
{
    Body->SetCollisionMask(Mask);
}

int CRigidBody3D::GetCollisionMask() const
{
    return Body->GetCollisionMask();
}

void CRigidBody3D::SetGravity(const Vector3& x)
{
    Body->SetGravity(x);
}

Vector3 CRigidBody3D::GetGravity() const
{
    return Body->GetGravity();
}

void CRigidBody3D::SetMass(float x)
{
    Body->SetMass(x);
}

float CRigidBody3D::GetMass() const
{
    return Body->GetMass();
}

void CRigidBody3D::SetLinearVelocity(const Vector3& x)
{
    Body->SetLinearVelocity(x);
}

Vector3 CRigidBody3D::GetLinearVelocity() const
{
    return Body->GetLinearVelocity();
}

void CRigidBody3D::SetAngularVelocity(const Vector3& x)
{
    Body->SetAngularVelocity(x);
}

Vector3 CRigidBody3D::GetAngularVelocity() const
{
    return Body->GetAngularVelocity();
}

void CRigidBody3D::AddForce(const Vector3& x)
{
    Body->AddForce(x);
}

void CRigidBody3D::AddForceAtPosition(const Vector3& x, const Vector3& p)
{
    Body->AddForceAtPosition(x, p);
}

void CRigidBody3D::AddTorque(const Vector3& x)
{
    Body->AddTorque(x);
}

void CRigidBody3D::SetFriction(const float x)
{
    Body->SetFriction(x);
}

float CRigidBody3D::GetFriction() const
{
    return Body->GetFriction();
}

void CRigidBody3D::SetRollingFriction(const float x)
{
    Body->SetRollingFriction(x);
}

float CRigidBody3D::GetRollingFriction() const
{
    return Body->GetRollingFriction();
}

void CRigidBody3D::SetRestitution(const float x)
{
    Body->SetRestitution(x);
}

float CRigidBody3D::GetRestitution() const
{
    return Body->GetRestitution();
}

void CRigidBody3D::SetLinearFactor(const Vector3& x)
{
    Body->SetLinearFactor(x);
}

Vector3 CRigidBody3D::GetLinearFactor() const
{
    return Body->GetLinearFactor();
}

void CRigidBody3D::SetAngularFactor(const Vector3& x)
{
    Body->SetAngularFactor(x);
}

Vector3 CRigidBody3D::GetAngularFactor() const
{
    return Body->GetAngularFactor();
}

void CRigidBody3D::SetLinearDamping(const float x)
{
    Body->SetLinearDamping(x);
}

float CRigidBody3D::GetLinearDamping() const
{
    return Body->GetLinearDamping();
}

void CRigidBody3D::SetAngularDamping(const float x)
{
    Body->SetAngularDamping(x);
}

float CRigidBody3D::GetAngularDamping() const
{
    return Body->GetAngularDamping();
}

void CRigidBody3D::SetSleepThreshold(const Vector2& t)
{
    Body->SetSleepThreshold(t);
}

Vector2 CRigidBody3D::GetSleepThreshold() const
{
    return Body->GetSleepThreshold();
}

void CRigidBody3D::SetBodyType(const ERigidBodyType3D x)
{
    Body->SetBodyType(x);
}

ERigidBodyType3D CRigidBody3D::GetBodyType() const
{
    return Body->GetBodyType();
}