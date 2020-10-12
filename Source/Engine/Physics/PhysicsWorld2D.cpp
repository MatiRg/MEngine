#include "PhysicsWorld2D.hpp"
#include "Physics2D.hpp"
#include "RigidBody2D.hpp"
#include "BoxCollider2D.hpp"
#include "Box2DUtils.hpp"
#include "Contact2D.hpp"
#include "../Engine/Engine.hpp"
#include "../Scene/Entity2D.hpp"
#include "../Core/Log.hpp"
#include "../Core/XML.hpp"

void CPhysicsWorld2D::CContactListener::BeginContact(b2Contact* Contact)
{ 
    auto RigidBodyA = static_cast<CRigidBody2D*>( Contact->GetFixtureA()->GetBody()->GetUserData() );
    auto ColliderA = static_cast<CBoxCollider2D*>( Contact->GetFixtureA()->GetUserData() );

    auto RigidBodyB = static_cast<CRigidBody2D*>( Contact->GetFixtureB()->GetBody()->GetUserData() );
    auto ColliderB = static_cast<CBoxCollider2D*>( Contact->GetFixtureB()->GetUserData() );

    auto EntityA = RigidBodyA->GetOwner();
    auto EntityB = RigidBodyB->GetOwner();

    EntityA->OnCollisionEnter( {RigidBodyB, ColliderB} );
    EntityB->OnCollisionEnter( {RigidBodyA, ColliderA} );
}

void CPhysicsWorld2D::CContactListener::EndContact(b2Contact* Contact)
{ 
    auto RigidBodyA = static_cast<CRigidBody2D*>( Contact->GetFixtureA()->GetBody()->GetUserData() );
    auto ColliderA = static_cast<CBoxCollider2D*>( Contact->GetFixtureA()->GetUserData() );

    auto RigidBodyB = static_cast<CRigidBody2D*>( Contact->GetFixtureB()->GetBody()->GetUserData() );
    auto ColliderB = static_cast<CBoxCollider2D*>( Contact->GetFixtureB()->GetUserData() );

    auto EntityA = RigidBodyA->GetOwner();
    auto EntityB = RigidBodyB->GetOwner();

    EntityA->OnCollisionLeave( {RigidBodyB, ColliderB} );
    EntityB->OnCollisionLeave( {RigidBodyA, ColliderA} );
}

//

CPhysicsWorld2D::CPhysicsWorld2D(CEngine* aEngine): 
    IComponent(aEngine),
    Gravity(0.0f, -9.71f),
    WorldBox2D( Tob2Vec2(Gravity) )
{
    Engine->GetPhysics2D()->SetWorld(this);
    WorldBox2D.SetContactListener( &Listener );
    WorldBox2D.SetDebugDraw( Engine->GetPhysics2D()->GetDebugDrawer() );
    LOG(ESeverity::Debug) << "Physics World 2D - Created\n";
}

CPhysicsWorld2D::~CPhysicsWorld2D()
{
    Engine->GetPhysics2D()->SetWorld(nullptr);
    LOG(ESeverity::Debug) << "Physics World 2D - Destroyed\n";
}

bool CPhysicsWorld2D::OnLoad(CXMLElement* Root)
{
	Gravity = XML::LoadVector2( Root, "Gravity", Vector2(0.0f, -9.71f) );
    SetGravity(Gravity);
    //
	DebugDraw = XML::LoadBool( Root, "DebugDraw", false );
    return true;
}

bool CPhysicsWorld2D::OnSave(CXMLElement* Root)
{
    XML::SaveVector2( Root, "Gravity", Gravity );
    //
    XML::SaveBool( Root, "DebugDraw", DebugDraw );
    return true;
}

void CPhysicsWorld2D::SetGravity(const Vector2& aGravity)
{
    Gravity = aGravity;
    WorldBox2D.SetGravity( Tob2Vec2(Gravity) );
}

void CPhysicsWorld2D::OnRender()
{
    if( DebugDraw )
    {
        WorldBox2D.DebugDraw();
    }
}