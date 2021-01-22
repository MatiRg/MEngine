#pragma once
#include "Box2DCollisionShape2D.hpp"
#include "../../Physics/RigidBody2D.hpp"
#include <box2d/box2d.h>

class CBox2DPhysicsWorld2D;
class CBox2DCollisionShape2D;

class CBox2DRigidBody2D final: public IRigidBody2D
{
public:
    CBox2DRigidBody2D(CBox2DPhysicsWorld2D*, CBox2DCollisionShape2D*);
    ~CBox2DRigidBody2D();

    void SetPosition(const Vector2&) override;
    Vector2 GetPosition() const override;

    void SetAngle(const float) override;
    float GetAngle() const override;

    void SetEnabled(const bool) override;
    bool IsEnabled() const override;

    void SetBodyType(const ERigidBodyType2D) override;
    ERigidBodyType2D GetBodyType() const override;

    void SetFixedRotation(const bool) override;
    bool HasFixedRotation() const override;

    void AddForce(const Vector2&) override;
    void AddTorque(const float) override;
    void AddLinearImpulse(const Vector2&) override;
    void AddAngularImpulse(const float) override;

    void SetMass(const float) override;
    float GetMass() const override;

    void SetLinearVelocity(const Vector2&) override;
    Vector2 GetLinearVelocity() const override;

    // Degrees
    void SetAngularVelocity(const float) override;
    float GetAngularVelocity() const override;

    void SetLinearDamping(const float) override;
    float GetLinearDamping() const override;

    void SetAngularDamping(const float) override;
    float GetAngularDamping() const override;

    b2Body* GetBody() const { return Body; }
private:
    b2Body* Body = nullptr;
};