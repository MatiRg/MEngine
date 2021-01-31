#include "BulletCollisionShape3D.hpp"
#include "BulletUtils.hpp"

CBulletCollisionShape3D::CBulletCollisionShape3D(const Vector3& Size)
{
	Shape = std::make_unique<btBoxShape>(ToBulletVector3(Size));
}

CBulletCollisionShape3D::~CBulletCollisionShape3D()
{
	Shape.reset();
}
