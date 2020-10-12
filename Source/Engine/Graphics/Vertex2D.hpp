#pragma once
#include "../Math/Vector2.hpp"
#include "../Math/Color.hpp"

struct SVertexPC
{
    SVertexPC(const Vector2& aPosition, const Color& aDrawColor):
        Position(aPosition),
        DrawColor(aDrawColor)
    {
    }

    Vector2 Position; // [x, y, Layer]
    Color DrawColor;
};