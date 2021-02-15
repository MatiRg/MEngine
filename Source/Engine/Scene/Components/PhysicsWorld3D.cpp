#include "PhysicsWorld3D.hpp"
#include "RigidBody3D.hpp"
#include "BoxCollider3D.hpp"
#include "../../Physics/RigidBody3D.hpp"
#include "../../Physics/CollisionShape3D.hpp"
#include "../../Physics/Physics3D.hpp"
#include "../../Engine/Engine.hpp"
#include "../../Core/XML.hpp"
#include "../Entity.hpp"

#define WORLD_PHYSICS_3D_CALLBACK(Func) \
    CRigidBody3D* BodyA = Contact.BodyA->GetUserDataAs<CRigidBody3D>(); \
    CRigidBody3D* BodyB = Contact.BodyB->GetUserDataAs<CRigidBody3D>(); \
    CEntity* EntityA = BodyA->GetOwner(); \
    CEntity* EntityB = BodyB->GetOwner(); \
    SEntityCollision3D DataA(BodyB, EntityB, Contact.ContactPoints); \
    EntityA-> Func (DataA); \
    SEntityCollision3D DataB(BodyA, EntityA, Contact.ContactPoints); \
    EntityB-> Func (DataB); 

class CWorldContactCallback3D : public IContactCallback3D
{
public:
    void OnCollisionEnter(const SContact3D& Contact) override 
    {
        WORLD_PHYSICS_3D_CALLBACK(OnCollisionEnter)
    }

    void OnCollisionStay(const SContact3D& Contact) override
    {
        WORLD_PHYSICS_3D_CALLBACK(OnCollisionStay)
    }

    void OnCollisionLeave(const SContact3D& Contact) override
    {
        WORLD_PHYSICS_3D_CALLBACK(OnCollisionLeave)
    }
};

#undef WORLD_PHYSICS_3D_CALLBACK

//

CPhysicsWorld3D::CPhysicsWorld3D(CEngine* aEngine): 
    IComponent(aEngine)
{
}

CPhysicsWorld3D::~CPhysicsWorld3D()
{
}

void CPhysicsWorld3D::OnCreate()
{
    Callback = std::make_unique<CWorldContactCallback3D>();
    World = Engine->GetPhysics3D()->CreateWorld( {0.0f, -10.0f, 0.0f} );
    World->SetContactCallback(Callback.get());
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
