#pragma once
#include "../../Physics/PhysicsWorld3D.hpp"
#include <Bullet/btBulletDynamicsCommon.h>
#include <memory>
#include <vector>

class CBulletCollisionShape3D;
class CBulletRigidBody3D;

class CBulletPhysicsWorld3D final: public IPhysicsWorld3D
{
public:
    CBulletPhysicsWorld3D();
    ~CBulletPhysicsWorld3D();

    void OnUpdate(const float) override;

    void SetGravity(const Vector3&) override;
    Vector3 GetGravity() const override;

    ICollisionShape3D* CreateBoxShape(const Vector3&) override;
    void DestroyShape(ICollisionShape3D*) override;

    IRigidBody3D* CreateBody(ICollisionShape3D*) override;
    void DestroyBody(IRigidBody3D*) override;

    void DebugDraw(CRenderer3D*) override;

    btDiscreteDynamicsWorld* GetWorld() const { return World.get(); }
private:
    std::unique_ptr<btDefaultCollisionConfiguration> CollisionConfiguration;
    std::unique_ptr<btCollisionDispatcher> Dispatcher;
    std::unique_ptr<btBroadphaseInterface> BroadphaseInterface;
    std::unique_ptr<btSequentialImpulseConstraintSolver> SequentialImpulseConstraintSolver;
    std::unique_ptr<btDiscreteDynamicsWorld> World;
    //
    std::vector<CBulletCollisionShape3D*> Shapes;
    std::vector<CBulletRigidBody3D*> Bodies;
};
