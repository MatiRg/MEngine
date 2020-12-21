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

    TVector4<T> Inversion() const
    {
        return TVector4<T>( -x, -y, -z, -w );
    }

    void Invert()
    {
        x = -x;
        y = -y;
        z = -z;
        w = -w;
    }

    // Length(magnitude) of Vector
    T Length() const
    {
        return Math::Sqrt( LengthSquared() );
    }

    // Length(magnitude) of Vector Squared
    T LengthSquared() const
    {
        return x*x + y*y + z*z + w*w;
    }

    void Normalize()
    {
        T L = Length();
        if( L )
        {
            x /= L;
            y /= L;
            z /= L;
            w /= L;
        }
    }

    T DotProduct(const TVector4<T>& Other) const
    {
        return x*Other.x + y*Other.y + z*Other.z + w*Other.w;
    }

    // Angle between vectors
    T Angle(const TVector4<T>& Other) const
    {
        return Math::RadToDeg( Math::Acos( DotProduct( Other ) / ( Length() * Other.Length() ) ) );
    }

    void AddScaledVector(const TVector4<T>& Other, const T Scale)
    {
        x += Other.x * Scale;
        y += Other.y * Scale;
        z += Other.z * Scale;
        w += Other.w * Scale;
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
        return Inversion();
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
using IntVector4 = TVector4<int>;
using Size4 = TVector4<uint32_t>;

namespace Math
{
    template<class T>
    TVector4<T> Normalize(const TVector4<T>& a)
    {
        TVector4<T> t = a;
        t.Normalize();
        return t;
    }

    template<class T>
    T DotProduct(const TVector4<T>& a, const TVector4<T>& b)
    {
        return a.DotProduct(b);
    }

    template<class T>
    T Angle(const TVector4<T>& a, const TVector4<T>& b)
    {
        return a.Angle(b);
    }

    template<class T>
    TVector4<T> Lerp(const TVector4<T>& a, const TVector4<T>& b, const T v)
    {
        TVector4<T> R;
        for(int i = 0; i < 4; ++i)
        {
            R[i] = Lerp( a[i], b[i], v );
        }
        return R;
    }
}

template<class T>
std::ostream& operator<<(std::ostream& Stream, const TVector4<T>& Other)
{
    Stream << Other.x << ", " << Other.y << ", " << Other.z << ", " << Other.w;
    return Stream;
}
