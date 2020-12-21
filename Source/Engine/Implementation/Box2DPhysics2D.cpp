#include "Box2DPhysics2D.hpp"
#include "Box2DPhysicsWorld2D.hpp"

CBox2DPhysics2D::CBox2DPhysics2D()
{
}

CBox2DPhysics2D::~CBox2DPhysics2D()
{
}

std::unique_ptr<IPhysicsWorld2D> CBox2DPhysics2D::CreateWorld(const Vector2& Gravity)
{
	auto Tmp = std::make_unique<CBox2DPhysicsWorld2D>();
	Tmp->SetGravity(Gravity);
	return Tmp;
}