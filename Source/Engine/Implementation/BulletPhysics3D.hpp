#pragma once
#include "../Physics/Physics3D.hpp"

class CBulletPhysics3D final: public IPhysics3D
{
public:
    CBulletPhysics3D();
    ~CBulletPhysics3D();

    std::unique_ptr<IPhysicsWorld3D> CreateWorld(const Vector3&) override;
};
