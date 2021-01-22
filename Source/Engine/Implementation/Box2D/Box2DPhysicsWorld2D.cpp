#include "Box2DPhysicsWorld2D.hpp"
#include "Box2DRigidBody2D.hpp"
#include "Box2DCollisionShape2D.hpp"
#include "Box2DUtils.hpp"
#include "../../Graphics/Renderer2D.hpp"
#include "../../Core/Log.hpp"

void CBox2DPhysicsWorld2D::CContactListener::BeginContact(b2Contact* Contact)
{ 
    auto RigidBodyA = static_cast<IRigidBody2D*>( Contact->GetFixtureA()->GetBody()->GetUserData() );
    auto ColliderA = static_cast<ICollisionShape2D*>( Contact->GetFixtureA()->GetUserData() );

    auto RigidBodyB = static_cast<IRigidBody2D*>( Contact->GetFixtureB()->GetBody()->GetUserData() );
    auto ColliderB = static_cast<ICollisionShape2D*>( Contact->GetFixtureB()->GetUserData() );

    if (World->CollisionCallbackEnter)
    {
        World->CollisionCallbackEnter({ RigidBodyA, ColliderA }, { RigidBodyB, ColliderB });
    }
}

void CBox2DPhysicsWorld2D::CContactListener::EndContact(b2Contact* Contact)
{ 
    auto RigidBodyA = static_cast<IRigidBody2D*>( Contact->GetFixtureA()->GetBody()->GetUserData() );
    auto ColliderA = static_cast<ICollisionShape2D*>( Contact->GetFixtureA()->GetUserData() );

    auto RigidBodyB = static_cast<IRigidBody2D*>( Contact->GetFixtureB()->GetBody()->GetUserData() );
    auto ColliderB = static_cast<ICollisionShape2D*>( Contact->GetFixtureB()->GetUserData() );

    if (World->CollisionCallbackLeave)
    {
        World->CollisionCallbackLeave({ RigidBodyA, ColliderA }, { RigidBodyB, ColliderB });
    }
}

//

CBox2DPhysicsWorld2D::CBox2DPhysicsWorld2D():
    Gravity(0.0f, -9.71f),
    Listener(this),
    WorldBox2D( Tob2Vec2(Gravity) )
{
    WorldBox2D.SetContactListener( &Listener );
}

CBox2DPhysicsWorld2D::~CBox2DPhysicsWorld2D()
{
}

ICollisionShape2D* CBox2DPhysicsWorld2D::CreateBoxCollider(const Vector2& Size, const Vector2& Offset)
{
    CBox2DCollisionShape2D* Tmp = new CBox2DCollisionShape2D();
    Tmp->CreateBox(Size, Offset);
    return Tmp;
}

void CBox2DPhysicsWorld2D::DestroyCollider(ICollisionShape2D* aShape)
{
    if (aShape)
    {
        delete aShape;
    }
}

IRigidBody2D* CBox2DPhysicsWorld2D::CreateRigidBody(ICollisionShape2D* Shape)
{
    if (Shape)
    {
        return new CBox2DRigidBody2D(this, static_cast<CBox2DCollisionShape2D*>(Shape) );
    }
    return nullptr;
}

void CBox2DPhysicsWorld2D::DestroyRigidBody(IRigidBody2D* aBody)
{
    if (aBody)
    {
        delete aBody;
    }
}

void CBox2DPhysicsWorld2D::SetGravity(const Vector2& aGravity)
{
    Gravity = aGravity;
    WorldBox2D.SetGravity( Tob2Vec2(Gravity) );
}

void CBox2DPhysicsWorld2D::OnUpdate(const float TimeStep)
{
    int32 VelocityIterations = 8;   //how strongly to correct velocity
    int32 PositionIterations = 3;   //how strongly to correct position
    WorldBox2D.Step(TimeStep, VelocityIterations, PositionIterations);
}

void CBox2DPhysicsWorld2D::DebugDraw(CRenderer2D* Renderer)
{
    if (!SetDebugDrawQuerry)
    {
        DebugDrawer = std::make_unique<CBox2DDebugDraw>(Renderer);
        WorldBox2D.SetDebugDraw(DebugDrawer.get());
        SetDebugDrawQuerry = true;
    }
    WorldBox2D.DebugDraw();
}

//

void CBox2DDebugDraw::DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
{
    std::vector<SVertex2D> Vertexes;
    Vector2 p1 = ToVector2(vertices[vertexCount - 1]);
    for (int32 i = 0; i < vertexCount; ++i)
    {
        Vector2 p2 = ToVector2(vertices[i]);
        Vertexes.push_back({ p1, ToColor(color) });
        Vertexes.push_back({ p2, ToColor(color) });
        p1 = p2;
    }
    Renderer2D->DrawPolygon(Vertexes, EPrimitiveMode::Lines, Matrix4(), 0.0f);
}

/// Draw a solid closed polygon provided in CCW order.
void CBox2DDebugDraw::DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
{
    std::vector<SVertex2D> Vertexes;

    Color FillColor(0.5f * color.r, 0.5f * color.g, 0.5f * color.b, 0.5f);
    for (int32 i = 1; i < vertexCount - 1; ++i)
    {
        Vertexes.push_back({ ToVector2(vertices[0]), FillColor });
        Vertexes.push_back({ ToVector2(vertices[i]), FillColor });
        Vertexes.push_back({ ToVector2(vertices[i + 1]), FillColor });
    }
    Renderer2D->DrawPolygon(Vertexes, EPrimitiveMode::Triangles, Matrix4(), 0.0f);

    Vertexes.clear();

    Vector2 p1 = ToVector2(vertices[vertexCount - 1]);
    for (int32 i = 0; i < vertexCount; ++i)
    {
        Vector2 p2 = ToVector2(vertices[i]);
        Vertexes.push_back({ p1, ToColor(color) });
        Vertexes.push_back({ p2, ToColor(color) });
        p1 = p2;
    }
    Renderer2D->DrawPolygon(Vertexes, EPrimitiveMode::Lines, Matrix4(), 0.0f);
}

