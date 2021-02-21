#pragma once
#include "GLM.hpp"
#include <ostream>

using Vector2 = glm::vec2;
using IntVector2 = glm::ivec2;

static constexpr Vector2 VECTOR2_UP = { 0.0f, 1.0f };
static constexpr Vector2 VECTOR2_DOWN = { 0.0f, -1.0f };
static constexpr Vector2 VECTOR2_RIGHT = { 1.0f, 0.0f };
static constexpr Vector2 VECTOR2_LEFT = { -1.0f, 0.0f };
static constexpr Vector2 VECTOR2_ZERO = { 0.0f, 0.0f };
static constexpr Vector2 VECTOR2_ONE = { 1.0f, 1.0f };

namespace Math
{
    Vector2 Normalize(const Vector2& a);
    float Distance(const Vector2& a, const Vector2& b);
    float DistanceSquared(const Vector2& a, const Vector2& b);
    float Length(const Vector2& a);
    float LengthSquared(const Vector2& a);
    float DotProduct(const Vector2& a, const Vector2& b);
    float CrossProduct(const Vector2& a, const Vector2& b);
    // Vectors Normalized, In Degrees
    float Angle(const Vector2& From, const Vector2& To);
    Vector2 MoveTowards(const Vector2& Now, const Vector2& Target, const float Delta);
    Vector2 Lerp(const Vector2& a, const Vector2& b, const float t);
    Vector2 Max(const Vector2& a, const Vector2& b);
    Vector2 Min(const Vector2& a, const Vector2& b);
    //
    const float* ToArray(const Vector2& a);
}

std::ostream& operator<<(std::ostream& Stream, const Vector2& Other);
std::ostream& operator<<(std::ostream& Stream, const IntVector2& Other);