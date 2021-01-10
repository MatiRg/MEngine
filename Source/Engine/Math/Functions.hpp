#pragma once
#include <cmath>
#include <tuple>
#include <limits>

namespace Math
{
    template<class T>
    static constexpr T PI = T(3.1415926535897932385);  // variable template

    template<class T>
    static constexpr T EPSILON = std::numeric_limits<T>::epsilon();

    template<class T>
    T Sign(T value)
    {
        return value < T(0) ? T(-1) : T(1);
    }

    template<class T>
    T Sign0(T value)
    {
        if( value > T(0) )
        {
            return T(1);
        }
        else if( value < T(0) )
        {
            return T(-1);
        }
        else
        {
            return T(0);
        }
    }

    template<class T>
    T Max(const T l, const T r)
    {
        return (l > r) ? l : r;
    }

    template<class T>
    T Min(const T l, const T r)
    {
        return (l > r) ? r : l;
    }

    template<class T>
    T Ceil(const T Val)
    {
        return std::ceil( Val );
    }

    template<class T>
    T Floor(const T Val)
    {
        return std::floor( Val );
    }

    template<class T>
    T Round(const T Val)
    {
        return std::round( Val );
    }

    template<class T>
    T Clamp(T Val, const T Min, const T Max)
    {
        if( Val < Min ) Val = Min;
        if( Val > Max ) Val = Max;
        return Val;
    }

    template<class T>
    T Clamp01(T Val)
    {
        return Clamp(Val, T(0), T(1));
    }

    template<class T>
    T FMod(const T a, const T b)
    {
        return std::fmod(a, b);
    }

    // Wrap Val To <0, Max)
    template<class T>
    T Repeat(T Val, T Max)
    {
        return FMod(Val, Max);
    }

    // Wrap Val To <Min, Max)
    template<class T>
    T Wrap(T Val, T Min, T Max)
    {
        if (Val >= Min && Val < Max) return Val;

        Max = Max - Min;
        T Offset = Min;
        Min = T(0);
        Val = Val - Offset;

        for (; Val < T(0); Val += Max);
        for (; Val >= Max; Val -= Max);
        return Val + Offset;
    }

    // Wrap Angle to <0, 359>
    template<class T>
    T WrapAngle(T Angle)
    {
        for (; Angle < T(0); Angle += T(360));
        for (; Angle >= T(360); Angle -= T(360));
        return Angle;
    }

    template<class T>
    T DegToRad(const T r)
    {
        return static_cast<T>(r * 0.01745329252f);
    }

    template<class T>
    T RadToDeg(const T r)
    {
        return static_cast<T>(r * 57.295779513f);
    }

    template<class T>
    T Sqrt(const T r)
    {
        return std::sqrt(r);
    }

    template<class T>
    T InvSqrt(const T r)
    {
        return T(1) / Sqrt(r);
    }

    template<class T>
    T Cbrt(const T r)
    {
        return std::cbrt(r);
    }

    template<class T>
    T Pow(const T r, const T t)
    {
        return std::pow(r, t);
    }

    template<class T>
    T Abs(const T r)
    {
        if( r < T(0) ) 
        {
            return -r;
        }
        else 
        {
            return r;
        }
    }

    template<class T>
    T Log(const T r)
    {
        return std::log(r);
    }

    template<class T>
    T Log2(const T r)
    {
        return std::log2(r);
    }

    template<class T>
    T Exp(const T r)
    {
        return std::exp(r);
    }

    template<class T>
    T Sin(const T r)
    {
        return std::sin(r);
    }

    template<class T>
    T Cos(const T r)
    {
        return std::cos(r);
    }

    template<class T>
    T Tan(const T r)
    {
        return std::tan(r);
    }

    template<class T>
    T Asin(const T r)
    {
        return std::asin(r);
    }

    template<class T>
    T Acos(const T r)
    {
        return std::acos(r);
    }

    template<class T>
    T Atan(const T r)
    {
        return std::atan(r);
    }

    template<class T>
    T Hypot(const T x, const T y)
    {
        return std::hypot(x, y);
    }

    template<class T>
    T Atan2(const T y, const T x)
    {
        return std::atan2(y, x);
    }

    template<class T>
    std::tuple<T, T> Modf(const T Val)
    {
        T ig = T(0);
        T fr = std::modf( Val, &ig );
        return {ig, fr};
    }

    template<class T>
    T NextPoW2(T Arg)
    {
        return Pow( T(2), Ceil(Log2(Arg)) );
    }

    template<class T>
    bool IsPoW2(T Arg)
    {
        T x = Log2(Arg), ig, fr;
        std::tie(ig, fr) = Modf( x );
        fr *= T(10000000);
        return static_cast<int>(fr) == 0;
    }

    template<class T>
    T Lerp(const T a, const T b, const T t)
    {
        return a + (b-a)*t;
    }

    // Cosine Interpolation
    // https://codeplea.com/simple-interpolation
    template<class T>
    T CosineIp(const T a, const T b, const T t)
    {
        return Lerp(a, b, -Cos(PI<T>*t)/T(2) + T(0.5) );
    }

    // Smooth Interpolation
    // https://codeplea.com/simple-interpolation
    template<class T>
    T SmothIp(const T a, const T b, const T t)
    {
        return Lerp(a, b, (t*t)*(T(3)-T(2)*t) );
    }

    // Acceleration Interpolation
    // https://codeplea.com/simple-interpolation
    template<class T>
    T AccelerationIp(const T a, const T b, const T t)
    {
        return Lerp(a, b, t*t );
    }

    // Deceleration Interpolation
    // https://codeplea.com/simple-interpolation
    template<class T>
    T DecelerationIp(const T a, const T b, const T t)
    {
        T tt = T(1)-t;
        return Lerp(a, b, T(1)-(tt*tt) );
    }

    // Min, Max: Min < Max
    int Random(const int, const int);

    // Min, Max: Min < Max
    float Random(const float, const float);
}