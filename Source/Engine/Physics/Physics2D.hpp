#pragma once
#include "../Engine/EngineModule.hpp"
#include "../Math/Vector2.hpp"
#include <memory>

class IPhysicsWorld2D;

class IPhysics2D: public IEngineModule
{
public:
    IPhysics2D();
    virtual ~IPhysics2D();

    ENGINE_MODULE(IPhysics2D)

    // Gravity
    virtual std::unique_ptr<IPhysicsWorld2D> CreateWorld(const Vector2&) = 0;
};
