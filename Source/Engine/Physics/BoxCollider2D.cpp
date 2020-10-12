#include "BoxCollider2D.hpp"
#include "RigidBody2D.hpp"
#include "Box2DUtils.hpp"
#include "../Core/Log.hpp"
#include "../Core/XML.hpp"
#include "../Scene/Entity2D.hpp"
#include "../Scene/Transform2D.hpp"

CBoxCollider2D::CBoxCollider2D(CEngine* aEngine): 
    IComponent(aEngine)
{
}

CBoxCollider2D::~CBoxCollider2D()
{
    GetOwner()->GetTransform().RemoveScaleCallback(this);
    DestroyFixture();
}

void CBoxCollider2D::OnCreate()
{
    CreateFixture();
    GetOwner()->GetTransform().AddScaleCallback(this, [&](const Vector2& /*NewScale*/) {
        SetSize( GetSize() );
    });
}

bool CBoxCollider2D::OnLoad(CXMLElement* Root)
{
	Size = XML::LoadVector2( Root, "Size", Vector2::ONE());
    SetSize(Size);
    //
	Friction = XML::LoadFloat(Root, "Friction", 0.2f);
    SetFriction(Friction);
	Restitution = XML::LoadFloat( Root, "Restitution", 0.0f);
    SetRestitution(Restitution);
	Density = XML::LoadFloat( Root, "Density", 1.0f);
    SetDensity(Density);
	Offset = XML::LoadVector2( Root, "Offset", Vector2::ZERO());
    SetOffset(Offset);
    //
	Sensor = XML::LoadBool( Root, "Sensor", false);
    SetSensor(Sensor);
    return true;
}

bool CBoxCollider2D::OnSave(CXMLElement* Root)
{
    XML::SaveVector2( Root, "Size", Size );
    //
    XML::SaveFloat( Root, "Friction", Friction);
    XML::SaveFloat( Root, "Restitution", Restitution);
    XML::SaveFloat( Root, "Density", Density);
    XML::SaveVector2( Root, "Offset", Offset);
    XML::SaveBool( Root, "Sensor", Sensor);
    return true;
}

void CBoxCollider2D::SetSize(const Vector2& aSize) 
{ 
    Size = aSize;
    if( Fixture )
    {
        DestroyFixture();
        CreateFixture();
    }
}

void CBoxCollider2D::SetFriction(const float Value)
{
    Friction = Value;
    if( Fixture )
    {
        Fixture->SetFriction(Friction);
    }
}

void CBoxCollider2D::SetRestitution(const float Value)
{
    Restitution = Value;
    if( Fixture )
    {
        Fixture->SetRestitution(Restitution);
    }
}

void CBoxCollider2D::SetDensity(const float Value)
{
    Density = Value;
    if( Fixture )
    {
        Fixture->SetDensity(Density);
        Fixture->GetBody()->ResetMassData();
    }
}

void CBoxCollider2D::SetOffset(const Vector2& Value)
{
    Offset = Value;
    if( Fixture )
    {
        DestroyFixture();
        CreateFixture();
    }
}

void CBoxCollider2D::SetSensor(const bool Value)
{
    Sensor = Value;
    if( Fixture )
    {
        Fixture->SetSensor(Sensor);
    }
}

void CBoxCollider2D::DestroyFixture()
{
    if( Owner->HasComponent<CRigidBody2D>() ) // Fixtures will be destroyed when body is detroyed
    {
        auto RB = Owner->GetComponent<CRigidBody2D>();
        RB->GetBody()->DestroyFixture(Fixture);
        Fixture = nullptr;
    }
}

void CBoxCollider2D::CreateFixture()
{
    if( Owner->HasComponent<CRigidBody2D>() )
    {
        auto RB = Owner->GetComponent<CRigidBody2D>();

        CTransform2D Ts = Owner->GetTransform().GetWorldTransform();
        Vector2 NewSize = Size*Ts.GetScale();

        b2PolygonShape PolygonShape;
        PolygonShape.SetAsBox( NewSize.x/2.0f, NewSize.y/2.0f );
        PolygonShape.m_centroid = Tob2Vec2( Offset );

        b2FixtureDef FixtureDef;
        FixtureDef.shape = &PolygonShape;
        FixtureDef.friction = Friction;
        FixtureDef.restitution = Restitution;
        FixtureDef.density = Density;
        FixtureDef.isSensor = Sensor;
        FixtureDef.userData = this;

        Fixture = RB->GetBody()->CreateFixture(&FixtureDef);
    }
}