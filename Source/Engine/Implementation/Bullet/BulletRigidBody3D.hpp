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

    void SetBodyType(const ERigidBodyType3D) override;
    ERigidBodyType3D GetBodyType() const override;

    btRigidBody* GetBody() const { return Body.get(); }
private:
    CBulletPhysicsWorld3D* World = nullptr;
    CBulletCollisionShape3D* Shape = nullptr;
    float Mass = 1.0f;
    std::unique_ptr<btDefaultMotionState> MotionState;
    std::unique_ptr<btRigidBody> Body;
    ERigidBodyType3D BodyType = ERigidBodyType3D::Dynamic;
};
