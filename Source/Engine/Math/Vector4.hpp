#pragma once
#include "Functions.hpp"
#include "Vector3.hpp"
#include <ostream>

template<class T>
class TVector4
{
public:
    TVector4(const T Val = T(0)):
        x( Val ), y( Val ), z( Val ), w( Val )
    {
    }

    TVector4(const T Val1, const T Val2, const T Val3, const T Val4):
        x( Val1 ), y( Val2 ), z( Val3 ), w( Val4 )
    {
    }

    TVector4(const TVector3<T>& Val1, const T Val4):
        x( Val1.x ), y( Val1.y ), z( Val1.z ), w( Val4 )
    {
    }

    TVector4(const TVector4<T>& Other):
        x( Other.x ), y( Other.y ), z( Other.z ), w( Other.w )
    {
    }

    ~TVector4() {}

    TVector3<T> ToXYZ() const
    {
        return {x, y, z};
    }

    TVector4<T>& operator=(const TVector4<T>& Other)
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

    T& operator[](std::size_t idx)
    {
        return (&x)[idx];
    }

    const T& operator[](std::size_t idx) const
    {
        return (&x)[idx];
    }

    TVector4<T>& operator*=(const T Other)
    {
        x *= Other;
        y *= Other;
        z *= Other;
        w *= Other;
        return *this;
    }

    TVector4<T> operator*(const T Other) const
    {
        return TVector4<T>( x * Other, y * Other, z * Other, w * Other );
    }

    TVector4<T>& operator*=(const TVector4<T>& Other)
    {
        x *= Other.x;
        y *= Other.y;
        z *= Other.z;
        w *= Other.w;
        return *this;
    }

    TVector4<T> operator*(const TVector4<T>& Other) const
    {
        return TVector4<T>( x * Other.x, y * Other.y, z * Other.z, w * Other.w );
    }

    TVector4<T>& operator+=(const T Other)
    {
        x += Other;
        y += Other;
        z += Other;
        w += Other;
        return *this;
    }

    TVector4<T> operator+(const T Other) const
    {
        return TVector4<T>( x + Other, y + Other, z + Other, w + Other );
    }

    TVector4<T>& operator+=(const TVector4<T>& Other)
    {
        x += Other.x;
        y += Other.y;
        z += Other.z;
        w += Other.w;
        return *this;
    }

    TVector4<T> operator+(const TVector4<T>& Other) const
    {
        return TVector4( x + Other.x, y + Other.y, z + Other.z, w + Other.w );
    }

    TVector4<T> operator-() const
    {
        return {-x, -y, -z, -w};
    }

    TVector4<T>& operator-=(const T Other)
    {
        x -= Other;
        y -= Other;
        z -= Other;
        w -= Other;
        return *this;
    }

    TVector4<T> operator-(const T Other) const
    {
        return TVector4<T>( x - Other, y - Other, z - Other, w - Other );
    }

    TVector4<T>& operator-=(const TVector4<T>& Other)
    {
        x -= Other.x;
        y -= Other.y;
        z -= Other.z;
        w -= Other.w;
        return *this;
    }

    TVector4<T> operator-(const TVector4<T>& Other) const
    {
        return TVector4<T>( x - Other.x, y - Other.y, z - Other.z, w - Other.w );
    }

    TVector4<T>& operator/=(const T Other)
    {
        x /= Other;
        y /= Other;
        z /= Other;
        w /= Other;
        return *this;
    }

    TVector4<T> operator/(const T Other) const
    {
        return TVector4<T>( x / Other, y / Other, z / Other, w / Other );
    }

    TVector4<T>& operator/=(const TVector4<T>& Other)
    {
        x /= Other.x;
        y /= Other.y;
        z /= Other.z;
        w /= Other.w;
        return *this;
    }

    TVector4<T> operator/(const TVector4<T>& Other) const
    {
        return TVector4<T>( x / Other.x, y / Other.y, z / Other.z, w / Other.w );
    }

    static TVector4<T> ZERO;
    static TVector4<T> ONE;
public:
    T x = T(0);
    T y = T(0);
    T z = T(0);
    T w = T(0);
};

template<class T> TVector4<T> TVector4<T>::ZERO = { T(0), T(0), T(0), T(0) };
template<class T> TVector4<T> TVector4<T>::ONE = { T(1), T(1), T(1), T(1) };

using Vector4 = TVector4<float>;

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