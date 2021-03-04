#pragma once
#include "Functions.hpp"
#include "Vector2.hpp"
#include <ostream>

class Vector3
{
public:
    Vector3(const float Val = 0.0f):
        x( Val ), y( Val ), z( Val )
    {
    }

    Vector3(const float Val1, const float Val2, const float Val3):
        x( Val1 ), y( Val2 ), z( Val3 )
    {
    }

    Vector3(const Vector2& Val1, const float Val3):
        x( Val1.x ), y( Val1.y ), z( Val3 )
    {
    }

    Vector3(const Vector3& Other):
        x( Other.x ), y( Other.y ), z( Other.z )
    {
    }

    ~Vector3() {}

    Vector3 Normalized() const;
    //
    void Normalize();
    //
    float Distance(const Vector3& b) const;
    //
    float DistanceSquared(const Vector3& b) const;
    //
    float Length() const;
    //
    float LengthSquared() const;
    //
    float DotProduct(const Vector3& b) const;
    //
    Vector3 CrossProduct(const Vector3& b) const;
    // Vectors Normalized, In Degrees
    float Angle(const Vector3& To) const;
    // Vectors Normalized, In Degrees
    float SignedAngle(const Vector3& To, const Vector3& Axis) const;

    Vector2 XZ() const
    {
        return { x, z };
    }

    Vector2 XY() const
    {
        return { x, y };
    }

    Vector2 YZ() const
    {
        return { y, z };
    }

    Vector3& operator=(const Vector3& Other)
    {
        if( this != &Other )
        {
            x = Other.x;
            y = Other.y;
            z = Other.z;
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

    Vector3& operator*=(const float Other)
    {
        x *= Other;
        y *= Other;
        z *= Other;
        return *this;
    }

    Vector3 operator*(const float Other) const
    {
        return Vector3( x * Other, y * Other, z * Other );
    }

    Vector3& operator*=(const Vector3& Other)
    {
        x *= Other.x;
        y *= Other.y;
        z *= Other.z;
        return *this;
    }

    Vector3 operator*(const Vector3& Other) const
    {
        return Vector3( x * Other.x, y * Other.y, z * Other.z );
    }

    Vector3& operator+=(const float Other)
    {
        x += Other;
        y += Other;
        z += Other;
        return *this;
    }

    Vector3 operator+(const float Other) const
    {
        return Vector3( x + Other, y + Other, z + Other );
    }

    Vector3& operator+=(const Vector3& Other)
    {
        x += Other.x;
        y += Other.y;
        z += Other.z;
        return *this;
    }

    Vector3 operator+(const Vector3& Other) const
    {
        return Vector3( x + Other.x, y + Other.y, z + Other.z );
    }

    Vector3 operator-() const
    {
        return {-x, -y, -z};
    }

    Vector3& operator-=(const float Other)
    {
        x -= Other;
        y -= Other;
        z -= Other;
        return *this;
    }

    Vector3 operator-(const float Other) const
    {
        return Vector3( x - Other, y - Other, z - Other );
    }

    Vector3& operator-=(const Vector3& Other)
    {
        x -= Other.x;
        y -= Other.y;
        z -= Other.z;
        return *this;
    }

    Vector3 operator-(const Vector3& Other) const
    {
        return Vector3( x - Other.x, y - Other.y, z - Other.z );
    }

    Vector3& operator/=(const float Other)
    {
        x /= Other;
        y /= Other;
        z /= Other;
        return *this;
    }

    Vector3 operator/(const float Other) const
    {
        return Vector3( x / Other, y / Other, z / Other );
    }

    Vector3& operator/=(const Vector3& Other)
    {
        x /= Other.x;
        y /= Other.y;
        z /= Other.z;
        return *this;
    }

    Vector3 operator/(const Vector3& Other) const
    {
        return Vector3( x / Other.x, y / Other.y, z / Other.z );
    }

    static Vector3 UP;
    static Vector3 DOWN;
    static Vector3 RIGHT;
    static Vector3 LEFT;
    static Vector3 FORWARD;
    static Vector3 BACK;
    static Vector3 ZERO;
    static Vector3 ONE;
public:
    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;
};

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
    bool IsEqual(const Vector3& a, const Vector3& b, const float Eps);
}

std::ostream& operator<<(std::ostream& Stream, const Vector3& Other);