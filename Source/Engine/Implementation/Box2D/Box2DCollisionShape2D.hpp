#pragma once
#include "../../Physics/CollisionShape2D.hpp"
#include <box2d/box2d.h>

class CBox2DRigidBody2D;

class CBox2DCollisionShape2D final: public ICollisionShape2D
{
public:
    CBox2DCollisionShape2D();
    ~CBox2DCollisionShape2D();

    IRigidBody2D* GetRigidBody() const override;

    void CreateBox(const Vector2&, const Vector2&) override;

    void SetFriction(const float) override;
    float GetFriction() const override { return Friction; }

    void SetRestitution(const float) override;
    float GetRestitution() const override { return Restitution; }

    void SetDensity(const float) override;
    float GetDensity() const override { return Density; }

    void SetSize(const Vector2&) override;
    const Vector2& GetSize() const override { return Size; }

    void SetOffset(const Vector2&) override;
    const Vector2& GetOffset() const override { return Offset; }

    void SetSensor(const bool) override;
    bool GetSensor() const override { return Sensor; }

    // Box2D setup
    void SetRigidBody(CBox2DRigidBody2D*);
private:
    void DestroyFixture();
    void CreateFixture();
private:
    CBox2DRigidBody2D* Rigidbody = nullptr;
    Vector2 Size = VECTOR2_ONE;
    b2Fixture* Fixture = nullptr;
	float Friction = 0.2f;
	float Restitution = 0.0f;
	float Density = 1.0f;
    Vector2 Offset = VECTOR2_ZERO;
    bool Sensor = false;
};