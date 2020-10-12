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

    TVector2<T> Inversion() const
    {
        return TVector2<T>( -x, -y );
    }

    void Invert()
    {
        x = -x;
        y = -y;
    }

    /**< Length(magnitude) of Vector */
    T Length() const
    {
        return Math::Sqrt( LengthSquared() );
    }

    /**< Length(magnitude) of Vector Squared */
    T LengthSquared() const
    {
        return x*x + y*y;
    }

    void Normalize()
    {
        T L = Length();
        if( L )
        {
            x /= L;
            y /= L;
        }
    }

    T DotProduct(const TVector2<T>& Other) const
    {
        return x*Other.x + y*Other.y;
    }

    T Angle() const
    {
        return Math::RadToDeg( Math::Atan2(y, x) );
    }

    // Angle between vectors
    T Angle(const TVector2<T>& Other) const
    {
        return Math::RadToDeg( Math::Acos( DotProduct( Other ) / ( Length() * Other.Length() ) ) );
    }

    void SetRotation(T Angle)
    {
        Angle = Math::DegToRad(Angle);
        x = Math::Cos(Angle);
        y = Math::Sin(Angle);
    }

    void AddScaledVector(const TVector2<T>& Other, const T Scale)
    {
        x += Other.x * Scale;
        y += Other.y * Scale;
    }

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
        return Inversion();
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

    static TVector2<T> UP() { return TVector2<T>( T(0), T(1) ); }
    static TVector2<T> DOWN() { return TVector2<T>( T(0), T(-1) ); }
    static TVector2<T> RIGHT() { return TVector2<T>( T(1), T(0) ); }
    static TVector2<T> LEFT() { return TVector2<T>( T(-1), T(0) ); }
    static TVector2<T> ZERO() { return TVector2<T>( T(0), T(0) ); }
    static TVector2<T> ONE() { return TVector2<T>( T(1), T(1) ); }
public:
    T x = T(0);
    T y = T(0);
};

using Vector2 = TVector2<float>;
using IntVector2 = TVector2<int>;
using Size2 = TVector2<uint32_t>;

namespace Math
{
    template<class T>
    TVector2<T> Normalize(const TVector2<T>& a)
    {
        TVector2<T> t = a;
        t.Normalize();
        return t;
    }

    template<class T>
    T DotProduct(const TVector2<T>& a, const TVector2<T>& b)
    {
        return a.DotProduct(b);
    }

    template<class T>
    T Angle(const TVector2<T>& a, const TVector2<T>& b)
    {
        return a.Angle(b);
    }

    template<class T>
    TVector2<T> Lerp(const TVector2<T>& a, const TVector2<T>& b, const T v)
    {
        TVector2<T> R;
        for(int i = 0; i < 2; ++i)
        {
            R[i] = Lerp( a[i], b[i], v );
        }
        return R;
    }
}

template<class T>
std::ostream& operator<<(std::ostream& Stream, const TVector2<T>& Other)
{
    Stream << Other.x << ", " << Other.y;
    return Stream;
}
