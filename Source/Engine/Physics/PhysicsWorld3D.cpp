#include "PhysicsWorld3D.hpp"
#include "../System//MemoryManager.hpp"

IPhysicsWorld3D::IPhysicsWorld3D()
{
	ADD_MEMORY_RECORD(this);
}

IPhysicsWorld3D::~IPhysicsWorld3D()
{
	ERASE_MEMORY_RECORD(this);
}
