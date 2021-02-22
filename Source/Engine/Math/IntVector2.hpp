#pragma once
#include "Functions.hpp"
#include <ostream>

class IntVector2
{
public:
    IntVector2(const int Val = 0):
        x( Val ), y( Val )
    {
    }

    IntVector2(const int Val1, const int Val2):
        x( Val1 ), y( Val2 )
    {
    }

    IntVector2(const IntVector2& Other):
        x( Other.x ), y( Other.y )
    {
    }

    ~IntVector2() {}

    IntVector2& operator=(const IntVector2& Other)
    {
        if( this != &Other )
        {
            x = Other.x;
            y = Other.y;
        }
        return *this;
    }

    int& operator[](std::size_t idx)
    {
        return (&x)[idx];
    }

    const int& operator[](std::size_t idx) const
    {
        return (&x)[idx];
    }

    IntVector2& operator*=(const int Other)
    {
        x *= Other;
        y *= Other;
        return *this;
    }

    IntVector2 operator*(const int Other) const
    {
        return IntVector2( x * Other, y * Other );
    }

    IntVector2& operator*=(const IntVector2& Other)
    {
        x *= Other.x;
        y *= Other.y;
        return *this;
    }

    IntVector2 operator*(const IntVector2& Other) const
    {
        return IntVector2( x * Other.x, y * Other.y );
    }

    IntVector2& operator+=(const int Other)
    {
        x += Other;
        y += Other;
        return *this;
    }

    IntVector2 operator+(const int Other) const
    {
        return IntVector2( x + Other, y + Other );
    }

    IntVector2& operator+=(const IntVector2& Other)
    {
        x += Other.x;
        y += Other.y;
        return *this;
    }

    IntVector2 operator+(const IntVector2& Other) const
    {
        return IntVector2( x + Other.x, y + Other.y );
    }

    IntVector2 operator-() const
    {
        return {-x, -y};
    }

    IntVector2& operator-=(const int Other)
    {
        x -= Other;
        y -= Other;
        return *this;
    }

    IntVector2 operator-(const int Other) const
    {
        return IntVector2( x - Other, y - Other );
    }

    IntVector2& operator-=(const IntVector2& Other)
    {
        x -= Other.x;
        y -= Other.y;
        return *this;
    }

    IntVector2 operator-(const IntVector2& Other) const
    {
        return IntVector2( x - Other.x, y - Other.y );
    }

    IntVector2& operator/=(const int Other)
    {
        x /= Other;
        y /= Other;
        return *this;
    }

    IntVector2 operator/(const int Other) const
    {
        return IntVector2( x / Other, y / Other );
    }

    IntVector2& operator/=(const IntVector2& Other)
    {
        x /= Other.x;
        y /= Other.y;
        return *this;
    }

    IntVector2 operator/(const IntVector2& Other) const
    {
        return IntVector2( x / Other.x, y / Other.y );
    }

    static IntVector2 ZERO;
    static IntVector2 ONE;
public:
    int x = 0;
    int y = 0;
};

std::ostream& operator<<(std::ostream& Stream, const IntVector2& Other);