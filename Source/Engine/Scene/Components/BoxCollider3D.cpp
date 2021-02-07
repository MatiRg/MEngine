#include "BoxCollider3D.hpp"
#include "PhysicsWorld3D.hpp"
#include "RigidBody3D.hpp"
#include "../../Physics/PhysicsWorld3D.hpp"
#include "../../Core/XML.hpp"
#include "../Entity.hpp"
#include "../Transform.hpp"
#include "../World.hpp"

CBoxCollider3D::CBoxCollider3D(CEngine* aEngine): 
    IComponent(aEngine)
{
}

CBoxCollider3D::~CBoxCollider3D()
{
    if (Shape)
    {
        PhysicsWorld3D->GetWorld()->DestroyShape(Shape);
    }
}

void CBoxCollider3D::OnCreate()
{
    CWorld* World = GetOwner()->GetWorld();
    //
    PhysicsWorld3D = World->GetComponent<CPhysicsWorld3D>();
    if (!PhysicsWorld3D)
    {
        PhysicsWorld3D = World->CreateComponent<CPhysicsWorld3D>();
    }
    Shape = PhysicsWorld3D->GetWorld()->CreateBoxShape({ 0.5f });
    Shape->SetUserData(this);
    /*CRigidBody3D* Body = GetOwner()->GetComponent<CRigidBody3D>();
    if (!Body)
    {
        Body = GetOwner()->CreateComponent<CRigidBody3D>();
    }*/
}

bool CBoxCollider3D::OnLoad(CXMLElement* Root)
{
    //SetSize( XML::LoadVector2( Root, "Size", Vector2::ONE) );
    ////
    //SetFriction( XML::LoadFloat(Root, "Friction", 0.2f) );
    //SetRestitution( XML::LoadFloat( Root, "Restitution", 0.0f) );
    //SetDensity( XML::LoadFloat( Root, "Density", 1.0f) );
    //SetOffset(XML::LoadVector2( Root, "Offset", Vector2::ZERO));
    ////
    //SetSensor(XML::LoadBool(Root, "Sensor", false));
    return true;
}

bool CBoxCollider3D::OnSave(CXMLElement* Root)
{
    //XML::SaveVector2( Root, "Size", GetSize() );
    ////
    //XML::SaveFloat( Root, "Friction", GetFriction() );
    //XML::SaveFloat( Root, "Restitution", GetRestitution() );
    //XML::SaveFloat( Root, "Density", GetDensity());
    //XML::SaveVector2( Root, "Offset", GetOffset() );
    //XML::SaveBool( Root, "Sensor", GetSensor() );
    return true;
}