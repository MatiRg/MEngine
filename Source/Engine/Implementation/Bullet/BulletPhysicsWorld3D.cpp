#include "BulletPhysicsWorld3D.hpp"
#include "BulletCollisionShape3D.hpp"
#include "BulletRigidBody3D.hpp"
#include "BulletUtils.hpp"
#include <algorithm>

CBulletPhysicsWorld3D::CBulletPhysicsWorld3D()
{
	///collision configuration contains default setup for memory, collision setup. Advanced users can create their own configuration.
	CollisionConfiguration = std::make_unique<btDefaultCollisionConfiguration>();
	///use the default collision dispatcher. For parallel processing you can use a diffent dispatcher (see Extras/BulletMultiThreaded)
	Dispatcher = std::make_unique<btCollisionDispatcher>(CollisionConfiguration.get());
	///btDbvtBroadphase is a good general purpose broadphase. You can also try out btAxis3Sweep.
	BroadphaseInterface = std::make_unique<btDbvtBroadphase>();
	///the default constraint solver. For parallel processing you can use a different solver (see Extras/BulletMultiThreaded)
	SequentialImpulseConstraintSolver = std::make_unique<btSequentialImpulseConstraintSolver>();

	World = std::make_unique<btDiscreteDynamicsWorld>(Dispatcher.get(), BroadphaseInterface.get(), SequentialImpulseConstraintSolver.get(), CollisionConfiguration.get());
	World->setGravity({ 0.0f, -10.0f, 0.0f });
	World->setWorldUserInfo(this);
}

CBulletPhysicsWorld3D::~CBulletPhysicsWorld3D()
{
	//cleanup in the reverse order of creation/initialization
	for (const auto& i : Shapes)
	{
		delete i;
	}
	Shapes.clear();
	//
	for (const auto& i: Bodies)
	{
		World->removeRigidBody( i->GetBody() );
		delete i;
	}
	Bodies.clear();
	// World
	World.reset();
	//delete solver
	SequentialImpulseConstraintSolver.reset();
	//delete broadphase
	BroadphaseInterface.reset();
	//delete dispatcher
	Dispatcher.reset();
	// Configuration
	CollisionConfiguration.reset();
}

void CBulletPhysicsWorld3D::SetGravity(const Vector3& Gravity)
{
	World->setGravity(ToBulletVector3(Gravity));
}

Vector3 CBulletPhysicsWorld3D::GetGravity() const
{
    return ToVector3( World->getGravity() );
}

ICollisionShape3D* CBulletPhysicsWorld3D::CreateBoxShape(const Vector3& Size)
{
	CBulletCollisionShape3D* Shape = new CBulletCollisionShape3D(Size);
	Shapes.push_back(Shape);
	return Shape;
}

void CBulletPhysicsWorld3D::DestroyShape(ICollisionShape3D* Shape)
{
	CBulletCollisionShape3D* BulletShape = static_cast<CBulletCollisionShape3D*>(Shape);
	if (BulletShape)
	{
		auto Found = std::find_if(Shapes.begin(), Shapes.end(), [&](CBulletCollisionShape3D* Rhs)
		{
			return BulletShape == Rhs;
		});
		if (Found != Shapes.end())
		{
			BulletShape = *Found;
			Shapes.erase(Found);
			delete BulletShape;
		}
	}
}

IRigidBody3D* CBulletPhysicsWorld3D::CreateBody(ICollisionShape3D* Shape)
{
	CBulletRigidBody3D* Body = new CBulletRigidBody3D(this, static_cast<CBulletCollisionShape3D*>(Shape));
	Bodies.push_back(Body);
	return Body;
}

void CBulletPhysicsWorld3D::DestroyBody(IRigidBody3D* Body)
{
	CBulletRigidBody3D* BulletBody = static_cast<CBulletRigidBody3D*>(Body);
	if (BulletBody)
	{
		auto Found = std::find_if(Bodies.begin(), Bodies.end(), [&](CBulletRigidBody3D* Rhs)
		{
			return BulletBody == Rhs;
		});
		if( Found != Bodies.end() )
		{
			BulletBody = *Found;
			Bodies.erase(Found);
			delete BulletBody;
		}
	}
}

void CBulletPhysicsWorld3D::OnUpdate(const float DT)
{
	World->stepSimulation(DT, 10);
}

void CBulletPhysicsWorld3D::DebugDraw(CRenderer3D*)
{
    // TO DO
}
