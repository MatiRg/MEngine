#pragma once
#include "../Scene/Component.hpp"
#include "../Math/Vector2.hpp"
#include <box2d/box2d.h>

class CPhysicsWorld2D;

enum class ERigidBody2DType
{
    Static,
    Dynamic,
    Kinematic
};

class CRigidBody2D final: public IComponent
{
public:
    CRigidBody2D(CEngine* aEngine);
    ~CRigidBody2D();

    COMPONENT(CRigidBody2D)

    void OnCreate() override;

    bool OnLoad(CXMLElement*) override;
    bool OnSave(CXMLElement*) override;

    void OnEnabled() override;
    void OnDisabled() override;

    void OnBeginFrame() override;

    void SetBodyType(const ERigidBody2DType);
    ERigidBody2DType GetBodyType() const { return BodyType; }

    void SetFixedRotation(const bool);
    bool HasFixedRotation() const { return FixedRotation; }

    void AddForce(const Vector2&);
    void AddTorque(const float);
    void AddLinearImpulse(const Vector2&);
    void AddAngularImpulse(const float);

    float GetMass() const;

    void SetLinearVelocity(const Vector2&);
    Vector2 GetLinearVelocity() const;

    // Degrees
    void SetAngularVelocity(const float);
    float GetAngularVelocity() const;

    void SetLinearDamping(const float);
    float GetLinearDamping() const { return LinearDamping; }

    void SetAngularDamping(const float);
    float GetAngularDamping() const { return AngularDamping; }

    b2Body* GetBody() const { return Body; }
private:
    CPhysicsWorld2D* PhysicsWorld2D = nullptr;
    b2Body* Body = nullptr;
    ERigidBody2DType BodyType = ERigidBody2DType::Dynamic;
    bool FixedRotation = true;
	float LinearDamping = 0.0f;
	float AngularDamping = 0.0f;
};