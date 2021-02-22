#pragma once
#include "Functions.hpp"
#include "Vector3.hpp"
#include <ostream>

class Vector4
{
public:
    Vector4(const float Val = 0.0f):
        x( Val ), y( Val ), z( Val ), w( Val )
    {
    }

    Vector4(const float Val1, const float Val2, const float Val3, const float Val4):
        x( Val1 ), y( Val2 ), z( Val3 ), w( Val4 )
    {
    }

    Vector4(const Vector3& Val1, const float Val4):
        x( Val1.x ), y( Val1.y ), z( Val1.z ), w( Val4 )
    {
    }

    Vector4(const Vector4& Other):
        x( Other.x ), y( Other.y ), z( Other.z ), w( Other.w )
    {
    }

    ~Vector4() {}

    Vector4 Normalized() const;
    //
    void Normalize();
    //
    float Distance(const Vector4& b) const;
    //
    float DistanceSquared(const Vector4& b) const;
    //
    float Length() const;
    //
    float LengthSquared() const;
    //
    float DotProduct(const Vector4& b) const;

    Vector3 ToXYZ() const
    {
        return {x, y, z};
    }

    Vector4& operator=(const Vector4& Other)
    {
        if( this != &Other )
        {
            x = Other.x;
            y = Other.y;
            z = Other.z;
            w = Other.w;
        }
        return *this;
    }

    float& operator[](std::size_t idx)
    {
        return (&x)[idx];
    }

    const float& operator[](std::size_t idx) const
    {
        return (&x)[idx];
    }

    Vector4& operator*=(const float Other)
    {
        x *= Other;
        y *= Other;
        z *= Other;
        w *= Other;
        return *this;
    }

    Vector4 operator*(const float Other) const
    {
        return Vector4( x * Other, y * Other, z * Other, w * Other );
    }

    Vector4& operator*=(const Vector4& Other)
    {
        x *= Other.x;
        y *= Other.y;
        z *= Other.z;
        w *= Other.w;
        return *this;
    }

    Vector4 operator*(const Vector4& Other) const
    {
        return Vector4( x * Other.x, y * Other.y, z * Other.z, w * Other.w );
    }

    Vector4& operator+=(const float Other)
    {
        x += Other;
        y += Other;
        z += Other;
        w += Other;
        return *this;
    }

    Vector4 operator+(const float Other) const
    {
        return Vector4( x + Other, y + Other, z + Other, w + Other );
    }

    Vector4& operator+=(const Vector4& Other)
    {
        x += Other.x;
        y += Other.y;
        z += Other.z;
        w += Other.w;
        return *this;
    }

    Vector4 operator+(const Vector4& Other) const
    {
        return Vector4( x + Other.x, y + Other.y, z + Other.z, w + Other.w );
    }

    Vector4 operator-() const
    {
        return {-x, -y, -z, -w};
    }

    Vector4& operator-=(const float Other)
    {
        x -= Other;
        y -= Other;
        z -= Other;
        w -= Other;
        return *this;
    }

    Vector4 operator-(const float Other) const
    {
        return Vector4( x - Other, y - Other, z - Other, w - Other );
    }

    Vector4& operator-=(const Vector4& Other)
    {
        x -= Other.x;
        y -= Other.y;
        z -= Other.z;
        w -= Other.w;
        return *this;
    }

    Vector4 operator-(const Vector4& Other) const
    {
        return Vector4( x - Other.x, y - Other.y, z - Other.z, w - Other.w );
    }

    Vector4& operator/=(const float Other)
    {
        x /= Other;
        y /= Other;
        z /= Other;
        w /= Other;
        return *this;
    }

    Vector4 operator/(const float Other) const
    {
        return Vector4( x / Other, y / Other, z / Other, w / Other );
    }

    Vector4& operator/=(const Vector4& Other)
    {
        x /= Other.x;
        y /= Other.y;
        z /= Other.z;
        w /= Other.w;
        return *this;
    }

    Vector4 operator/(const Vector4& Other) const
    {
        return Vector4( x / Other.x, y / Other.y, z / Other.z, w / Other.w );
    }

    static Vector4 ZERO;
    static Vector4 ONE;
public:
    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;
    float w = 0.0f;
};

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
}

std::ostream& operator<<(std::ostream& Stream, const Vector4& Other);