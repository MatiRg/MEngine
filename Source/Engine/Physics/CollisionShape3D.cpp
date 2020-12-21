#include "CollisionShape3D.hpp"
#include "../System//MemoryManager.hpp"

ICollisionShape3D::ICollisionShape3D()
{
	ADD_MEMORY_RECORD(this);
}

ICollisionShape3D::~ICollisionShape3D()
{
	ERASE_MEMORY_RECORD(this);
}
