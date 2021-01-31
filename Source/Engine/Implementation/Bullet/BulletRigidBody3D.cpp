#include "BulletRigidBody3D.hpp"
#include "BulletPhysicsWorld3D.hpp"
#include "BulletCollisionShape3D.hpp"
#include "BulletUtils.hpp"

CBulletRigidBody3D::CBulletRigidBody3D(CBulletPhysicsWorld3D* aWorld, CBulletCollisionShape3D* aShape):
	World(aWorld),
	Shape(aShape)
{
	btTransform StartTransform;
	StartTransform.setIdentity();

	btVector3 LocalInertia(0, 0, 0);
	if (BodyType == ERigidBodyType3D::Dynamic)
	{
		Shape->GetShape()->calculateLocalInertia(Mass, LocalInertia);
	}
	// Using motionstate is optional, it provides interpolation capabilities, and only synchronizes 'active' objects
	MotionState = std::make_unique<btDefaultMotionState>(StartTransform);

	btRigidBody::btRigidBodyConstructionInfo RBInfo(Mass, MotionState.get(), Shape->GetShape(), LocalInertia);
	Body = std::make_unique<btRigidBody>(RBInfo);
	Body->setUserPointer(this);

	// Add the body to the dynamics world
	World->GetWorld()->addRigidBody(Body.get());
}

CBulletRigidBody3D::~CBulletRigidBody3D()
{
	World->GetWorld()->removeRigidBody( Body.get() );
	Body.reset();
	MotionState.reset();
}

IPhysicsWorld3D* CBulletRigidBody3D::GetWorld() const
{
	return World;
}

void CBulletRigidBody3D::SetPosition(const Vector3& Position)
{
	auto& Tr = Body->getWorldTransform();
	Tr.setOrigin( ToBulletVector3(Position) );
}

Vector3 CBulletRigidBody3D::GetPosition() const
{
	const auto& Tr = Body->getWorldTransform();
	return ToVector3( Tr.getOrigin() );
}

void CBulletRigidBody3D::SetRotation(const Quaternion& Rotation)
{
	auto& Tr = Body->getWorldTransform();
	Tr.setRotation(ToBulletQuaternion(Rotation));
}

Quaternion CBulletRigidBody3D::GetRotation() const
{
	const auto& Tr = Body->getWorldTransform();
	return ToQuaternion(Tr.getRotation());
}

// TO DO
void CBulletRigidBody3D::SetBodyType(const ERigidBodyType3D Type)
{
	//Body->SetType(Tob2BodyType(aType));
}

ERigidBodyType3D CBulletRigidBody3D::GetBodyType() const
{
	return BodyType;
}