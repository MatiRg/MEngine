#pragma once
#include "../Core/NonCopyable.hpp"

class ICollisionShape3D: public NonCopyableMovable
{
public:
    ICollisionShape3D();
    virtual ~ICollisionShape3D();
};
