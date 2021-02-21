#pragma once
#include "GLM.hpp"
#include "Vector2.hpp"
#include "Vector3.hpp"
#include "Matrix4.hpp"

using Quaternion = glm::quat;

static constexpr Quaternion QUATERNION_IDENTITY = Quaternion(1.0f, 0.0f, 0.0f, 0.0f);

namespace Math
{
    Quaternion Conjugate(const Quaternion& Qt);
    float Length(const Quaternion& Qt);
    float LengthSquared(const Quaternion& Qt);
    Quaternion Normalize(const Quaternion& Qt);
    float DotProduct(const Quaternion& v1, const Quaternion& v2);
    Quaternion Inverse(const Quaternion& Qt);

    // Converts Quaternion to Euler Angles in Degrees
    // Pitch Yaw Roll
    Vector3 ToEulerAngles(const Quaternion& Qt);
    // Pitch Yaw Roll
    Quaternion FromEulerAngles(const float Pitch, const float Yaw, const float Roll);
    // Pitch Yaw Roll
    Quaternion FromEulerAngles(const Vector3& Angles);
    // Degrees, Normalized Axis
    Quaternion FromAxisAngle(const Vector3& Axis, const float Angle);
    // From To Rotation, Vectors Normalized
    Quaternion FromToRotation(const Vector3& a, const Vector3& b);
    // Axis Normalized
    Quaternion LookRotation(const Vector3& Direction, const Vector3& Up);

    // In Degrees
    float Angle(const Quaternion& a, const Quaternion& b);
    Quaternion RotateTowards(const Quaternion& From, const Quaternion& To, float Delta);

    // Normalized
    Quaternion Lerp(const Quaternion& a, const Quaternion& b, const float t);
    // Normalized
    Quaternion SLerp(const Quaternion& a, const Quaternion& b, const float t);

    // Rotation From Quaternion
    Matrix4 ToMatrix4(const Quaternion& Qt);

    Matrix4 Transform(const Vector3& Position, const Quaternion& Rotation, const Vector3& Scale);
    
    // In Degrees
    Matrix4 Transform2D(const Vector2& Origin, const float Angle, const Vector2& Scale);
}

std::ostream& operator<<(std::ostream& Stream, const Quaternion& Other);
