#pragma once
#include "../Math/Vector3.hpp"
#include <vector>

class IRigidBody3D;

struct SContactPoint3D
{
    SContactPoint3D(const Vector3& aNormal, const Vector3& aPosition):
        Normal(aNormal),
        Position(aPosition)
    {
    }

    Vector3 Normal;
    Vector3 Position;
};

struct SContact3D
{
    SContact3D(IRigidBody3D* aBodyA, IRigidBody3D* aBodyB):
        BodyA(aBodyA),
        BodyB(aBodyB)
    {
    }

    IRigidBody3D* BodyA = nullptr;
    IRigidBody3D* BodyB = nullptr;
    std::vector<SContactPoint3D> ContactPoints;
};