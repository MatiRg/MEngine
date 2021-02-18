#include "BulletRigidBody3D.hpp"
#include "BulletPhysicsWorld3D.hpp"
#include "BulletCollisionShape3D.hpp"
#include "BulletUtils.hpp"
#include "../../Math/Functions.hpp"
#include <limits>

CBulletRigidBody3D::CBulletRigidBody3D(CBulletPhysicsWorld3D* aWorld, CBulletCollisionShape3D* aShape):
	World(aWorld),
	Shape(aShape),
	CollisionLayer(-1),
	CollisionMask(-1)
{
	BodyType = ERigidBodyType3D::Dynamic;

	btTransform StartTransform;
	StartTransform.setIdentity();

	btVector3 LocalInertia(0, 0, 0);
	Shape->GetShape()->calculateLocalInertia(1.0f, LocalInertia);
	// Using motionstate is optional, it provides interpolation capabilities, and only synchronizes 'active' objects
	MotionState = std::make_unique<btDefaultMotionState>(StartTransform);

	btRigidBody::btRigidBodyConstructionInfo RBInfo(1.0f, MotionState.get(), Shape->GetShape(), LocalInertia);
	Body = std::make_unique<btRigidBody>(RBInfo);
	Body->setUserPointer(this);
	DefaultCollisionFlags = Body->getCollisionFlags();
	DefaultActivationState = Body->getActivationState();

	// Add the body to the dynamics world
	AddRigidBodyToWorld();
}

CBulletRigidBody3D::~CBulletRigidBody3D()
{
	RemoveRigidBodyFromWorld();
	Body.reset();
	MotionState.reset();
}

IPhysicsWorld3D* CBulletRigidBody3D::GetWorld() const
{
	return World;
}

void CBulletRigidBody3D::SetCollisionLayer(int Layer)
{
	RemoveRigidBodyFromWorld();
	CollisionLayer = Layer;
	AddRigidBodyToWorld();
}

int CBulletRigidBody3D::GetCollisionLayer() const
{
	return CollisionLayer;
}

void CBulletRigidBody3D::SetCollisionMask(int Mask)
{
	RemoveRigidBodyFromWorld();
	CollisionMask = Mask;
	AddRigidBodyToWorld();
}

int CBulletRigidBody3D::GetCollisionMask() const
{
	return CollisionMask;
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
	Body->updateInertiaTensor();
}

Quaternion CBulletRigidBody3D::GetRotation() const
{
	const auto& Tr = Body->getWorldTransform();
	return ToQuaternion(Tr.getRotation());
}

void CBulletRigidBody3D::SetGravity(const Vector3& x)
{
	Body->setGravity( ToBulletVector3(x) );
}

Vector3 CBulletRigidBody3D::GetGravity() const
{
	return ToVector3(Body->getGravity());
}

void CBulletRigidBody3D::SetMass(float Mass)
{
	RemoveRigidBodyFromWorld();
	//
	Mass = Math::Max(Mass, 0.0f);
	btVector3 LocalInertia(0, 0, 0);
	Shape->GetShape()->calculateLocalInertia(Mass, LocalInertia);
	Body->setMassProps(Mass, LocalInertia);
	Body->updateInertiaTensor();
	//
	if (Mass != 0.0f)
	{
		BodyType = ERigidBodyType3D::Dynamic;
		// ?
		Body->setCollisionFlags(DefaultCollisionFlags);
		Body->setActivationState(DefaultActivationState);
	}
	else
	{
		if (Body->isKinematicObject())
		{
			BodyType = ERigidBodyType3D::Kinematic;
		}
		else
		{
			BodyType = ERigidBodyType3D::Static;
		}
	}
	//
	AddRigidBodyToWorld();
}

float CBulletRigidBody3D::GetMass() const
{
	return Body->getMass();
}

void CBulletRigidBody3D::SetLinearVelocity(const Vector3& x)
{
	Activate();
	Body->setLinearVelocity(ToBulletVector3(x));
}

Vector3 CBulletRigidBody3D::GetLinearVelocity() const
{
	return ToVector3( Body->getLinearVelocity() );
}

void CBulletRigidBody3D::SetAngularVelocity(const Vector3& x)
{
	Activate();
	Body->setAngularVelocity(ToBulletVector3(x));
}

Vector3 CBulletRigidBody3D::GetAngularVelocity() const
{
	return ToVector3(Body->getAngularVelocity());
}

void CBulletRigidBody3D::AddForce(const Vector3& x)
{
	Activate();
	Body->applyCentralForce( ToBulletVector3(x) );
}

void CBulletRigidBody3D::AddForceAtPosition(const Vector3& x, const Vector3& p)
{
	Activate();
	Body->applyForce(ToBulletVector3(x), ToBulletVector3(p) );
}

void CBulletRigidBody3D::AddTorque(const Vector3& x)
{
	Activate();
	Body->applyTorque(ToBulletVector3(x));
}

