#pragma once
#include "Functions.hpp"
#include <ostream>

template<class T>
class TVector2
{
public:
    TVector2(const T Val = T(0)):
        x( Val ), y( Val )
    {
    }

    TVector2(const T Val1, const T Val2):
        x( Val1 ), y( Val2 )
    {
    }

    TVector2(const TVector2<T>& Other):
        x( Other.x ), y( Other.y )
    {
    }

    ~TVector2() {}

    TVector2<T>& operator=(const TVector2<T>& Other)
    {
        if( this != &Other )
        {
            x = Other.x;
            y = Other.y;
        }
        return *this;
    }

    T& operator[](std::size_t idx)
    {
        return (&x)[idx];
    }

    const T& operator[](std::size_t idx) const
    {
        return (&x)[idx];
    }

    TVector2<T>& operator*=(const T Other)
    {
        x *= Other;
        y *= Other;
        return *this;
    }

    TVector2<T> operator*(const T Other) const
    {
        return TVector2<T>( x * Other, y * Other );
    }

    TVector2<T>& operator*=(const TVector2<T>& Other)
    {
        x *= Other.x;
        y *= Other.y;
        return *this;
    }

    TVector2<T> operator*(const TVector2<T>& Other) const
    {
        return TVector2<T>( x * Other.x, y * Other.y );
    }

    TVector2<T>& operator+=(const T Other)
    {
        x += Other;
        y += Other;
        return *this;
    }

    TVector2<T> operator+(const T Other) const
    {
        return TVector2<T>( x + Other, y + Other );
    }

    TVector2<T>& operator+=(const TVector2<T>& Other)
    {
        x += Other.x;
        y += Other.y;
        return *this;
    }

    TVector2<T> operator+(const TVector2<T>& Other) const
    {
        return TVector2( x + Other.x, y + Other.y );
    }

    TVector2<T> operator-() const
    {
        return {-x, -y};
    }

    TVector2<T>& operator-=(const T Other)
    {
        x -= Other;
        y -= Other;
        return *this;
    }

    TVector2<T> operator-(const T Other) const
    {
        return TVector2<T>( x - Other, y - Other );
    }

    TVector2<T>& operator-=(const TVector2<T>& Other)
    {
        x -= Other.x;
        y -= Other.y;
        return *this;
    }

    TVector2<T> operator-(const TVector2<T>& Other) const
    {
        return TVector2<T>( x - Other.x, y - Other.y );
    }

    TVector2<T>& operator/=(const T Other)
    {
        x /= Other;
        y /= Other;
        return *this;
    }

    TVector2<T> operator/(const T Other) const
    {
        return TVector2<T>( x / Other, y / Other );
    }

    TVector2<T>& operator/=(const TVector2<T>& Other)
    {
        x /= Other.x;
        y /= Other.y;
        return *this;
    }

    TVector2<T> operator/(const TVector2<T>& Other) const
    {
        return TVector2<T>( x / Other.x, y / Other.y );
    }

    static TVector2<T> UP;
    static TVector2<T> DOWN;
    static TVector2<T> RIGHT;
    static TVector2<T> LEFT;
    static TVector2<T> ZERO;
    static TVector2<T> ONE;
public:
    T x = T(0);
    T y = T(0);
};

template<class T> TVector2<T> TVector2<T>::UP = { T(0), T(1) };
template<class T> TVector2<T> TVector2<T>::DOWN = { T(0), T(-1) };
template<class T> TVector2<T> TVector2<T>::RIGHT = { T(1), T(0) };
template<class T> TVector2<T> TVector2<T>::LEFT = { T(-1), T(0) };
template<class T> TVector2<T> TVector2<T>::ZERO = { T(0), T(0) };
template<class T> TVector2<T> TVector2<T>::ONE = { T(1), T(1) };

using Vector2 = TVector2<float>;
using IntVector2 = TVector2<int>;

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
std::ostream& operator<<(std::ostream& Stream, const IntVector2& Other);