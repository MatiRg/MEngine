#include "RigidBody3D.hpp"
#include "../System//MemoryManager.hpp"

IRigidBody3D::IRigidBody3D()
{
	ADD_MEMORY_RECORD(this);
}

IRigidBody3D::~IRigidBody3D()
{
	ERASE_MEMORY_RECORD(this);
}
