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
    if (Body)
    {
        PhysicsWorld3D->GetWorld()->DestroyBody(Body);
    }
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
    //Body->SetEnabled(true);
}

void CRigidBody3D::OnDisabled()
{
    //Body->SetEnabled(false);
}

void CRigidBody3D::OnBeginFrame()
{
    Owner->GetTransform().SetWorldPosition(Body->GetPosition(), true);
    Owner->GetTransform().SetWorldRotation(Body->GetRotation(), true);
}
