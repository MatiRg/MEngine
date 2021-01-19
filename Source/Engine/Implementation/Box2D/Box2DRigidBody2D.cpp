#include "Box2DRigidBody2D.hpp"
#include "Box2DPhysicsWorld2D.hpp"
#include "Box2DUtils.hpp"
#include "../../Math/Functions.hpp"

namespace
{
    b2BodyType Tob2BodyType(const ERigidBodyType2D Type)
    {
        switch(Type)
        {
        case ERigidBodyType2D::Static:
            return b2_staticBody;
        case ERigidBodyType2D::Kinematic:
            return b2_kinematicBody;
        case ERigidBodyType2D::Dynamic:
        default:
            return b2_dynamicBody;
        }
    }

    ERigidBodyType2D ToBodyType2D(const b2BodyType Type)
    {
        switch (Type)
        {
        case b2_staticBody:
            return ERigidBodyType2D::Static;
        case b2_kinematicBody:
            return ERigidBodyType2D::Kinematic;
        case b2_dynamicBody:
        default:
            return ERigidBodyType2D::Dynamic;
        }
    }
}

CBox2DRigidBody2D::CBox2DRigidBody2D(CBox2DPhysicsWorld2D* aWorld, CBox2DCollisionShape2D* aShape):
    IRigidBody2D(aWorld, aShape)
{
    b2BodyDef Definiton;
    Definiton.position = b2Vec2_zero;
    Definiton.angle = Math::DegToRad(0.0f);
    Definiton.linearVelocity = { 0.0f, 0.0f };
    Definiton.linearDamping = 0.0f;
    Definiton.angularVelocity = Math::DegToRad(0.0f);
    Definiton.angularDamping = 0.0f;
    Definiton.type = b2_dynamicBody;
    Definiton.fixedRotation = false;
    Definiton.userData = this;

    Body = aWorld->GetWorld().CreateBody(&Definiton);

    aShape->SetRigidBody(this);
}

CBox2DRigidBody2D::~CBox2DRigidBody2D()
{
    Body->GetWorld()->DestroyBody(Body);
}

void CBox2DRigidBody2D::SetPosition(const Vector2& x)
{
    Body->SetTransform(Tob2Vec2(x), Body->GetAngle());
}

Vector2 CBox2DRigidBody2D::GetPosition() const
{
    return ToVector2(Body->GetPosition());
}

void CBox2DRigidBody2D::SetAngle(const float x)
{
    Body->SetTransform(Body->GetPosition(), Math::DegToRad(x));
}

float CBox2DRigidBody2D::GetAngle() const
{
    return Math::RadToDeg(Body->GetAngle());
}

void CBox2DRigidBody2D::SetEnabled(const bool x)
{
    Body->SetEnabled(x);
}

bool CBox2DRigidBody2D::IsEnabled() const
{
    return Body->IsEnabled();
}

Vector2 CBox2DRigidBody2D::GetLinearVelocity() const
{
    return ToVector2(Body->GetLinearVelocity());
}

float CBox2DRigidBody2D::GetAngularVelocity() const
{
    return Math::RadToDeg(Body->GetAngularVelocity());
}

void CBox2DRigidBody2D::SetBodyType(const ERigidBodyType2D aType)
{
    Body->SetType( Tob2BodyType(aType) );
}

ERigidBodyType2D CBox2DRigidBody2D::GetBodyType() const
{
    return ToBodyType2D(Body->GetType());
}

void CBox2DRigidBody2D::SetFixedRotation(const bool Value)
{
    Body->SetFixedRotation(Value);
}

bool CBox2DRigidBody2D::HasFixedRotation() const
{ 
    return Body->IsFixedRotation();
}

void CBox2DRigidBody2D::AddForce(const Vector2& Force)
{
    Body->ApplyForce( Tob2Vec2(Force), Body->GetWorldCenter(), true );
}

void CBox2DRigidBody2D::AddTorque(const float Torque)
{
    Body->ApplyTorque( Torque, true );
}

void CBox2DRigidBody2D::AddLinearImpulse(const Vector2& Impulse)
{
    Body->ApplyLinearImpulse( Tob2Vec2(Impulse), Body->GetWorldCenter(), true );
}

void CBox2DRigidBody2D::AddAngularImpulse(const float Impulse)
{
    Body->ApplyAngularImpulse( Impulse, true );
}

void CBox2DRigidBody2D::SetMass(const float x)
{
    if (x == 0.0f)
    {
        SetBodyType(ERigidBodyType2D::Static);
        return;
    }
    Collider->SetDensity( x / (Collider->GetSize().x* Collider->GetSize().y) );
}

float CBox2DRigidBody2D::GetMass() const
{
    return Body->GetMass();
}

void CBox2DRigidBody2D::SetLinearVelocity(const Vector2& Velocity)
{
    Body->SetLinearVelocity( Tob2Vec2(Velocity) );
}

void CBox2DRigidBody2D::SetAngularVelocity(const float Velocity)
{
    Body->SetAngularVelocity( Math::DegToRad(Velocity) );
}

void CBox2DRigidBody2D::SetLinearDamping(const float Value)
{
    Body->SetLinearDamping(Value);
}

void CBox2DRigidBody2D::SetAngularDamping(const float Value)
{
    Body->SetAngularDamping(Value);
}

float CBox2DRigidBody2D::GetLinearDamping() const
{ 
    return Body->GetLinearDamping(); 
}

float CBox2DRigidBody2D::GetAngularDamping() const
{ 
    return Body->GetAngularDamping(); 
}