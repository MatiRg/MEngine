#include "PhysicsWorld2D.hpp"
#include "../Core/Log.hpp"
#include "../System/MemoryManager.hpp"

IPhysicsWorld2D::IPhysicsWorld2D()
{
    LOG(ESeverity::Debug) << "Physics World 2D - Created\n";
    ADD_MEMORY_RECORD(this);
}

IPhysicsWorld2D::~IPhysicsWorld2D()
{
    LOG(ESeverity::Debug) << "Physics World 2D - Destroyed\n";
    ERASE_MEMORY_RECORD(this);
}