#pragma once
#include "GLM.hpp"
#include "Matrix4.hpp"

using Matrix3 = glm::mat3;

static constexpr Matrix3 MATRIX3_IDENTITY = Matrix3(1.0f);

namespace Math
{
    Matrix3 Transpose(const Matrix3& Mat);
    Matrix3 Inverse(const Matrix3& Mat);
    Matrix3 Lerp(const Matrix3& a, const Matrix3& b, const float v);
    //
    const float* ToArray(const Matrix3& a);
    Matrix3 MakeMatrix3(const float* a);
}

std::ostream& operator<<(std::ostream& Stream, const Matrix3& Other);