#pragma once
#include "../../Physics/RigidBody3D.hpp"
#include <Bullet/btBulletDynamicsCommon.h>
#include <memory>

class CBulletPhysicsWorld3D;
class CBulletCollisionShape3D;

class CBulletRigidBody3D final: public IRigidBody3D
{
public:
    CBulletRigidBody3D(CBulletPhysicsWorld3D*, CBulletCollisionShape3D*);
    ~CBulletRigidBody3D();

    IPhysicsWorld3D* GetWorld() const override;

    void SetPosition(const Vector3&) override;
    Vector3 GetPosition() const override;

    void SetRotation(const Quaternion&) override;
    Quaternion GetRotation() const override;

    void SetGravity(const Vector3&) override;
    Vector3 GetGravity() const override;

    void SetMass(float) override;
    float GetMass() const override;

    void SetLinearVelocity(const Vector3&) override;
    Vector3 GetLinearVelocity() const override;

    void SetAngularVelocity(const Vector3&) override;
    Vector3 GetAngularVelocity() const override;

    void AddForce(const Vector3&) override;
    void AddForceAtPosition(const Vector3&, const Vector3&) override;
    void AddTorque(const Vector3&) override;

    void SetFriction(const float) override;
    float GetFriction() const override;

    void SetRollingFriction(const float) override;
    float GetRollingFriction() const override;

    void SetRestitution(const float) override;
    float GetRestitution() const override;

    void SetLinearFactor(const Vector3&) override;
    Vector3 GetLinearFactor() const override;

    void SetAngularFactor(const Vector3&) override;
    Vector3 GetAngularFactor() const override;

    void SetLinearDamping(const float) override;
    float GetLinearDamping() const override;

    void SetAngularDamping(const float) override;
    float GetAngularDamping() const override;

    void SetBodyType(const ERigidBodyType3D) override;
    ERigidBodyType3D GetBodyType() const override;

    btRigidBody* GetBody() const { return Body.get(); }
private:
    void AddRigidBodyToWorld();
    void RemoveRigidBodyFromWorld();
private:
    CBulletPhysicsWorld3D* World = nullptr;
    CBulletCollisionShape3D* Shape = nullptr;
    std::unique_ptr<btDefaultMotionState> MotionState;
    std::unique_ptr<btRigidBody> Body;
    ERigidBodyType3D BodyType = ERigidBodyType3D::Dynamic;
    int DefaultCollisionFlags = 0;
    int DefaultActivationState = 0;
    bool AddedToWorld = false;
};
