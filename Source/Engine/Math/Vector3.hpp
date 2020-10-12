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

    TVector3<T> Inversion() const
    {
        return TVector3<T>( -x, -y, -z );
    }

    void Invert()
    {
        x = -x;
        y = -y;
        z = -z;
    }

    // Length(magnitude) of Vector
    T Length() const
    {
        return Math::Sqrt( LengthSquared() );
    }

    // Length(magnitude) of Vector Squared
    T LengthSquared() const
    {
        return x*x + y*y + z*z;
    }

    void Normalize()
    {
        T L = Length();
        if( L )
        {
            x /= L;
            y /= L;
            z /= L;
        }
    }

    T DotProduct(const TVector3<T>& Other) const
    {
        return x*Other.x + y*Other.y + z*Other.z;
    }

    TVector3<T> CrossProduct(const TVector3<T>& Other) const
    {
        return
        {
            y*Other.z - z*Other.y,
            z*Other.x - x*Other.z,
            x*Other.y - y*Other.x
        };
    }

    // Angle between vectors
    T Angle(const TVector3<T>& Other) const
    {
        return Math::RadToDeg( Math::Acos( DotProduct( Other ) / ( Length() * Other.Length() ) ) );
    }

    void AddScaledVector(const TVector3<T>& Other, const T Scale)
    {
        x += Other.x * Scale;
        y += Other.y * Scale;
        z += Other.z * Scale;
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
        return Inversion();
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

    static TVector3<T> UP() { return TVector3<T>( T(0), T(1), T(0) ); }
    static TVector3<T> DOWN() { return TVector3<T>( T(0), T(-1), T(0) ); }
    static TVector3<T> RIGHT() { return TVector3<T>( T(1), T(0), T(0) ); }
    static TVector3<T> LEFT() { return TVector3<T>( T(-1), T(0), T(0) ); }
    static TVector3<T> FORWARD() { return TVector3<T>( T(0), T(0), T(1) ); }
    static TVector3<T> BACK() { return TVector3<T>( T(0), T(0), T(-1) ); }
    static TVector3<T> ZERO() { return TVector3<T>( T(0), T(0), T(0) ); }
    static TVector3<T> ONE() { return TVector3<T>( T(1), T(1), T(1) ); }
public:
    T x = T(0);
    T y = T(0);
    T z = T(0);
};

using Vector3 = TVector3<float>;
using IntVector3 = TVector3<int>;
using Size3 = TVector3<uint32_t>;

namespace Math
{
    template<class T>
    TVector3<T> Normalize(const TVector3<T>& a)
    {
        TVector3<T> t = a;
        t.Normalize();
        return t;
    }

    template<class T>
    T DotProduct(const TVector3<T>& a, const TVector3<T>& b)
    {
        return a.DotProduct(b);
    }

    template<class T>
    TVector3<T> CrossProduct(const TVector3<T>& a, const TVector3<T>& b)
    {
        return a.CrossProduct(b);
    }

    template<class T>
    T Angle(const TVector3<T>& a, const TVector3<T>& b)
    {
        return a.Angle(b);
    }

    template<class T>
    TVector3<T> Lerp(const TVector3<T>& a, const TVector3<T>& b, const T v)
    {
        TVector3<T> R;
        for(int i = 0; i < 3; ++i)
        {
            R[i] = Lerp( a[i], b[i], v );
        }
        return R;
    }
}

template<class T>
std::ostream& operator<<(std::ostream& Stream, const TVector3<T>& Other)
{
    Stream << Other.x << ", " << Other.y << ", " << Other.z;
    return Stream;
}
