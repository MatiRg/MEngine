#pragma once
#include "../Engine/EngineModule.hpp"
#include "../Math/Vector3.hpp"
#include <memory>

class IPhysicsWorld3D;

class IPhysics3D: public IEngineModule
{
public:
    IPhysics3D();
    virtual ~IPhysics3D();

    ENGINE_MODULE(IPhysics3D)

    // Gravity
    virtual std::unique_ptr<IPhysicsWorld3D> CreateWorld(const Vector3&) = 0;
};
