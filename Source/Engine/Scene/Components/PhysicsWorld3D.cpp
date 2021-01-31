#include "PhysicsWorld3D.hpp"
#include "RigidBody3D.hpp"
#include "BoxCollider3D.hpp"
#include "../../Physics/RigidBody3D.hpp"
#include "../../Physics/CollisionShape3D.hpp"
#include "../../Physics/Physics3D.hpp"
#include "../../Engine/Engine.hpp"
#include "../../Core/XML.hpp"
#include "../Entity.hpp"

CPhysicsWorld3D::CPhysicsWorld3D(CEngine* aEngine): 
    IComponent(aEngine)
{
}

CPhysicsWorld3D::~CPhysicsWorld3D()
{
}

void CPhysicsWorld3D::OnCreate()
{
    World = Engine->GetPhysics3D()->CreateWorld( {0.0f, -10.0f, 0.0f} );
}

bool CPhysicsWorld3D::OnLoad(CXMLElement* Root)
{
	World->SetGravity( XML::LoadVector3( Root, "Gravity", Vector3(0.0f, -10.0f, 0.0f) ) );
    //
	DebugDraw = XML::LoadBool( Root, "DebugDraw", false );
    return true;
}

bool CPhysicsWorld3D::OnSave(CXMLElement* Root)
{
    XML::SaveVector3( Root, "Gravity", World->GetGravity() );
    //
    XML::SaveBool( Root, "DebugDraw", DebugDraw );
    return true;
}

void CPhysicsWorld3D::SetGravity(const Vector3& Gravity)
{
    World->SetGravity(Gravity);
}

Vector3 CPhysicsWorld3D::GetGravity() const
{
    return World->GetGravity();
}

void CPhysicsWorld3D::OnUpdate(const float DT)
{
    World->OnUpdate(DT);
}

void CPhysicsWorld3D::OnRender()
{
    if( DebugDraw )
    {
        //World->DebugDraw(Engine->GetRenderer2D());
    }
}
