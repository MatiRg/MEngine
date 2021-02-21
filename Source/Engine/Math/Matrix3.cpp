#include "Matrix3.hpp"

namespace Math
{
    Matrix3 Transpose(const Matrix3& Mat)
    {
        return glm::transpose(Mat);
    }

    Matrix3 Inverse(const Matrix3& Mat)
    {
        return glm::inverse(Mat);
    }

    Matrix3 Lerp(const Matrix3& a, const Matrix3& b, const float t)
    {
        return glm::mix(a, b, t);
    }

    const float* ToArray(const Matrix3& a)
    {
        return glm::value_ptr(a);
    }

    Matrix3 MakeMatrix3(const float* a)
    {
        return glm::make_mat3(a);
    }
}

std::ostream& operator<<(std::ostream& Stream, const Matrix3& Other)
{
    const float* Data = glm::value_ptr(Other);
    Stream << Data[0] << " " << Data[1] << " " << Data[2] << "\n";
    Stream << Data[3] << " " << Data[4] << " " << Data[5] << "\n";
    Stream << Data[6] << " " << Data[7] << " " << Data[8];
    return Stream;
}
