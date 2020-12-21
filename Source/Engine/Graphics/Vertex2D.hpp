#pragma once
#include "../Math/Vector2.hpp"
#include "../Math/Color.hpp"

struct SVertex2D
{
    SVertex2D(const Vector2& aPosition, const Color& aDrawColor):
        Position(aPosition),
        DrawColor(aDrawColor)
    {
    }

    Vector2 Position; // [x, y, Layer]
    Color DrawColor;
};