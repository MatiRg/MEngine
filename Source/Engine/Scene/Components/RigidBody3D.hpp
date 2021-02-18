#pragma once
#include "../Component.hpp"
#include"../../Physics/RigidBody3D.hpp"

class CPhysicsWorld3D;

class CRigidBody3D final: public IComponent
{
public:
    CRigidBody3D(CEngine* aEngine);
    ~CRigidBody3D();

    COMPONENT(CRigidBody3D)

    void OnCreate() override;

    bool OnLoad(CXMLElement*) override;
    bool OnSave(CXMLElement*) override;

    void OnEnabled() override;
    void OnDisabled() override;

    void OnBeginFrame() override;

    //! Always Set Collision Layer And Collision Mask
    void SetCollisionLayer(int);
    int GetCollisionLayer() const;

    //! Always Set Collision Layer And Collision Mask
    void SetCollisionMask(int);
    int GetCollisionMask() const;

    void SetGravity(const Vector3&);
    Vector3 GetGravity() const;

    void SetMass(float);
    float GetMass() const;

    void SetLinearVelocity(const Vector3&);
    Vector3 GetLinearVelocity() const;

    void SetAngularVelocity(const Vector3&);
    Vector3 GetAngularVelocity() const;

    void AddForce(const Vector3&);
    void AddForceAtPosition(const Vector3&, const Vector3&);
    void AddTorque(const Vector3&);

    void SetFriction(const float);
    float GetFriction() const;

    void SetRollingFriction(const float);
    float GetRollingFriction() const;

    void SetRestitution(const float);
    float GetRestitution() const;

    void SetLinearFactor(const Vector3&);
    Vector3 GetLinearFactor() const;

    void SetAngularFactor(const Vector3&);
    Vector3 GetAngularFactor() const;

    void SetLinearDamping(const float);
    float GetLinearDamping() const;

    void SetAngularDamping(const float);
    float GetAngularDamping() const;

    void SetSleepThreshold(const Vector2&);
    Vector2 GetSleepThreshold() const;

    void SetBodyType(const ERigidBodyType3D);
    ERigidBodyType3D GetBodyType() const;

    IRigidBody3D* GetBody() const { return Body; }
private:
    CPhysicsWorld3D* PhysicsWorld3D = nullptr;
    IRigidBody3D* Body = nullptr;
};