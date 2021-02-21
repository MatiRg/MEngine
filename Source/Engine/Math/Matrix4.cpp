#include "Matrix4.hpp"

namespace Math
{
    Matrix4 Transpose(const Matrix4& Mat)
    {
        return glm::transpose(Mat);
    }

    Matrix4 Inverse(const Matrix4& Mat)
    {
        return glm::inverse(Mat);
    }

    Matrix4 RotationX(const float Angle)
    {
        return glm::eulerAngleX(glm::radians(Angle));
    }

    Matrix4 RotationY(const float Angle)
    {
        return glm::eulerAngleY(glm::radians(Angle));
    }

    Matrix4 RotationZ(const float Angle)
    {
        return glm::eulerAngleZ(glm::radians(Angle));
    }

    // Angle Axis - Degrees, Normalized Axis
    Matrix4 Rotation(const Vector3& Axis, const float Angle)
    {
        return glm::rotate(MATRIX4_IDENTITY, glm::radians(Angle), Axis);
    }

    Matrix4 Scale(const Vector3& Scale)
    {
        return glm::scale(MATRIX4_IDENTITY, Scale);
    }

    Matrix4 Scale(const float x, const float y, const float z)
    {
        return Scale({x,y,z});
    }

    Matrix4 Scale(const float x)
    {
        return Scale({x,x,x});
    }

    Matrix4 Translation(const Vector3& Translation)
    {
        return glm::translate(MATRIX4_IDENTITY, Translation);
    }

    Matrix4 Translation(const float x, const float y, const float z)
    {
        return Translation( {x, y, z} );
    }

    // World Space Arguments
    Matrix4 LookAt(const Vector3& Camera, const Vector3& Position, const Vector3& Up)
    {
        return glm::lookAt(Camera, Position, Up);
    }

    // In Degrees
    Matrix4 Perspective(const float Fov, const float Aspect, const float Near, const float Far)
    {
        return glm::perspective(glm::radians(Fov), Aspect, Near, Far);
    }

    Matrix4 Ortho(const float Right, const float Left, const float Top, const float Bottom, const float Far, const float Near)
    {
        return glm::ortho(Left, Right, Bottom, Top, Near, Far);
    }

    const float* ToArray(const Matrix4& a)
    {
        return glm::value_ptr(a);
    }

    Matrix4 MakeMatrix4(const float* a)
    {
        return glm::make_mat4(a);
    }
}

std::ostream& operator<<(std::ostream& Stream, const Matrix4& Other)
{
    const float* Data = glm::value_ptr(Other);
    Stream << Data[0] << " " << Data[1] << " " << Data[2] << " " << Data[3] << "\n";
    Stream << Data[4] << " " << Data[5] << " " << Data[6] << " " << Data[7] << "\n";
    Stream << Data[8] << " " << Data[9] << " " << Data[10] << " " << Data[11] << "\n";
    Stream << Data[12] << " " << Data[13] << " " << Data[14] << " " << Data[15];
    return Stream;
}
