#pragma once
#include "Functions.hpp"
#include "IntVector2.hpp"
#include <ostream>

class Vector2
{
public:
    Vector2(const float Val = 0.0f):
        x( Val ), y( Val )
    {
    }

    Vector2(const float Val1, const float Val2):
        x( Val1 ), y( Val2 )
    {
    }

    Vector2(const IntVector2& Other):
        x( static_cast<float>(Other.x) ), y( static_cast<float>(Other.y) )
    {
    }

    Vector2(const Vector2& Other):
        x( Other.x ), y( Other.y )
    {
    }

    ~Vector2() {}

    Vector2 Normalized() const;
    //
    void Normalize();
    //
    float Distance(const Vector2& b) const;
    //
    float DistanceSquared(const Vector2& b) const;
    //
    float Length() const;
    //
    float LengthSquared() const;
    //
    float DotProduct(const Vector2& b) const;
    //
    float CrossProduct(const Vector2& b) const;
    // Vectors Normalized, In Degrees
    float Angle(const Vector2& To) const;

    Vector2& operator=(const Vector2& Other)
    {
        if( this != &Other )
        {
            x = Other.x;
            y = Other.y;
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

    Vector2& operator*=(const float Other)
    {
        x *= Other;
        y *= Other;
        return *this;
    }

    Vector2 operator*(const float Other) const
    {
        return Vector2( x * Other, y * Other );
    }

    Vector2& operator*=(const Vector2& Other)
    {
        x *= Other.x;
        y *= Other.y;
        return *this;
    }

    Vector2 operator*(const Vector2& Other) const
    {
        return Vector2( x * Other.x, y * Other.y );
    }

    Vector2& operator+=(const float Other)
    {
        x += Other;
        y += Other;
        return *this;
    }

    Vector2 operator+(const float Other) const
    {
        return Vector2( x + Other, y + Other );
    }

    Vector2& operator+=(const Vector2& Other)
    {
        x += Other.x;
        y += Other.y;
        return *this;
    }

    Vector2 operator+(const Vector2& Other) const
    {
        return Vector2( x + Other.x, y + Other.y );
    }

    Vector2 operator-() const
    {
        return {-x, -y};
    }

    Vector2& operator-=(const float Other)
    {
        x -= Other;
        y -= Other;
        return *this;
    }

    Vector2 operator-(const float Other) const
    {
        return Vector2( x - Other, y - Other );
    }

    Vector2& operator-=(const Vector2& Other)
    {
        x -= Other.x;
        y -= Other.y;
        return *this;
    }

    Vector2 operator-(const Vector2& Other) const
    {
        return Vector2( x - Other.x, y - Other.y );
    }

    Vector2& operator/=(const float Other)
    {
        x /= Other;
        y /= Other;
        return *this;
    }

    Vector2 operator/(const float Other) const
    {
        return Vector2( x / Other, y / Other );
    }

    Vector2& operator/=(const Vector2& Other)
    {
        x /= Other.x;
        y /= Other.y;
        return *this;
    }

    Vector2 operator/(const Vector2& Other) const
    {
        return Vector2( x / Other.x, y / Other.y );
    }

    static Vector2 UP;
    static Vector2 DOWN;
    static Vector2 RIGHT;
    static Vector2 LEFT;
    static Vector2 ZERO;
    static Vector2 ONE;
public:
    float x = 0.0f;
    float y = 0.0f;
};

namespace Math
{
    Vector2 Normalize(const Vector2& a);
    float Distance(const Vector2& a, const Vector2& b);
    float DistanceSquared(const Vector2& a, const Vector2& b);
    float Length(const Vector2& a);
    float LengthSquared(const Vector2& a);
    float DotProduct(const Vector2& a, const Vector2& b);
    float CrossProduct(const Vector2& a, const Vector2& b);
    // Vectors Normalized, In Degrees
    float Angle(const Vector2& From, const Vector2& To);
    Vector2 MoveTowards(const Vector2& Now, const Vector2& Target, const float Delta);
    Vector2 Lerp(const Vector2& a, const Vector2& b, const float t);
    Vector2 Max(const Vector2& a, const Vector2& b);
    Vector2 Min(const Vector2& a, const Vector2& b);
}

std::ostream& operator<<(std::ostream& Stream, const Vector2& Other);