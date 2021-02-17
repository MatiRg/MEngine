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

    // Distance
    T Distance(const TVector3<T>& Other) const
    {
        return (*this - Other).Length();
    }

    // Distance
    T DistanceSquared(const TVector3<T>& Other) const
    {
        return (*this - Other).LengthSquared();
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

    TVector3<T> Normalized() const
    {
        TVector3<T> Tmp = *this;
        Tmp.Normalize();
        return Tmp;
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
        T Denominator = Length() * Other.Length();
        if (Denominator <= T(0.0000000001f))
        {
            return T(0);
        }
        T Dot = Math::Clamp(DotProduct(Other) / Denominator, T(-1), T(1));
        return Math::RadToDeg(Math::Acos(Dot));
    }

    void AddScaledVector(const TVector3<T>& Other, const T Scale)
    {
        x += Other.x * Scale;
        y += Other.y * Scale;
        z += Other.z * Scale;
    }

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
    T Distance(const TVector3<T>& a, const TVector3<T>& b)
    {
        return a.Distance(b);
    }

    template<class T>
    T DistanceSquared(const TVector3<T>& a, const TVector3<T>& b)
    {
        return a.DistanceSquared(b);
    }

    template<class T>
    T Length(const TVector3<T>& a)
    {
        return a.Length();
    }

    template<class T>
    T LengthSquared(const TVector3<T>& a)
    {
        return a.LengthSquared();
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
    T ScalarTripleProduct(const TVector3<T>& a, const TVector3<T>& b, const TVector3<T>& c)
    {
        return DotProduct( a, CrossProduct(b, c) );
    }

    template<class T>
    TVector3<T> TripleProduct(const TVector3<T>& a, const TVector3<T>& b, const TVector3<T>& c)
    {
        return CrossProduct(a, CrossProduct(b, c));
    }

    template<class T>
    TVector3<T> Reflect(const TVector3<T>& Dir, const TVector3<T>& Norm)
    {
        T f = T(-2) * Dot(Norm, Dir);
        return { f * Norm.x + Dir.x, f * Norm.y + Dir.y, f * Norm.z + Dir.z };
    }

    template<class T>
    TVector3<T> Project(const TVector3<T>& vector, const TVector3<T>& Norm)
    {
        T SqrMag = DotProduct(Norm, Norm);
        if (SqrMag < EPSILON<T>)
        {
            return TVector3<T>::ZERO;
        }
        else
        {
            T d = DotProduct(vector, Norm);
            return { Norm.x * d / SqrMag, Norm.y * d / SqrMag, Norm.z * d / SqrMag };
        }
    }

    template<class T>
    T Angle(const TVector3<T>& From, const TVector3<T>& To)
    {
        return From.Angle(To);
    }

    // Should work like Unity
    template<class T>
    T SignedAngle(const TVector3<T>& From, const TVector3<T>& To, const TVector3<T>& Axis)
    {
        T UnsignedAngle = Angle(From, To);
        TVector3<T> Cross = CrossProduct(From, To);
        return UnsignedAngle * Sign( DotProduct(Cross, Axis) );
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

    // https://answers.unity.com/questions/414829/any-one-know-maths-behind-this-movetowards-functio.html
    template<class T>
    TVector3<T> MoveTowards(const TVector3<T>& Now, const TVector3<T>& Target, const T Delta)
    {
        TVector3<T> Tmp = Target - Now;
        T Distance = Tmp.Length();
        if (Distance <= Delta || IsEqual(Distance, 0.0f, 0.000001f))
        {
            return Target;
        }
        return Now + (Tmp / Distance) * Delta;
    }

    template<class T>
    TVector3<T> Max(const TVector3<T>& l, const TVector3<T>& r)
    {
        return TVector3<T>(Max(l.x, r.x), Max(l.y, r.y), Max(l.z, r.z));
    }

    template<class T>
    TVector3<T> Min(const TVector3<T>& l, const TVector3<T>& r)
    {
        return TVector3<T>(Min(l.x, r.x), Min(l.y, r.y), Min(l.z, r.z));
    }
}

template<class T>
std::ostream& operator<<(std::ostream& Stream, const TVector3<T>& Other)
{
    Stream << Other.x << ", " << Other.y << ", " << Other.z;
    return Stream;
}
