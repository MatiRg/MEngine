#include "BulletPhysics3D.hpp"
#include "BulletPhysicsWorld3D.hpp"

CBulletPhysics3D::CBulletPhysics3D()
{
}

CBulletPhysics3D::~CBulletPhysics3D()
{
}

std::unique_ptr<IPhysicsWorld3D> CBulletPhysics3D::CreateWorld(const Vector3& Gravity)
{
	auto Tmp = std::make_unique<CBulletPhysicsWorld3D>();
	Tmp->SetGravity(Gravity);
	return Tmp;
}