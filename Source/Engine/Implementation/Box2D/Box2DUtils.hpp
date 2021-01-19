#pragma once
#include "../../Math/Vector2.hpp"
#include "../../Math/Color.hpp"
#include <box2d/box2d.h>

inline b2Vec2 Tob2Vec2(const Vector2& Val)
{
    return b2Vec2( Val.x, Val.y );
}

inline Vector2 ToVector2(const b2Vec2& Val)
{
    return Vector2( Val.x, Val.y );
}

inline Color ToColor(const b2Color& Val)
{
    return Color( Val.r, Val.g, Val.b, Val.a );
}