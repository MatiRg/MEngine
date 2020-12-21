#include "CollisionShape2D.hpp"
#include "../Core/Log.hpp"
#include "../System/MemoryManager.hpp"

ICollisionShape2D::ICollisionShape2D()
{
    LOG(ESeverity::Debug) << "Collision Shape 2D - Created\n";
    ADD_MEMORY_RECORD(this);
}

ICollisionShape2D::~ICollisionShape2D()
{
    LOG(ESeverity::Debug) << "Collision Shape 2D - Destroyed\n";
    ERASE_MEMORY_RECORD(this);
}