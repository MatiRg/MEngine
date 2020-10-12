#include "Component.hpp"
#include "../Engine/Engine.hpp"
#include "../System/MemoryManager.hpp"

IComponent::IComponent(CEngine* aEngine): 
    Engine(aEngine)
{
    ADD_MEMORY_RECORD(this);
}

IComponent::~IComponent()
{
    ERASE_MEMORY_RECORD(this);
}