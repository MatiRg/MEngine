#pragma once
#include "../Core/NonCopyable.hpp"
#include "../Math/Vector3.hpp"

class CRenderer3D;

class IPhysicsWorld3D: public NonCopyableMovable
{
public:
    IPhysicsWorld3D();
    virtual ~IPhysicsWorld3D();

    virtual void OnUpdate(const float) = 0;

    virtual void SetGravity(const Vector3&) = 0;
    virtual const Vector3& GetGravity() const = 0;

    virtual void DebugDraw(CRenderer3D*) = 0;
};
