#pragma once
#include "Functions.hpp"
#include "Vector4.hpp"
#include "Matrix3.hpp"

class Matrix4
{
public:
    Matrix4()
    {
        SetIdentity();
    }

    Matrix4(const float a1, const float a2, const float a3, const float a4,
            const float a5, const float a6, const float a7, const float a8,
            const float a9, const float a10, const float a11, const float a12,
            const float a13, const float a14, const float a15, const float a16):
                Values{ a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16 }
    {
    }

    Matrix4(const Vector4& a, const Vector4& b, const Vector4& c, const Vector4& d):
            Values{ a.x, a.y, a.z, a.w, b.x, b.y, b.z, b.w, c.x, c.y, c.z, c.w, d.x, d.y, d.z, d.w }
    {
    }

    Matrix4(const Matrix3& m):
            Values{ m[0], m[1], m[2], 0.0f, m[3], m[4], m[5], 0.0f, m[6], m[7], m[8], 0.0f, 0.0f, 0.0f, 0.0f, 1.0f }
    {
    }

    Matrix4(const float* D)
    {
        for(int i = 0; i < 16; ++i)
        {
            Values[i] = D[i];
        }
    }

    Matrix4(const Matrix4& D)
    {
        for(int i = 0; i < 16; ++i)
        {
            Values[i] = D.Values[i];
        }
    }

    ~Matrix4()
    {
    }

    void SetIdentity();
    //
    Matrix4 Inverse() const;
    //
    void Invert();
    //
    Matrix4 Transposition() const;
    //
    void Transpose();

    Vector3 GetTranslation() const
    {
        return { Values[12], Values[13], Values[14] };
    }

    // Valid?
    Vector3 GetScale() const
    {
        return { 
            Math::Sqrt(Values[0] * Values[0] + Values[1] * Values[1] + Values[2] * Values[2]),
            Math::Sqrt(Values[4] * Values[4] + Values[5] * Values[5] + Values[6] * Values[6]),
            Math::Sqrt(Values[8] * Values[8] + Values[9] * Values[9] + Values[10] * Values[10])
        };
    }

    void SetForward(const Vector3& v)
    {
        Values[0] = v.x;
        Values[1] = v.y;
        Values[2] = v.z;
    }

    Vector3 GetForward() const
    {
        return { Values[0], Values[1], Values[2] };
    }

    void SetUp(const Vector3& v)
    {
        Values[4] = v.x;
        Values[5] = v.y;
        Values[6] = v.z;
    }

    Vector3 GetUp() const
    {
        return { Values[4], Values[5], Values[6] };
    }

    void SetRight(const Vector3& v)
    {
        Values[8] = v.x;
        Values[9] = v.y;
        Values[10] = v.z;
    }

    Vector3 GetRight() const
    {
        return { Values[8], Values[9], Values[10] };
    }

    float& operator[](const std::size_t idx)
    {
        return Values[idx];
    }

    const float& operator[](const std::size_t idx) const
    {
        return Values[idx];
    }

    Matrix4& operator=(const Matrix4& D)
    {
        if( this == &D )
        {
            return *this;
        }
        for(int i = 0; i < 16; ++i)
        {
            Values[i] = D.Values[i];
        }
        return *this;
    }

    Vector3 operator*(const Vector3& v) const
    {
        Vector4 Tmp = Multiply({ v, 1.0f });
        return {Tmp.x, Tmp.y, Tmp.z};
    }

    Vector4 operator*(const Vector4& v) const
    {
        Multiply(v);
    }

    Matrix4 operator*(const Matrix4& o) const;

    float* Data() { return Values; }
    const float* Data() const { return Values; }

    static Matrix4 IDENTITY;
private:
    Vector4 Multiply(const Vector4& v) const
    {
        return
        {
            Values[0] * v.x + Values[1] * v.y + Values[2] * v.z + Values[3] * v.w,
            Values[4] * v.x + Values[5] * v.y + Values[6] * v.z + Values[7] * v.w,
            Values[8] * v.x + Values[9] * v.y + Values[10] * v.z + Values[11] * v.w,
            Values[12] * v.x + Values[13] * v.y + Values[14] * v.z + Values[15] * v.w,
        };
    }
private:
    /*
    * 0  1  2  3
    * 4  5  6  7
    * 8  9  10 11
    * 12 13 14 15
    */
    float Values[16];
};


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

    Matrix4 Scale(const float x);
    Matrix4 Scale(const float x, const float y, const float z);
    Matrix4 Scale(const Vector3& Scale);

    Matrix4 Translation(const float x, const float y, const float z);
    Matrix4 Translation(const Vector3& Translation);

    // World Space Arguments
    Matrix4 LookAt(const Vector3& Camera, const Vector3& Position, const Vector3& Up);
    // In Degrees
    Matrix4 Perspective(const float Fov, const float Aspect, const float Near, const float Far);
    Matrix4 Ortho(const float Right, const float Left, const float Top, const float Bottom, const float Far, const float Near);

    Matrix4 Lerp(const Matrix4& a, const Matrix4& b, const float t);
}

std::ostream& operator<<(std::ostream& Stream, const Matrix4& Other);
