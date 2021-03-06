#include "BoxCollider3D.hpp"
#include "PhysicsWorld3D.hpp"
#include "RigidBody3D.hpp"
#include "../../Physics/PhysicsWorld3D.hpp"
#include "../../Core/XML.hpp"
#include "../../Math/Transform.hpp"
#include "../Entity.hpp"
#include "../World.hpp"

CBoxCollider3D::CBoxCollider3D(CEngine* aEngine): 
    IComponent(aEngine)
{
}

CBoxCollider3D::~CBoxCollider3D()
{
    GetOwner()->GetTransform().RemoveScaleCallback(this);
    PhysicsWorld3D->GetWorld()->DestroyShape(Shape);
    // TO DO: When Has RigidBody ?
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
    //
    SetSize({0.5f});
    //
    GetOwner()->GetTransform().AddScaleCallback(this, [&](const Vector3& /*NewScale*/) {
        SetSize(GetSize());
    });
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

void CBoxCollider3D::SetSize(const Vector3& aSize)
{
    Size = aSize;
    //
    ICollisionShape3D* OldShape = Shape;
    
    Shape = PhysicsWorld3D->GetWorld()->CreateBoxShape(Size * GetOwner()->GetTransform().GetWorldScale());
    Shape->SetUserData(this);

    CRigidBody3D* Body = GetOwner()->GetComponent<CRigidBody3D>();
    if( Body )
    {
        Body->GetBody()->SetCollisionShape(Shape);
    }

    if (OldShape)
    {
        PhysicsWorld3D->GetWorld()->DestroyShape(OldShape);
    }
}