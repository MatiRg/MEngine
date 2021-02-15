#include "BulletPhysicsWorld3D.hpp"
#include "BulletCollisionShape3D.hpp"
#include "BulletRigidBody3D.hpp"
#include "BulletUtils.hpp"
#include "../../Graphics/DebugDrawer.hpp"
#include <algorithm>
#include <Bullet/LinearMath/btIDebugDraw.h>

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
	World->getDispatchInfo().m_useContinuous = true;
	World->getSolverInfo().m_splitImpulse = false; // Performance Fix ?
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
	//
	BulletDebugDrawer.reset();
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
		// Remove from Collision Callbacks
		for (auto It = PreviousCollisions.begin(); It != PreviousCollisions.end();)
		{
			const auto& Key = It->first;
			if (Key.first == BulletBody || Key.second == BulletBody)
			{
				It = PreviousCollisions.erase(It);
			}
			else
			{
				++It;
			}
		}
	}
}

void CBulletPhysicsWorld3D::OnUpdate(const float DT)
{
	World->stepSimulation(DT, 10);
	// Collisions
	Collisions.clear();
	int NumManifolds = Dispatcher->getNumManifolds();
	for (int i = 0; i < NumManifolds; ++i)
	{
		btPersistentManifold* Manifold = Dispatcher->getManifoldByIndexInternal(i);
		// Could Be None
		if (!Manifold->getNumContacts())
		{
			continue;
		}
		//
		auto CBa = Manifold->getBody0();
		auto CBb = Manifold->getBody1();
		CBulletRigidBody3D* BodyA = static_cast<CBulletRigidBody3D*>(CBa->getUserPointer());
		CBulletRigidBody3D* BodyB = static_cast<CBulletRigidBody3D*>(CBb->getUserPointer());
		// Skip static object they are static after all
		if (BodyA->GetBodyType() == ERigidBodyType3D::Static && BodyB->GetBodyType() == ERigidBodyType3D::Static)
		{
			continue;
		}
		// Proper Order For Hash and Detection
		//if (BodyA <= BodyB)
		//{
		Collisions[{BodyA, BodyB}] = Manifold;
		//}
		//else
		//{
		//	Collisions[{BodyB, BodyA}] = Manifold;
		//}
	}
	//
	for (const auto& i : Collisions)
	{
		CBulletRigidBody3D* BodyA = i.first.first;
		CBulletRigidBody3D* BodyB = i.first.second;
		btPersistentManifold* Manifold = i.second;
		SContact3D Contact = MakeContactFromManifold(BodyA, BodyB, Manifold);
		//
		auto It = PreviousCollisions.find(i.first);
		if( It != PreviousCollisions.end() )
		{
			// OnStay
			if (ContactCallback)
			{
				ContactCallback->OnCollisionStay(Contact);
			}
			PreviousCollisions.erase(It);
		}
		else
		{
			// OnEnter
			if (ContactCallback)
			{
				ContactCallback->OnCollisionEnter(Contact);
			}
		}
	}
	for (const auto& i : PreviousCollisions)
	{
		CBulletRigidBody3D* BodyA = i.first.first;
		CBulletRigidBody3D* BodyB = i.first.second;
		btPersistentManifold* Manifold = i.second;
		SContact3D Contact = MakeContactFromManifold(BodyA, BodyB, Manifold);
		// OnLeave
		if (ContactCallback)
		{
			ContactCallback->OnCollisionLeave(Contact);
		}
	}
	//
	PreviousCollisions = Collisions;
}

SContact3D CBulletPhysicsWorld3D::MakeContactFromManifold(CBulletRigidBody3D* BodyA, CBulletRigidBody3D* BodyB, btPersistentManifold* Manifold) const
{
	SContact3D Contact(BodyA, BodyB);
	int NumContacts = Manifold->getNumContacts();
	for (int i = 0; i < NumContacts; ++i)
	{
		const auto& Point = Manifold->getContactPoint(i);
		//
		Contact.ContactPoints.push_back({ ToVector3(Point.m_normalWorldOnB), ToVector3(Point.m_positionWorldOnB) });
	}
	return Contact;
}

class CBulletDebugDrawer : public btIDebugDraw
{
public:
	CBulletDebugDrawer(CDebugDrawer* aDrawer):
		Drawer(aDrawer),
		Mode(btIDebugDraw::DBG_DrawWireframe)
	{
	}
	~CBulletDebugDrawer() = default;

	void drawLine(const btVector3& from, const btVector3& to, const btVector3& color) override
	{
		Drawer->AddLine(ToVector3(from), ToVector3(to), ToColor(color));
	}

	void drawContactPoint(const btVector3&, const btVector3&, btScalar, int, const btVector3&) override {}
	void reportErrorWarning(const char*) override {}
	void draw3dText(const btVector3&, const char*) override {}

	void setDebugMode(int debugMode) { Mode = debugMode; }
	int getDebugMode() const { return Mode; }
private:
	CDebugDrawer* Drawer = nullptr;
	int Mode = 0;
};

void CBulletPhysicsWorld3D::DebugDraw(CDebugDrawer* DebugDrawer)
{
	if (!BulletDebugDrawer)
	{
		BulletDebugDrawer = std::make_unique<CBulletDebugDrawer>(DebugDrawer);
		World->setDebugDrawer(BulletDebugDrawer.get());
	}
	World->debugDrawWorld();
}