void CBulletRigidBody3D::SetFriction(const float x)
{
	Body->setFriction(x);
}

float CBulletRigidBody3D::GetFriction() const
{
	return Body->getFriction();
}

void CBulletRigidBody3D::SetRollingFriction(const float x)
{
	Body->setRollingFriction(x);
}

float CBulletRigidBody3D::GetRollingFriction() const
{
	return Body->getRollingFriction();
}

void CBulletRigidBody3D::SetRestitution(const float x)
{
	Body->setRestitution(x);
}

float CBulletRigidBody3D::GetRestitution() const
{
	return Body->getRestitution();
}

void CBulletRigidBody3D::SetLinearFactor(const Vector3& x)
{
	Body->setLinearFactor( ToBulletVector3(x.Normalized()) );
}

Vector3 CBulletRigidBody3D::GetLinearFactor() const
{
	return ToVector3(Body->getLinearFactor() );
}

void CBulletRigidBody3D::SetAngularFactor(const Vector3& x)
{
	Body->setAngularFactor(ToBulletVector3(x.Normalized()));
}

Vector3 CBulletRigidBody3D::GetAngularFactor() const
{
	return ToVector3( Body->getAngularFactor() );
}

void CBulletRigidBody3D::SetLinearDamping(const float x)
{
	Body->setDamping(x, GetAngularDamping());
}

float CBulletRigidBody3D::GetLinearDamping() const
{
	return Body->getLinearDamping();
}

void CBulletRigidBody3D::SetAngularDamping(const float x)
{
	Body->setDamping(GetLinearDamping(), x);
}

float CBulletRigidBody3D::GetAngularDamping() const
{
	return Body->getAngularDamping();
}

void CBulletRigidBody3D::SetSleepThreshold(const Vector2& t)
{
	Body->setSleepingThresholds(t.x, t.y);
}

Vector2 CBulletRigidBody3D::GetSleepThreshold() const
{
	return { Body->getLinearSleepingThreshold(), Body->getAngularSleepingThreshold() };
}

void CBulletRigidBody3D::SetBodyType(const ERigidBodyType3D Type)
{
	RemoveRigidBodyFromWorld();
	if (Type == ERigidBodyType3D::Dynamic)
	{
		SetMass( 1.0f ); // Save Actual Dynamic Mass ?
		Body->setCollisionFlags(DefaultCollisionFlags);
		Body->setActivationState(DefaultActivationState);
	}
	else if (Type == ERigidBodyType3D::Static)
	{
		SetMass(0.0f);
		Body->setCollisionFlags(DefaultCollisionFlags);
		Body->setActivationState(DefaultActivationState);
	}
	else if (Type == ERigidBodyType3D::Kinematic)
	{
		SetMass(0.0f);
		Body->setCollisionFlags(DefaultCollisionFlags | btCollisionObject::CF_KINEMATIC_OBJECT);
		Body->setActivationState(DISABLE_DEACTIVATION);
	}
	BodyType = Type;
	AddRigidBodyToWorld();
}

void CBulletRigidBody3D::AddRigidBodyToWorld()
{
	if (!AddedToWorld)
	{
		if (CollisionLayer >= 0 && CollisionMask >= 0)
		{
			World->GetWorld()->addRigidBody(Body.get(), CollisionLayer, CollisionMask);
		}
		else
		{
			World->GetWorld()->addRigidBody(Body.get());
		}
		AddedToWorld = true;
	}
}

void CBulletRigidBody3D::RemoveRigidBodyFromWorld()
{
	if (AddedToWorld)
	{
		World->GetWorld()->removeRigidBody(Body.get());
		AddedToWorld = false;
	}
}

void CBulletRigidBody3D::SetEnabled(const bool x)
{
	if (x)
	{
		AddRigidBodyToWorld();
	}
	else
	{
		RemoveRigidBodyFromWorld();
	}
}

bool CBulletRigidBody3D::IsEnabled() const
{
	return AddedToWorld;
}

ERigidBodyType3D CBulletRigidBody3D::GetBodyType() const
{
	return BodyType;
}

void CBulletRigidBody3D::SetCollisionShape(ICollisionShape3D* NewShape)
{
	if (!NewShape)
	{
		return;
	}
	//
	RemoveRigidBodyFromWorld();
	//
	float Mass = GetMass();
	btVector3 LocalInertia(0, 0, 0);
	Shape = static_cast<CBulletCollisionShape3D*>(NewShape);
	Body->setCollisionShape( Shape->GetShape() );
	Shape->GetShape()->calculateLocalInertia(Mass, LocalInertia);
	Body->setMassProps(Mass, LocalInertia);
	Body->updateInertiaTensor();
	//
	AddRigidBodyToWorld();
}

ICollisionShape3D* CBulletRigidBody3D::GetCollisionShape() const
{ 
	return Shape; 
}

void CBulletRigidBody3D::Activate()
{
	if (BodyType == ERigidBodyType3D::Dynamic)
	{
		Body->activate(true);
	}
}