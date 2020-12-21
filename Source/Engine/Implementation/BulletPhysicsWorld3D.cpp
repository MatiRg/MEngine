#include "BulletPhysicsWorld3D.hpp"

CBulletPhysicsWorld3D::CBulletPhysicsWorld3D():
    Gravity(0.0f, -9.71f, 0.0f)
{
}

CBulletPhysicsWorld3D::~CBulletPhysicsWorld3D()
{
}

void CBulletPhysicsWorld3D::SetGravity(const Vector3& aGravity)
{
    Gravity = aGravity;
}

const Vector3& CBulletPhysicsWorld3D::GetGravity() const
{
    return Gravity;
}

void CBulletPhysicsWorld3D::OnUpdate(const float)
{
    // TO DO
}

void CBulletPhysicsWorld3D::DebugDraw(CRenderer3D*)
{
    // TO DO
}
