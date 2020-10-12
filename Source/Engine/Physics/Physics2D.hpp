#pragma once
#include "../Engine/EngineModule.hpp"
#include <memory>
#include <box2d/box2d.h>

class CRenderer2D;
class CPhysicsWorld2D;
class CBox2DDebugDraw;

class CPhysics2D final: public IEngineModule
{
public:
    CPhysics2D(CRenderer2D*);
    ~CPhysics2D();

    ENGINE_MODULE(CPhysics2D)

	void PhysicsUpdate(const float);

    void SetWorld(CPhysicsWorld2D* aWorld) { World = aWorld; }
    CPhysicsWorld2D* GetWorld() const { return World; }
    bool HasWorld() const { return World != nullptr; }

    CBox2DDebugDraw* GetDebugDrawer() const { return DebugDrawer.get(); }
private:
    CPhysicsWorld2D* World = nullptr;
    std::unique_ptr<CBox2DDebugDraw> DebugDrawer;
};

//

class CBox2DDebugDraw final: public b2Draw
{
public:
    CBox2DDebugDraw(CRenderer2D* aRenderer2D): Renderer2D(aRenderer2D) { m_drawFlags = e_shapeBit|e_jointBit|e_pairBit; }

	void DrawPolygon(const b2Vec2*, int32, const b2Color&) override;

	/// Draw a solid closed polygon provided in CCW order.
	void DrawSolidPolygon(const b2Vec2*, int32, const b2Color&) override;

	/// Draw a circle.
	void DrawCircle(const b2Vec2&, float, const b2Color&) override;
	
	/// Draw a solid circle.
	void DrawSolidCircle(const b2Vec2&, float, const b2Vec2&, const b2Color&) override;
	
	/// Draw a line segment.
	void DrawSegment(const b2Vec2&, const b2Vec2&, const b2Color&) override;

	/// Draw a transform. Choose your own length scale.
	/// @param xf a transform.
	void DrawTransform(const b2Transform&) override;

	/// Draw a point.
	void DrawPoint(const b2Vec2&, float, const b2Color&) override;
private:
    CRenderer2D* Renderer2D = nullptr;
};