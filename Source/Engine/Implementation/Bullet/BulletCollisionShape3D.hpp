#pragma once
#include "../../Physics/CollisionShape3D.hpp"

class CBulletCollisionShape3D final: public ICollisionShape3D
{
public:
    CBulletCollisionShape3D();
    ~CBulletCollisionShape3D();
};
