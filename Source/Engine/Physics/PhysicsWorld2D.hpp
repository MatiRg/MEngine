#pragma once
#include "../Core/NonCopyable.hpp"
#include "../Math/Vector2.hpp"
#include "Contact2D.hpp"
#include <functional>

class ICollisionShape2D;
class IRigidBody2D;
class CRenderer2D;

using CollisionCallback2D = std::function<void(const SContact2D&, const SContact2D&)>;

class IPhysicsWorld2D: public NonCopyableMovable
{
public:
    IPhysicsWorld2D();
    virtual ~IPhysicsWorld2D();

    // Size, Offset
    virtual ICollisionShape2D* CreateBoxCollider(const Vector2&, const Vector2&) = 0;
    virtual void DestroyCollider(ICollisionShape2D*) = 0;

    virtual IRigidBody2D* CreateRigidBody(ICollisionShape2D*) = 0;
    virtual void DestroyRigidBody(IRigidBody2D*) = 0;

    virtual void OnUpdate(const float) = 0;

    virtual void SetGravity(const Vector2&) = 0;
    virtual const Vector2& GetGravity() const = 0;

    virtual void DebugDraw(CRenderer2D*) = 0;

    void SetEnterCollisionCallback(const CollisionCallback2D& aCall) { CollisionCallbackEnter = aCall; }
    void SetLeaveCollisionCallback(const CollisionCallback2D& aCall) { CollisionCallbackLeave = aCall; }
protected:
    CollisionCallback2D CollisionCallbackEnter;
    CollisionCallback2D CollisionCallbackLeave;
};
