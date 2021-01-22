#pragma once
#include "../../Physics/Physics2D.hpp"
#include <memory>
#include <box2d/box2d.h>

class CBox2DPhysics2D final: public IPhysics2D
{
public:
    CBox2DPhysics2D();
    ~CBox2DPhysics2D();

    std::unique_ptr<IPhysicsWorld2D> CreateWorld(const Vector2&) override;
};
