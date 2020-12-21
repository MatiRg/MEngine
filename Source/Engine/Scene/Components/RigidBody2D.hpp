#pragma once
#include "../Component.hpp"
#include"../../Physics/RigidBody2D.hpp"

class CPhysicsWorld2D;

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

    void SetBodyType(const ERigidBodyType2D);
    ERigidBodyType2D GetBodyType() const;

    void SetFixedRotation(const bool);
    bool HasFixedRotation() const;

    void AddForce(const Vector2&);
    void AddTorque(const float);
    void AddLinearImpulse(const Vector2&);
    void AddAngularImpulse(const float);

    void SetMass(const float);
    float GetMass() const;

    void SetLinearVelocity(const Vector2&);
    Vector2 GetLinearVelocity() const;

    // Degrees
    void SetAngularVelocity(const float);
    float GetAngularVelocity() const;

    void SetLinearDamping(const float);
    float GetLinearDamping() const;

    void SetAngularDamping(const float);
    float GetAngularDamping() const;

    IRigidBody2D* GetBody() const { return Body; }
private:
    CPhysicsWorld2D* World = nullptr;
    IRigidBody2D* Body = nullptr;
};