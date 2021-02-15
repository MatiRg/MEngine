#pragma once
#include "../Core/NonCopyable.hpp"
#include "../Math/Vector3.hpp"
#include "Contact3D.hpp"

class CDebugDrawer;
class ICollisionShape3D;
class IRigidBody3D;

class IContactCallback3D
{
public:
    IContactCallback3D() = default;
    virtual ~IContactCallback3D() = default;

    virtual void OnCollisionEnter(const SContact3D&) {}
    virtual void OnCollisionStay(const SContact3D&) {}
    virtual void OnCollisionLeave(const SContact3D&) {}
};

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

    virtual void DebugDraw(CDebugDrawer*) = 0;

    //! Will not Takes Ownership
    void SetContactCallback(IContactCallback3D* aContactCallback) { ContactCallback = aContactCallback; }
    IContactCallback3D* GetContactCallback() const { return ContactCallback; }
protected:
    IContactCallback3D* ContactCallback = nullptr;
};
