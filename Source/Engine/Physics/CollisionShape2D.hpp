#pragma once
#include "../Core/NonCopyable.hpp"
#include "../Math/Vector2.hpp"

class IRigidBody2D;

class ICollisionShape2D: public NonCopyableMovable
{
public:
    ICollisionShape2D();
    virtual ~ICollisionShape2D();

    virtual IRigidBody2D* GetRigidBody() const = 0;

    // Size, Offset
    virtual void CreateBox(const Vector2&, const Vector2&) = 0;

    virtual void SetFriction(const float) = 0;
    virtual float GetFriction() const = 0;

    virtual void SetRestitution(const float) = 0;
    virtual float GetRestitution() const = 0;

    virtual void SetDensity(const float) = 0;
    virtual float GetDensity() const = 0;

    virtual void SetSize(const Vector2&) = 0;
    virtual const Vector2& GetSize() const = 0;

    virtual void SetOffset(const Vector2&) = 0;
    virtual const Vector2& GetOffset() const = 0;

    virtual void SetSensor(const bool) = 0;
    virtual bool GetSensor() const = 0;

    void SetUserData(void* aData) { UserData = aData; }
    void* GetUserData() const { return UserData; }
    template<class T>
    T* GetUserDataAs() const { return static_cast<T*>(UserData); }
protected:
    void* UserData = nullptr;
};