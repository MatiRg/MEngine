#pragma once
#include "GLM.hpp"
#include "Vector3.hpp"

using Matrix4 = glm::mat4;

static constexpr Matrix4 MATRIX4_IDENTITY = {};

namespace Math
{
    Matrix4 Transpose(const Matrix4& Mat);
    Matrix4 Inverse(const Matrix4& Mat);

    // In Degrees 
    Matrix4 RotationX(const float Angle);
    // In Degrees
    Matrix4 RotationY(const float Angle);
    // In Degrees
    Matrix4 RotationZ(const float Angle);
    // Angle Axis - Degrees, Normalized Axis
    Matrix4 Rotation(const Vector3& Axis, const float Angle);

    Matrix4 Scale(const Vector3& Scale);
    Matrix4 Scale(const float x, const float y, const float z);
    Matrix4 Scale(const float x);

    Matrix4 Translation(const Vector3& Translation);
    Matrix4 Translation(const float x, const float y, const float z);

    // World Space Arguments
    Matrix4 LookAt(const Vector3& Camera, const Vector3& Position, const Vector3& Up);
    // In Degrees
    Matrix4 Perspective(const float Fov, const float Aspect, const float Near, const float Far);
    Matrix4 Ortho(const float Right, const float Left, const float Top, const float Bottom, const float Far, const float Near);

    const float* ToArray(const Matrix4& a);
    Matrix4 MakeMatrix4(const float* a);
}

std::ostream& operator<<(std::ostream& Stream, const Matrix4& Other);