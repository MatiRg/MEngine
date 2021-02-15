#pragma once
#include "../../Physics/CollisionShape3D.hpp"
#include "../../Math/Vector3.hpp"
#include <Bullet/btBulletDynamicsCommon.h>
#include <memory>

// TO DO: Extend to Other Shapes
class CBulletCollisionShape3D final: public ICollisionShape3D
{
public:
    CBulletCollisionShape3D(const Vector3&);
    ~CBulletCollisionShape3D();

    btCollisionShape* GetShape() const { return Shape.get(); }
private:
    std::unique_ptr<btCollisionShape> Shape;
};
