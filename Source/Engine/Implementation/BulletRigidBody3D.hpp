#pragma once
#include "../Physics/RigidBody3D.hpp"

class CBulletRigidBody3D final: public IRigidBody3D
{
public:
    CBulletRigidBody3D();
    ~CBulletRigidBody3D();
};