/// Draw a circle.
void CBox2DDebugDraw::DrawCircle(const b2Vec2& center, float radius, const b2Color& color)
{
    std::vector<SVertex2D> VertexesLines;
    Color DrawColor = ToColor(color);

    const float k_segments = 16.0f;
    const float k_increment = 2.0f * Math::PI<float> / k_segments;
    float sinInc = Math::Sin(k_increment);
    float cosInc = Math::Cos(k_increment);
    b2Vec2 r1(1.0f, 0.0f);
    b2Vec2 v1 = center + radius * r1;
    for (int32 i = 0; i < k_segments; ++i)
    {
        // Perform rotation to avoid additional trigonometry.
        b2Vec2 r2;
        r2.x = cosInc * r1.x - sinInc * r1.y;
        r2.y = sinInc * r1.x + cosInc * r1.y;
        b2Vec2 v2 = center + radius * r2;
        VertexesLines.push_back({ ToVector2(v1), DrawColor });
        VertexesLines.push_back({ ToVector2(v2), DrawColor });
        r1 = r2;
        v1 = v2;
    }

    Renderer2D->DrawPolygon(VertexesLines, EPrimitiveMode::Lines, Matrix4(), 0.0f);
}

/// Draw a solid circle.
void CBox2DDebugDraw::DrawSolidCircle(const b2Vec2& center, float radius, const b2Vec2& axis, const b2Color& color)
{
    std::vector<SVertex2D> VertexesTriangle;
    std::vector<SVertex2D> VertexesLines;
    Color FillColor(0.5f * color.r, 0.5f * color.g, 0.5f * color.b, 0.5f);
    Color DrawColor = ToColor(color);

    const float k_segments = 16.0f;
    const float k_increment = 2.0f * Math::PI<float> / k_segments;
    float sinInc = Math::Sin(k_increment);
    float cosInc = Math::Cos(k_increment);
    b2Vec2 v0 = center;
    b2Vec2 r1(cosInc, sinInc);
    b2Vec2 v1 = center + radius * r1;
    for (int32 i = 0; i < k_segments; ++i)
    {
        // Perform rotation to avoid additional trigonometry.
        b2Vec2 r2;
        r2.x = cosInc * r1.x - sinInc * r1.y;
        r2.y = sinInc * r1.x + cosInc * r1.y;
        b2Vec2 v2 = center + radius * r2;
        VertexesTriangle.push_back({ ToVector2(v0), FillColor });
        VertexesTriangle.push_back({ ToVector2(v1), FillColor });
        VertexesTriangle.push_back({ ToVector2(v2), FillColor });
        r1 = r2;
        v1 = v2;
    }

    r1.Set(1.0f, 0.0f);
    v1 = center + radius * r1;
    for (int32 i = 0; i < k_segments; ++i)
    {
        b2Vec2 r2;
        r2.x = cosInc * r1.x - sinInc * r1.y;
        r2.y = sinInc * r1.x + cosInc * r1.y;
        b2Vec2 v2 = center + radius * r2;
        VertexesLines.push_back({ ToVector2(v1), DrawColor });
        VertexesLines.push_back({ ToVector2(v2), DrawColor });
        r1 = r2;
        v1 = v2;
    }

    // Draw a line fixed in the circle to animate rotation.
    b2Vec2 p = center + radius * axis;
    VertexesLines.push_back({ ToVector2(center), DrawColor });
    VertexesLines.push_back({ ToVector2(p), DrawColor });

    Renderer2D->DrawPolygon(VertexesTriangle, EPrimitiveMode::Triangles, Matrix4(), 0.0f);
    Renderer2D->DrawPolygon(VertexesLines, EPrimitiveMode::Lines, Matrix4(), 0.0f);
}

/// Draw a line segment.
void CBox2DDebugDraw::DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color)
{
    Renderer2D->DrawLine(ToVector2(p1), ToVector2(p2), ToColor(color), Matrix4(), 0.0f);
}

/// Draw a transform. Choose your own length scale.
/// @param xf a transform.
void CBox2DDebugDraw::DrawTransform(const b2Transform& xf)
{
    const float AxisScale = 0.4f;
    b2Vec2 p1 = xf.p, p2;

    std::vector<SVertex2D> Vertexes;

    Vertexes.push_back({ ToVector2(p1), Color::RED });
    p2 = p1 + AxisScale * xf.q.GetXAxis();
    Vertexes.push_back({ ToVector2(p2), Color::RED });

    Vertexes.push_back({ ToVector2(p1), Color::GREEN });
    p2 = p1 + AxisScale * xf.q.GetYAxis();
    Vertexes.push_back({ ToVector2(p2), Color::GREEN });

    Renderer2D->DrawPolygon(Vertexes, EPrimitiveMode::Lines, Matrix4(), 0.0f);
}

/// Draw a point.
void CBox2DDebugDraw::DrawPoint(const b2Vec2& p, float /*Size*/, const b2Color& color)
{
    Renderer2D->DrawPoint(ToVector2(p), ToColor(color), Matrix4(), 0.0f);
}
