#include "Box2DCollisionShape2D.hpp"
#include "Box2DRigidBody2D.hpp"
#include "Box2DUtils.hpp"

CBox2DCollisionShape2D::CBox2DCollisionShape2D()
{
}

CBox2DCollisionShape2D::~CBox2DCollisionShape2D()
{
    DestroyFixture();
}

IRigidBody2D* CBox2DCollisionShape2D::GetRigidBody() const
{ 
    return Rigidbody; 
}

void CBox2DCollisionShape2D::SetRigidBody(CBox2DRigidBody2D* aBody)
{
    Rigidbody = aBody;
    CreateFixture();
}

void CBox2DCollisionShape2D::CreateBox(const Vector2& aSize, const Vector2& aOffset)
{
    Size = aSize;
    Offset = aOffset;
}

void CBox2DCollisionShape2D::SetSize(const Vector2& aSize)
{ 
    Size = aSize;
    if( Fixture )
    {
        DestroyFixture();
        CreateFixture();
    }
}

void CBox2DCollisionShape2D::SetFriction(const float Value)
{
    Friction = Value;
    if( Fixture )
    {
        Fixture->SetFriction(Friction);
    }
}

void CBox2DCollisionShape2D::SetRestitution(const float Value)
{
    Restitution = Value;
    if( Fixture )
    {
        Fixture->SetRestitution(Restitution);
    }
}

void CBox2DCollisionShape2D::SetDensity(const float Value)
{
    Density = Value;
    if( Fixture )
    {
        Fixture->SetDensity(Density);
        Fixture->GetBody()->ResetMassData();
    }
}

void CBox2DCollisionShape2D::SetOffset(const Vector2& Value)
{
    Offset = Value;
    if( Fixture )
    {
        DestroyFixture();
        CreateFixture();
    }
}

void CBox2DCollisionShape2D::SetSensor(const bool Value)
{
    Sensor = Value;
    if( Fixture )
    {
        Fixture->SetSensor(Sensor);
    }
}

void CBox2DCollisionShape2D::DestroyFixture()
{
    if (!Rigidbody)
    {
        return;
    }

    Rigidbody->GetBody()->DestroyFixture(Fixture);
    Fixture = nullptr;
}

void CBox2DCollisionShape2D::CreateFixture()
{
    if (!Rigidbody || Fixture)
    {
        return;
    }

    b2PolygonShape PolygonShape;
    PolygonShape.SetAsBox( Size.x/2.0f, Size.y/2.0f );
    PolygonShape.m_centroid = Tob2Vec2( Offset );
    
    b2FixtureDef FixtureDef;
    FixtureDef.shape = &PolygonShape;
    FixtureDef.friction = Friction;
    FixtureDef.restitution = Restitution;
    FixtureDef.density = Density;
    FixtureDef.isSensor = Sensor;
    FixtureDef.userData = this;
    
    Fixture = Rigidbody->GetBody()->CreateFixture(&FixtureDef);
}