#pragma once
#include "../Scene/Component.hpp"
#include "../Math/Vector2.hpp"
#include <box2d/box2d.h>

class CRigidBody2D;

class CBoxCollider2D final: public IComponent
{
public:
    CBoxCollider2D(CEngine* aEngine);
    ~CBoxCollider2D();

    COMPONENT(CBoxCollider2D)

    void OnCreate() override;

    bool OnLoad(CXMLElement*) override;
    bool OnSave(CXMLElement*) override;

    void SetFriction(const float);
    float GetFriction() const { return Friction; }

    void SetRestitution(const float);
    float GetRestitution() const { return Restitution; }

    void SetDensity(const float);
    float GetDensity() const { return Density; }

    void SetSize(const Vector2&);
    const Vector2& GetSize() const { return Size; }

    void SetOffset(const Vector2&);
    const Vector2& GetOffset() const { return Offset; }

    void SetSensor(const bool);
    bool GetSensor() const { return Sensor; }
private:
    void DestroyFixture();
    void CreateFixture();
private:
    CRigidBody2D* RigidBody = nullptr;
    Vector2 Size = Vector2::ONE();
    b2Fixture* Fixture = nullptr;
	float Friction = 0.2f;
	float Restitution = 0.0f;
	float Density = 1.0f;
    Vector2 Offset = Vector2::ZERO();
    bool Sensor = false;
};