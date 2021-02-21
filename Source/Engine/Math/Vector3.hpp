#pragma once
#include "Functions.hpp"
#include "Vector2.hpp"
#include <ostream>

template<class T>
class TVector3
{
public:
    TVector3(const T Val = T(0)):
        x( Val ), y( Val ), z( Val )
    {
    }

    TVector3(const T Val1, const T Val2, const T Val3):
        x( Val1 ), y( Val2 ), z( Val3 )
    {
    }

    TVector3(const TVector2<T>& Val1, const T Val3):
        x( Val1.x ), y( Val1.y ), z( Val3 )
    {
    }

    TVector3(const TVector3<T>& Other):
        x( Other.x ), y( Other.y ), z( Other.z )
    {
    }

    ~TVector3() {}

    TVector2<T> XZ() const
    {
        return { x, z };
    }

    TVector2<T> XY() const
    {
        return { x, y };
    }

    TVector2<T> YZ() const
    {
        return { y, z };
    }

    TVector3<T>& operator=(const TVector3<T>& Other)
    {
        if( this != &Other )
        {
            x = Other.x;
            y = Other.y;
            z = Other.z;
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

    TVector3<T>& operator*=(const T Other)
    {
        x *= Other;
        y *= Other;
        z *= Other;
        return *this;
    }

    TVector3<T> operator*(const T Other) const
    {
        return TVector3<T>( x * Other, y * Other, z * Other );
    }

    TVector3<T>& operator*=(const TVector3<T>& Other)
    {
        x *= Other.x;
        y *= Other.y;
        z *= Other.z;
        return *this;
    }

    TVector3<T> operator*(const TVector3<T>& Other) const
    {
        return TVector3<T>( x * Other.x, y * Other.y, z * Other.z );
    }

    TVector3<T>& operator+=(const T Other)
    {
        x += Other;
        y += Other;
        z += Other;
        return *this;
    }

    TVector3<T> operator+(const T Other) const
    {
        return TVector3<T>( x + Other, y + Other, z + Other );
    }

    TVector3<T>& operator+=(const TVector3<T>& Other)
    {
        x += Other.x;
        y += Other.y;
        z += Other.z;
        return *this;
    }

    TVector3<T> operator+(const TVector3<T>& Other) const
    {
        return TVector3( x + Other.x, y + Other.y, z + Other.z );
    }

    TVector3<T> operator-() const
    {
        return {-x, -y, -z};
    }

    TVector3<T>& operator-=(const T Other)
    {
        x -= Other;
        y -= Other;
        z -= Other;
        return *this;
    }

    TVector3<T> operator-(const T Other) const
    {
        return TVector3<T>( x - Other, y - Other, z - Other );
    }

    TVector3<T>& operator-=(const TVector3<T>& Other)
    {
        x -= Other.x;
        y -= Other.y;
        z -= Other.z;
        return *this;
    }

    TVector3<T> operator-(const TVector3<T>& Other) const
    {
        return TVector3<T>( x - Other.x, y - Other.y, z - Other.z );
    }

    TVector3<T>& operator/=(const T Other)
    {
        x /= Other;
        y /= Other;
        z /= Other;
        return *this;
    }

    TVector3<T> operator/(const T Other) const
    {
        return TVector3<T>( x / Other, y / Other, z / Other );
    }

    TVector3<T>& operator/=(const TVector3<T>& Other)
    {
        x /= Other.x;
        y /= Other.y;
        z /= Other.z;
        return *this;
    }

    TVector3<T> operator/(const TVector3<T>& Other) const
    {
        return TVector3<T>( x / Other.x, y / Other.y, z / Other.z );
    }

    static TVector3<T> UP;
    static TVector3<T> DOWN;
    static TVector3<T> RIGHT;
    static TVector3<T> LEFT;
    static TVector3<T> FORWARD;
    static TVector3<T> BACK;
    static TVector3<T> ZERO;
    static TVector3<T> ONE;
public:
    T x = T(0);
    T y = T(0);
    T z = T(0);
};

template<class T> TVector3<T> TVector3<T>::UP = { T(0), T(1), T(0) };
template<class T> TVector3<T> TVector3<T>::DOWN = { T(0), T(-1), T(0) };
template<class T> TVector3<T> TVector3<T>::RIGHT = { T(1), T(0), T(0) };
template<class T> TVector3<T> TVector3<T>::LEFT = { T(-1), T(0), T(0) };
template<class T> TVector3<T> TVector3<T>::FORWARD = { T(0), T(0), T(1) };
template<class T> TVector3<T> TVector3<T>::BACK = { T(0), T(0), T(-1) };
template<class T> TVector3<T> TVector3<T>::ZERO = { T(0), T(0), T(0) };
template<class T> TVector3<T> TVector3<T>::ONE = { T(1), T(1), T(1) };

using Vector3 = TVector3<float>;

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
}

std::ostream& operator<<(std::ostream& Stream, const Vector3& Other);