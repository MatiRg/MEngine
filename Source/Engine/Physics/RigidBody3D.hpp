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

    virtual void SetBodyType(const ERigidBodyType3D) = 0;
    virtual ERigidBodyType3D GetBodyType() const = 0;

    void SetUserData(void* Data) { UserData = Data; }
    void* GetUserData() const { return UserData; }
    template<class T>
    T* GetUserDataAs() const { return static_cast<T*>(UserData); }
protected:
    void* UserData = nullptr;
};
