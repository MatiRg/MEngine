#pragma once
#include "../Core/NonCopyable.hpp"

class IRigidBody3D: public NonCopyableMovable
{
public:
    IRigidBody3D();
    virtual ~IRigidBody3D();
};
