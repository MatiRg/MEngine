#pragma once
#include "GLM.hpp"
#include "Vector2.hpp"
#include <ostream>

using Vector3 = glm::vec3;
using IntVector3 = glm::ivec3;

static constexpr Vector3 VECTOR3_UP = { 0.0f, 1.0f, 0.0f };
static constexpr Vector3 VECTOR3_DOWN = { 0.0f, -1.0f, 0.0f };
static constexpr Vector3 VECTOR3_RIGHT = { 1.0f, 0.0f, 0.0f };
static constexpr Vector3 VECTOR3_LEFT = { -1.0f, 0.0f, 0.0f };
static constexpr Vector3 VECTOR3_FORWARD = { 0.0f, 0.0f, 1.0f };
static constexpr Vector3 VECTOR3_BACK = { 0.0f, 0.0f, -1.0f };
static constexpr Vector3 VECTOR3_ZERO = { 0.0f, 0.0f, 0.0f };
static constexpr Vector3 VECTOR3_ONE = { 1.0f, 1.0f, 1.0f };

namespace Math
{
    Vector3 Normalize(const Vector3& a);
    float Distance(const Vector3& a, const Vector3& b);
    float DistanceSquared(const Vector3& a, const Vector3& b);
    float Length(const Vector3& a);
    float LengthSquared(const Vector3& a);
    float DotProduct(const Vector3& a, const Vector3& b);
    Vector3 CrossProduct(const Vector3& a, const Vector3& b);
    float ScalarTripleProduct(const Vector3& a, const Vector3& b, const Vector3& c);
    Vector3 TripleProduct(const Vector3& a, const Vector3& b, const Vector3& c);
    Vector3 Reflect(const Vector3& Dir, const Vector3& Norm);
    Vector3 Project(const Vector3& Vc, const Vector3& Norm);
    // Vectors Normalized, In Degrees
    float Angle(const Vector3& From, const Vector3& To);
    // Vectors Normalized, In Degrees
    float SignedAngle(const Vector3& From, const Vector3& To, const Vector3& Axis);
    Vector3 MoveTowards(const Vector3& Now, const Vector3& Target, const float Delta);
    Vector3 Lerp(const Vector3& a, const Vector3& b, const float t);
    Vector3 Max(const Vector3& a, const Vector3& b);
    Vector3 Min(const Vector3& a, const Vector3& b);
    //
    Vector2 ToXZ(const Vector3& a);
    Vector2 ToXY(const Vector3& a);
    Vector2 ToYZ(const Vector3& a);
    //
    const float* ToArray(const Vector3& a);
}

std::ostream& operator<<(std::ostream& Stream, const Vector3& Other);
std::ostream& operator<<(std::ostream& Stream, const IntVector3& Other);