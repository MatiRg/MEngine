#include "RigidBody2D.hpp"
#include "PhysicsWorld2D.hpp"
#include "Physics2D.hpp"
#include "Box2DUtils.hpp"
#include "../Engine/Engine.hpp"
#include "../Core/Log.hpp"
#include "../Core/XML.hpp"
#include "../Math/Functions.hpp"
#include "../Scene/Transform2D.hpp"
#include "../Scene/Entity2D.hpp"

namespace
{
    b2BodyType Tob2BodyType(const ERigidBody2DType Type)
    {
        switch(Type)
        {
        case ERigidBody2DType::Static:
            return b2_staticBody;
        case ERigidBody2DType::Kinematic:
            return b2_kinematicBody;
        case ERigidBody2DType::Dynamic:
        default:
            return b2_dynamicBody;
        }
    }
}

CRigidBody2D::CRigidBody2D(CEngine* aEngine):
    IComponent(aEngine)
{
}

CRigidBody2D::~CRigidBody2D()
{
    GetOwner()->GetTransform().RemovePositionCallback(this);
    GetOwner()->GetTransform().RemoveAngleCallback(this);
    if( Body )
    {
        PhysicsWorld2D->GetWorld().DestroyBody(Body);
    }
}

void CRigidBody2D::OnCreate()
{
    PhysicsWorld2D = Engine->GetPhysics2D()->GetWorld();

    CTransform2D Ts = Owner->GetTransform().GetWorldTransform();

    b2BodyDef Definiton;
    Definiton.position = Tob2Vec2( Ts.GetPosition() );
    Definiton.angle = Math::DegToRad( Ts.GetAngle() );
    Definiton.linearVelocity = {0.0f, 0.0f};
    Definiton.linearDamping = LinearDamping;
    Definiton.angularVelocity = Math::DegToRad(0.0f);
    Definiton.angularDamping = AngularDamping;
    Definiton.type = Tob2BodyType(BodyType);
    Definiton.fixedRotation = FixedRotation;
    Definiton.userData = this;

    Body = PhysicsWorld2D->GetWorld().CreateBody(&Definiton);

    GetOwner()->GetTransform().AddPositionCallback(this, [&](const Vector2& /*NewPosition*/) {
        Vector2 Tmp = GetOwner()->GetTransform().GetWorldPosition();
        Body->SetTransform( Tob2Vec2(Tmp), Body->GetAngle() );
    });
    GetOwner()->GetTransform().AddAngleCallback(this, [&](const float /*NewAngle*/) {
        Body->SetTransform( Body->GetPosition(), Math::DegToRad(GetOwner()->GetParent()->GetTransform().GetWorldAngle()) );
    });
}

bool CRigidBody2D::OnLoad(CXMLElement* Root)
{
	BodyType = XML::LoadEnum( Root, "BodyType", ERigidBody2DType::Dynamic);
    SetBodyType(BodyType);
    //
	float Angle = XML::LoadFloat( Root, "Angle", 0.0f );
	Vector2 Position = XML::LoadVector2( Root, "Position", Vector2::ZERO() );
    Body->SetTransform( Tob2Vec2(Position), Math::DegToRad(Angle) );
    //
	FixedRotation = XML::LoadBool( Root, "FixedRotation", false);
    SetFixedRotation(FixedRotation);

	Vector2 LinearVelocity = XML::LoadVector2( Root, "LinearVelocity", Vector2::ZERO() );
    SetLinearVelocity(LinearVelocity);

	LinearDamping = XML::LoadFloat( Root, "LinearDamping", 0.0f );
    SetLinearDamping(LinearDamping);

	float AngularVelocity = XML::LoadFloat( Root, "AngularVelocity", 0.0f);
    SetAngularVelocity(AngularVelocity);

	AngularDamping = XML::LoadFloat( Root, "AngularDamping", 0.0f);
    SetAngularDamping(AngularDamping);
    //
	bool IsEnabled = XML::LoadBool( Root, "IsEnabled", true);
    Body->SetEnabled(IsEnabled);
    return true;
}

bool CRigidBody2D::OnSave(CXMLElement* Root)
{
    XML::SaveEnum( Root, "BodyType", BodyType);
    //
    float Angle = Math::RadToDeg( Body->GetAngle() );
    Vector2 Position = ToVector2(Body->GetPosition());
    XML::SaveFloat( Root, "Angle", Angle);
    XML::SaveVector2( Root, "Position", Position);
    //
    XML::SaveBool( Root, "FixedRotation", FixedRotation);

    Vector2 LinearVelocity = ToVector2(Body->GetLinearVelocity());
    XML::SaveVector2( Root, "LinearVelocity", LinearVelocity);

    XML::SaveFloat( Root, "LinearDamping", LinearDamping);

    float AngularVelocity = Math::RadToDeg(Body->GetAngularVelocity());
    XML::SaveFloat( Root, "AngularVelocity", AngularVelocity);

    XML::SaveFloat( Root, "AngularDamping", AngularDamping);
    //
    bool IsEnabled = Body->IsEnabled();
    XML::SaveBool( Root, "IsEnabled", IsEnabled);
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
    float Angle = Math::RadToDeg( Body->GetAngle() );
    Vector2 Position = ToVector2(Body->GetPosition());

    Owner->GetTransform().SetWorldPosition(Position, true);
    Owner->GetTransform().SetWorldAngle(Angle, true);
}

Vector2 CRigidBody2D::GetLinearVelocity() const
{
    return ToVector2(Body->GetLinearVelocity());
}

float CRigidBody2D::GetAngularVelocity() const
{
    return Math::RadToDeg(Body->GetAngularVelocity());
}

void CRigidBody2D::SetBodyType(const ERigidBody2DType aType)
{
    BodyType = aType;
    Body->SetType( Tob2BodyType(BodyType) );
}

void CRigidBody2D::SetFixedRotation(const bool Value)
{
    FixedRotation = Value;
    Body->SetFixedRotation(FixedRotation);
}

void CRigidBody2D::AddForce(const Vector2& Force)
{
    Body->ApplyForce( Tob2Vec2(Force), Body->GetWorldCenter(), true );
}

void CRigidBody2D::AddTorque(const float Torque)
{
    Body->ApplyTorque( Torque, true );
}

void CRigidBody2D::AddLinearImpulse(const Vector2& Impulse)
{
    Body->ApplyLinearImpulse( Tob2Vec2(Impulse), Body->GetWorldCenter(), true );
}

void CRigidBody2D::AddAngularImpulse(const float Impulse)
{
    Body->ApplyAngularImpulse( Impulse, true );
}

float CRigidBody2D::GetMass() const
{
    return Body->GetMass();
}

void CRigidBody2D::SetLinearVelocity(const Vector2& Velocity)
{
    Body->SetLinearVelocity( Tob2Vec2(Velocity) );
}

void CRigidBody2D::SetAngularVelocity(const float Velocity)
{
    Body->SetAngularVelocity( Math::DegToRad(Velocity) );
}

void CRigidBody2D::SetLinearDamping(const float Damping)
{
    LinearDamping = Damping;
    Body->SetLinearDamping(Damping);
}

void CRigidBody2D::SetAngularDamping(const float Damping)
{
    AngularDamping = Damping;
    Body->SetAngularDamping(Damping);
}
