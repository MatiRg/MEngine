#include "Functions.hpp"
#include <cmath>
#include <random>

namespace Math
{
    //! Not Best - https://randomascii.wordpress.com/2012/02/25/comparing-floating-point-numbers-2012-edition/
    bool IsEqual(const float a, const float b, const float Eps)
    {
        return std::abs(a - b) <= Eps;
    }

    float Sign(const float value)
    {
        if( value > 0.0f )
        {
            return 1.0f;
        }
        else if( value < 0.0f )
        {
            return -1.0f;
        }
        else
        {
            return 0.0f;
        }
    }

    float Max(const float a, const float b)
    {
        return (a > b) ? a : b;
    }

    float Min(const float a, const float b)
    {
        return (a < b) ? a : b;
    }

    float Ceil(const float Val)
    {
        return std::ceil( Val );
    }

    float Floor(const float Val)
    {
        return std::floor( Val );
    }

    float Round(const float Val)
    {
        return std::round( Val );
    }

    float Clamp(const float Val, const float Min, const float Max)
    {
        if( Val < Min ) return Min;
        if( Val > Max ) return Max;
        return Val;
    }

    float Clamp01(const float Val)
    {
        return Clamp(Val, 0.0f, 1.0f);
    }

    float Mod(const float a, const float b)
    {
        return std::fmod(a, b);
    }

    // Wrap Val To <0, Max)
    float Repeat(const float Val, const float Max)
    {
        return Mod(Val, Max);
    }

    // Wrap Val To <Min, Max)
    float Wrap(float Val, float Min, float Max)
    {
        if (Val >= Min && Val < Max) return Val;

        Max = Max - Min;
        float Offset = Min;
        Min = 0.0f;
        Val = Val - Offset;

        for (; Val < 0.0f; Val += Max);
        for (; Val >= Max; Val -= Max);
        return Val + Offset;
    }

    // Wrap Angle to <0, 359>
    float WrapAngle(float Angle)
    {
        for (; Angle < 0.0f; Angle += 360.0f);
        for (; Angle >= 360.0f; Angle -= 360.0f);
        return Angle;
    }

    float DeltaAngle(float Now, float Target)
    {
        float Tmp = Repeat(Target - Now, 360.0f);
        if (Tmp > 180.0f)
        {
            Tmp -= 360.0f;
        }
        return Tmp;
    }

    float DegToRad(const float r)
    {
        return r * 0.01745329252f;
    }

    float RadToDeg(const float r)
    {
        return r * 57.295779513f;
    }

    float Sqrt(const float r)
    {
        return std::sqrt(r);
    }

    float InvSqrt(const float r)
    {
        return 1.0f / Sqrt(r);
    }

    float Pow(const float r, const float t)
    {
        return std::pow(r, t);
    }

    float Abs(const float r)
    {
        return std::abs(r);
    }

    float Log(const float r)
    {
        return std::log(r);
    }

    float Log2(const float r)
    {
        return std::log2(r);
    }

    float Exp(const float r)
    {
        return std::exp(r);
    }

    // Radians
    float Sin(const float r)
    {
        return std::sin(r);
    }

    // Radians
    float Cos(const float r)
    {
        return std::cos(r);
    }

    // Radians
    float Tan(const float r)
    {
        return std::tan(r);
    }

    // Radians
    float Asin(const float r)
    {
        return std::asin(r);
    }

    // Radians
    float Acos(const float r)
    {
        return std::acos(r);
    }

    // Radians
    float Atan(const float r)
    {
        return std::atan(r);
    }

    // Radians
    float Atan2(const float y, const float x)
    {
        return std::atan2(y, x);
    }

    float Lerp(const float a, const float b, const float t)
    {
        return a + (b-a)*t;
    }

    // Cosine Interpolation
    // https://codeplea.com/simple-interpolation
    float CosineIp(const float a, const float b, const float t)
    {
        return Lerp(a, b, -Cos(PI * t) / 2.0f + 0.5f);
    }

    // Smooth Interpolation
    // https://codeplea.com/simple-interpolation
    float SmothIp(const float a, const float b, const float t)
    {
        return Lerp(a, b, (t * t) * (3.0f - 2.0f * t));
    }

    // Acceleration Interpolation
    // https://codeplea.com/simple-interpolation
    float AccelerationIp(const float a, const float b, const float t)
    {
        return Lerp(a, b, t * t);
    }

    // Deceleration Interpolation
    // https://codeplea.com/simple-interpolation
    float DecelerationIp(const float a, const float b, const float t)
    {
        float tt = 1.0f - t;
        return Lerp(a, b, 1.0f - (tt * tt));
    }

    //! Value, Old Min, Old Max, New Min, New Max
    float ReMap(const float Value, const float Start1, const float Stop1, const float Start2, const float Stop2)
    {
        return Start2 + (Stop2 - Start2) * ((Value - Start1) / (Stop1 - Start1));
    }

    float MoveTowards(const float Now, const float Target, const float Delta)
    {
        float Tmp = Target - Now;
        if (Abs(Tmp) <= Delta)
        {
            return Target;
        }
        return Now + Sign(Tmp) * Delta;
    }

    namespace
    {
        static std::random_device RandomDevice;
        static std::mt19937 Generator(RandomDevice());
    }

    int Random(const int a, const int b)
    {
        std::uniform_int_distribution<int> Distribution(Min(a, b), Max(a, b));
        return Distribution(Generator);
    }

    float Random(const float a, const float b)
    {
        std::uniform_real_distribution<float> Distribution(Min(a, b), Max(a, b));
        return Distribution(Generator);
    }
}