#pragma once
#include "../Physics/PhysicsWorld2D.hpp"
#include <box2d/box2d.h>
#include <memory>

class CBox2DDebugDraw final : public b2Draw
{
public:
    CBox2DDebugDraw(CRenderer2D* aRenderer2D) : Renderer2D(aRenderer2D) { m_drawFlags = e_shapeBit | e_jointBit | e_pairBit; }

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


class CBox2DPhysicsWorld2D final: public IPhysicsWorld2D
{
    class CContactListener: public b2ContactListener
    {
    public:
        CContactListener(CBox2DPhysicsWorld2D* aWorld): 
            World(aWorld)
        {
        }

        void BeginContact(b2Contact*);
        void EndContact(b2Contact*);
    private:
        CBox2DPhysicsWorld2D* World;
    };
public:
    CBox2DPhysicsWorld2D();
    ~CBox2DPhysicsWorld2D();

    // Size, Offset
    ICollisionShape2D* CreateBoxCollider(const Vector2&, const Vector2&) override;
    void DestroyCollider(ICollisionShape2D*) override;

    IRigidBody2D* CreateRigidBody(ICollisionShape2D*) override;
    void DestroyRigidBody(IRigidBody2D*) override;

    void OnUpdate(const float) override;

    void SetGravity(const Vector2&) override;
    const Vector2& GetGravity() const override { return Gravity; }

    void DebugDraw(CRenderer2D*) override;

    b2World& GetWorld() { return WorldBox2D; }
private:
    Vector2 Gravity;
    CContactListener Listener;
    b2World WorldBox2D;
    bool SetDebugDrawQuerry = false;
    std::unique_ptr<CBox2DDebugDraw> DebugDrawer;
};
