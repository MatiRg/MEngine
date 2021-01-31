#pragma once
#include "../Core/NonCopyable.hpp"
#include "../Math/Vector3.hpp"

class CRenderer3D;
class ICollisionShape3D;
class IRigidBody3D;

class IPhysicsWorld3D: public NonCopyableMovable
{
public:
    IPhysicsWorld3D();
    virtual ~IPhysicsWorld3D();

    virtual void OnUpdate(const float) = 0;

    virtual void SetGravity(const Vector3&) = 0;
    virtual Vector3 GetGravity() const = 0;

    //! Create Form Half Extents
    virtual ICollisionShape3D* CreateBoxShape(const Vector3&) = 0;
    virtual void DestroyShape(ICollisionShape3D*) = 0;

    virtual IRigidBody3D* CreateBody(ICollisionShape3D*) = 0;
    virtual void DestroyBody(IRigidBody3D*) = 0;

    virtual void DebugDraw(CRenderer3D*) = 0;
};
