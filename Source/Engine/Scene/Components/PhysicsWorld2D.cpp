#include "PhysicsWorld2D.hpp"
#include "RigidBody2D.hpp"
#include "BoxCollider2D.hpp"
#include "../../Physics/RigidBody2D.hpp"
#include "../../Physics/CollisionShape2D.hpp"
#include "../../Physics/Contact2D.hpp"
#include "../../Physics/Physics2D.hpp"
#include "../../Engine/Engine.hpp"
#include "../../Core/XML.hpp"
#include "../Entity.hpp"

namespace
{
    void OnCollisionEnter(const SContact2D& aContact, const SContact2D& bContact)
    {
        auto RigidBodyA = aContact.RigidBody->GetUserDataAs<CRigidBody2D>();
        auto ColliderA = aContact.Collider->GetUserDataAs<CBoxCollider2D>();

        auto RigidBodyB = bContact.RigidBody->GetUserDataAs<CRigidBody2D>();
        auto ColliderB = bContact.Collider->GetUserDataAs<CBoxCollider2D>();

        auto EntityA = RigidBodyA->GetOwner();
        auto EntityB = RigidBodyB->GetOwner();

        EntityA->OnCollisionEnter({ RigidBodyB, ColliderB });
        EntityB->OnCollisionEnter({ RigidBodyA, ColliderA });
    }

    void OnCollisionLeave(const SContact2D& aContact, const SContact2D& bContact)
    {
        auto RigidBodyA = aContact.RigidBody->GetUserDataAs<CRigidBody2D>();
        auto ColliderA = aContact.Collider->GetUserDataAs<CBoxCollider2D>();

        auto RigidBodyB = bContact.RigidBody->GetUserDataAs<CRigidBody2D>();
        auto ColliderB = bContact.Collider->GetUserDataAs<CBoxCollider2D>();

        auto EntityA = RigidBodyA->GetOwner();
        auto EntityB = RigidBodyB->GetOwner();

        EntityA->OnCollisionLeave({ RigidBodyB, ColliderB });
        EntityB->OnCollisionLeave({ RigidBodyA, ColliderA });
    }
}

//

CPhysicsWorld2D::CPhysicsWorld2D(CEngine* aEngine): 
    IComponent(aEngine)
{
}

CPhysicsWorld2D::~CPhysicsWorld2D()
{
}

void CPhysicsWorld2D::OnCreate()
{
    World = Engine->GetPhysics2D()->CreateWorld( {0.0f, -10.0f} );
    World->SetEnterCollisionCallback(OnCollisionEnter);
    World->SetLeaveCollisionCallback(OnCollisionLeave);
}

bool CPhysicsWorld2D::OnLoad(CXMLElement* Root)
{
	World->SetGravity( XML::LoadVector2( Root, "Gravity", Vector2(0.0f, -10.0f) ) );
    //
	DebugDraw = XML::LoadBool( Root, "DebugDraw", false );
    return true;
}

bool CPhysicsWorld2D::OnSave(CXMLElement* Root)
{
    XML::SaveVector2( Root, "Gravity", World->GetGravity() );
    //
    XML::SaveBool( Root, "DebugDraw", DebugDraw );
    return true;
}

void CPhysicsWorld2D::SetGravity(const Vector2& aGravity)
{
    World->SetGravity( aGravity );
}

const Vector2& CPhysicsWorld2D::GetGravity() const
{
    return World->GetGravity();
}

void CPhysicsWorld2D::OnUpdate(const float DT)
{
    World->OnUpdate(DT);
}

void CPhysicsWorld2D::OnRender()
{
    if( DebugDraw )
    {
        World->DebugDraw(Engine->GetRenderer2D());
    }
}
