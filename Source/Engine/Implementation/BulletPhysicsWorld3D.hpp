#pragma once
#include "../Physics/PhysicsWorld3D.hpp"

class CBulletPhysicsWorld3D final: public IPhysicsWorld3D
{
public:
    CBulletPhysicsWorld3D();
    ~CBulletPhysicsWorld3D();

    void OnUpdate(const float) override;

    void SetGravity(const Vector3&) override;
    const Vector3& GetGravity() const override;

    void DebugDraw(CRenderer3D*) override;
private:
    Vector3 Gravity;
};
