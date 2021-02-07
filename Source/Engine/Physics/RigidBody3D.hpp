#pragma once
#include "../Core/NonCopyable.hpp"
#include "../Math/Vector3.hpp"
#include "../Math/Quaternion.hpp"

class IPhysicsWorld3D;

enum class ERigidBodyType3D
{
    Static,
    Dynamic,
    Kinematic
};

class IRigidBody3D: public NonCopyableMovable
{
public:
    IRigidBody3D();
    virtual ~IRigidBody3D();

    virtual IPhysicsWorld3D* GetWorld() const = 0;

    virtual void SetPosition(const Vector3&) = 0;
    virtual Vector3 GetPosition() const = 0;

    virtual void SetRotation(const Quaternion&) = 0;
    virtual Quaternion GetRotation() const = 0;

    virtual void SetGravity(const Vector3&) = 0;
    virtual Vector3 GetGravity() const = 0;

    virtual void SetMass(float) = 0;
    virtual float GetMass() const = 0;

    virtual void SetLinearVelocity(const Vector3&) = 0;
    virtual Vector3 GetLinearVelocity() const = 0;

    virtual void SetAngularVelocity(const Vector3&) = 0;
    virtual Vector3 GetAngularVelocity() const = 0;

    virtual void AddForce(const Vector3&) = 0;
    virtual void AddForceAtPosition(const Vector3&, const Vector3&) = 0;
    virtual void AddTorque(const Vector3&) = 0;

    virtual void SetFriction(const float) = 0;
    virtual float GetFriction() const = 0;

    virtual void SetRollingFriction(const float) = 0;
    virtual float GetRollingFriction() const = 0;

    virtual void SetRestitution(const float) = 0;
    virtual float GetRestitution() const = 0;

    virtual void SetLinearFactor(const Vector3&) = 0;
    virtual Vector3 GetLinearFactor() const = 0;

    virtual void SetAngularFactor(const Vector3&) = 0;
    virtual Vector3 GetAngularFactor() const = 0;

    virtual void SetLinearDamping(const float) = 0;
    virtual float GetLinearDamping() const = 0;

    virtual void SetAngularDamping(const float) = 0;
    virtual float GetAngularDamping() const = 0;

    virtual void SetBodyType(const ERigidBodyType3D) = 0;
    virtual ERigidBodyType3D GetBodyType() const = 0;

    void SetUserData(void* Data) { UserData = Data; }
    void* GetUserData() const { return UserData; }
    template<class T>
    T* GetUserDataAs() const { return static_cast<T*>(UserData); }
protected:
    void* UserData = nullptr;
};
