#include "RigidBody2D.hpp"
#include "../Core/Log.hpp"
#include "../System/MemoryManager.hpp"

IRigidBody2D::IRigidBody2D(IPhysicsWorld2D* aWorld, ICollisionShape2D* aShape):
    Collider(aShape),
    World(aWorld)
{
    LOG(ESeverity::Debug) << "Rigid Body 2D - Created\n";
    ADD_MEMORY_RECORD(this);
}

IRigidBody2D::~IRigidBody2D()
{
    LOG(ESeverity::Debug) << "Rigid Body 2D - Destroyed\n";
    ERASE_MEMORY_RECORD(this);
}