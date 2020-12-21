#pragma once
#include "../Core/NonCopyable.hpp"
#include "../Math/Vector2.hpp"

class IPhysicsWorld2D;
class ICollisionShape2D;

enum class ERigidBodyType2D
{
    Static,
    Dynamic,
    Kinematic
};

class IRigidBody2D: public NonCopyableMovable
{
public:
    IRigidBody2D(IPhysicsWorld2D*, ICollisionShape2D*);
    virtual ~IRigidBody2D();

    IPhysicsWorld2D* GetWorld() const { return World; }
    ICollisionShape2D* GetCollider() const { return Collider; }

    virtual void SetEnabled(const bool) = 0;
    virtual bool IsEnabled() const = 0;

    virtual void SetPosition(const Vector2&) = 0;
    virtual Vector2 GetPosition() const = 0;

    virtual void SetAngle(const float) = 0;
    virtual float GetAngle() const = 0;

    virtual void SetBodyType(const ERigidBodyType2D) = 0;
    virtual ERigidBodyType2D GetBodyType() const = 0;

    virtual void SetFixedRotation(const bool) = 0;
    virtual bool HasFixedRotation() const = 0;

    virtual void AddForce(const Vector2&) = 0;
    virtual void AddTorque(const float) = 0;
    virtual void AddLinearImpulse(const Vector2&) = 0;
    virtual void AddAngularImpulse(const float) = 0;

    virtual void SetMass(const float) = 0;
    virtual float GetMass() const = 0;

    virtual void SetLinearVelocity(const Vector2&) = 0;
    virtual Vector2 GetLinearVelocity() const = 0;

    // Degrees
    virtual void SetAngularVelocity(const float) = 0;
    virtual float GetAngularVelocity() const = 0;

    virtual void SetLinearDamping(const float) = 0;
    virtual float GetLinearDamping() const = 0;

    virtual void SetAngularDamping(const float) = 0;
    virtual float GetAngularDamping() const = 0;

    void SetUserData(void* aData) { UserData = aData; }
    void* GetUserData() const { return UserData; }
    template<class T>
    T* GetUserDataAs() const { return static_cast<T*>(UserData); }
protected:
    IPhysicsWorld2D* World = nullptr;
    ICollisionShape2D* Collider = nullptr;
    void* UserData = nullptr;
};