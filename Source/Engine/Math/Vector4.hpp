#pragma once
#include "GLM.hpp"
#include <ostream>

using Vector4 = glm::vec4;

static constexpr Vector4 VECTOR4_ZERO = { 0.0f, 0.0f, 0.0f, 0.0f };
static constexpr Vector4 VECTOR4_ONE = { 1.0f, 1.0f, 1.0f, 1.0f };

namespace Math
{
    Vector4 Normalize(const Vector4& a);
    float Distance(const Vector4& a, const Vector4& b);
    float DistanceSquared(const Vector4& a, const Vector4& b);
    float Length(const Vector4& a);
    float LengthSquared(const Vector4& a);
    float DotProduct(const Vector4& a, const Vector4& b);
    Vector4 MoveTowards(const Vector4& Now, const Vector4& Target, const float Delta);
    Vector4 Lerp(const Vector4& a, const Vector4& b, const float t);
    Vector4 Max(const Vector4& a, const Vector4& b);
    Vector4 Min(const Vector4& a, const Vector4& b);
    //
    const float* ToArray(const Vector4& a);
}

std::ostream& operator<<(std::ostream& Stream, const Vector4& Other);