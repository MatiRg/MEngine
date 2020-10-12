#include "Physics2D.hpp"
#include "PhysicsWorld2D.hpp"
#include "Box2DUtils.hpp"
#include "../Scene/Transform2D.hpp"
#include "../Graphics/Renderer2D.hpp"

CPhysics2D::CPhysics2D(CRenderer2D* Renderer2D): 
    IEngineModule( "Physics2D" ),
    DebugDrawer( std::make_unique<CBox2DDebugDraw>(Renderer2D) )
{
}

CPhysics2D::~CPhysics2D()
{
    DebugDrawer.reset();
}

void CPhysics2D::PhysicsUpdate(const float TimeStep)
{
    if( HasWorld() )
    {
        int32 VelocityIterations = 8;   //how strongly to correct velocity
        int32 PositionIterations = 3;   //how strongly to correct position
        World->GetWorld().Step( TimeStep, VelocityIterations, PositionIterations );
    }
}

//

void CBox2DDebugDraw::DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
{
    std::vector<SVertexPC> Vertexes;
    Vector2 p1 = ToVector2(vertices[vertexCount - 1]);
	for(int32 i = 0; i < vertexCount; ++i)
	{
        Vector2 p2 = ToVector2(vertices[i]);
		Vertexes.push_back( {p1, ToColor(color)} );
		Vertexes.push_back( {p2, ToColor(color)} );
        p1 = p2;
	}
    Renderer2D->DrawPolygon( Vertexes, EPrimitiveMode::Lines, CTransform2D() );
}

/// Draw a solid closed polygon provided in CCW order.
void CBox2DDebugDraw::DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
{
    std::vector<SVertexPC> Vertexes;

    Color FillColor(0.5f * color.r, 0.5f * color.g, 0.5f * color.b, 0.5f);
	for(int32 i = 1; i < vertexCount - 1; ++i)
	{
        Vertexes.push_back( {ToVector2(vertices[0]), FillColor} );
        Vertexes.push_back( {ToVector2(vertices[i]), FillColor} );
        Vertexes.push_back( {ToVector2(vertices[i+1]), FillColor} );
	}
    Renderer2D->DrawPolygon( Vertexes, EPrimitiveMode::Triangles, CTransform2D() );

    Vertexes.clear();

    Vector2 p1 = ToVector2(vertices[vertexCount - 1]);
	for(int32 i = 0; i < vertexCount; ++i)
	{
        Vector2 p2 = ToVector2(vertices[i]);
		Vertexes.push_back( {p1, ToColor(color)} );
		Vertexes.push_back( {p2, ToColor(color)} );
        p1 = p2;
	}
    Renderer2D->DrawPolygon( Vertexes, EPrimitiveMode::Lines, CTransform2D() );
}

/// Draw a circle.
void CBox2DDebugDraw::DrawCircle(const b2Vec2& center, float radius, const b2Color& color)
{
    std::vector<SVertexPC> VertexesLines;
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
        VertexesLines.push_back( {ToVector2(v1), DrawColor} );
        VertexesLines.push_back( {ToVector2(v2), DrawColor} );
        r1 = r2;
        v1 = v2;
	}

    Renderer2D->DrawPolygon( VertexesLines, EPrimitiveMode::Lines, CTransform2D() );
}

/// Draw a solid circle.
void CBox2DDebugDraw::DrawSolidCircle(const b2Vec2& center, float radius, const b2Vec2& axis, const b2Color& color)
{
    std::vector<SVertexPC> VertexesTriangle;
    std::vector<SVertexPC> VertexesLines;
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
		VertexesTriangle.push_back( {ToVector2(v0), FillColor} );
        VertexesTriangle.push_back( {ToVector2(v1), FillColor} );
        VertexesTriangle.push_back( {ToVector2(v2), FillColor} );
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
        VertexesLines.push_back( {ToVector2(v1), DrawColor} );
        VertexesLines.push_back( {ToVector2(v2), DrawColor} );
        r1 = r2;
        v1 = v2;
	}

    // Draw a line fixed in the circle to animate rotation.
	b2Vec2 p = center + radius * axis;
	VertexesLines.push_back( {ToVector2(center), DrawColor} );
	VertexesLines.push_back( {ToVector2(p), DrawColor} );

    Renderer2D->DrawPolygon( VertexesTriangle, EPrimitiveMode::Triangles, CTransform2D() );
    Renderer2D->DrawPolygon( VertexesLines, EPrimitiveMode::Lines, CTransform2D() );
}

/// Draw a line segment.
void CBox2DDebugDraw::DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color)
{
    Renderer2D->DrawLine( ToVector2(p1), ToVector2(p2), ToColor(color), CTransform2D() );
}

/// Draw a transform. Choose your own length scale.
/// @param xf a transform.
void CBox2DDebugDraw::DrawTransform(const b2Transform& xf)
{
	const float AxisScale = 0.4f;
	b2Vec2 p1 = xf.p, p2;

    std::vector<SVertexPC> Vertexes;

	Vertexes.push_back( {ToVector2(p1), Color::RED()} );
	p2 = p1 + AxisScale * xf.q.GetXAxis();
	Vertexes.push_back( {ToVector2(p2), Color::RED()} );

	Vertexes.push_back( {ToVector2(p1), Color::GREEN()} );
	p2 = p1 + AxisScale * xf.q.GetYAxis();
	Vertexes.push_back( {ToVector2(p2), Color::GREEN()} );

    Renderer2D->DrawPolygon( Vertexes, EPrimitiveMode::Lines, CTransform2D() );
}

/// Draw a point.
void CBox2DDebugDraw::DrawPoint(const b2Vec2& p, float /*Size*/, const b2Color& color)
{
    Renderer2D->DrawPoint( ToVector2(p), ToColor(color), CTransform2D() );
}