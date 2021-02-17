#pragma once
#include <cmath>
#include <tuple>
#include <limits>
#include <vector>

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

    //! Not Best - https://randomascii.wordpress.com/2012/02/25/comparing-floating-point-numbers-2012-edition/
    template<class T>
    bool IsEqual(const T a, const T b, const T Eps)
    {
        return Abs(a - b) <= Eps;
    }

    //! Value, Old Min, Old Max, New Min, New Max
    template<class T>
    T ReMap(const T Value, const T Start1, const T Stop1, const T Start2, const T Stop2)
    {
        return Start2 + (Stop2 - Start2) * ((Value - Start1) / (Stop1 - Start1));
    }

    template<class T>
    struct TGradientKey
    {
        using Type = T;
        Type Value;
        float Time;

        TGradientKey(const Type& aValue, const float aTime) :
            Value(aValue),
            Time(aTime)
        {
        }
    };

    template<class T>
    class TGradient
    {
    public:
        using ValueType = T;
        using KeyType = TGradientKey<T>;
    public:
        TGradient() = default;
        ~TGradient() = default;

        //! Use unique t values
        void AddKey(const ValueType& Value, const float t)
        {
            Keys.emplace_back(Value, t);
            std::sort(Keys.begin(), Keys.end(), [](const KeyType& a, const KeyType& b) {
                return a.Time < b.Time;
            });
        }

        ValueType Evaluate(const float t) const
        {
            // https://answers.unity.com/questions/1703579/how-does-gradientevaluate-work-internally.html
            std::size_t Size = Keys.size();
            if (!Size)
            {
                return {};
            }
            if (Size == 1u)
            {
                return Keys[0u].Value;
            }
            // Border Values
            if (t < Keys[0u].Time || IsEqual(Keys[0u].Time, t, 0.000001f))
            {
                return Keys[0u].Value;
            }
            if (t > Keys[Size - 1u].Time || IsEqual(Keys[Size - 1u].Time, t, 0.000001f))
            {
                return Keys[Size - 1u].Value;
            }
            // Rest
            for (std::size_t i = 0; i < Size - 1u; ++i)
            {
                KeyType StartKey = Keys[i];
                KeyType EndKey = Keys[i + 1u];
                if (t < EndKey.Time)
                {
                    float tt = Clamp01((t - StartKey.Time) / (EndKey.Time - StartKey.Time));
                    return Lerp(StartKey.Value, EndKey.Value, tt);
                }
            }
            return Keys[Size - 1u].Value;
        }
    private:
        std::vector<KeyType> Keys;
    };

    // PID
    // https://forum.unity.com/threads/rigidbody-lookat-torque.146625/#post-1005645
    template<class T>
    class TPID
    {
    public:
        using Type = T;
    public:
        TPID() = default;
        TPID(const float aKp, const float aKi, const float aKd) :
            Kp(aKp),
            Ki(aKi),
            Kd(aKd)
        {
        }
        ~TPID() = default;

        Type Evaluate(const Type& Now, const Type& Target, const float DT)
        {
            return Evaluate(Now - Target, DT); // Or Target - Now ?
        }

        Type Evaluate(const Type& Error, const float DT)
        {
            I += Error * DT;
            Type D = (Error - PreviousError) / DT;
            PreviousError = Error;
            return Error * Kp + I * Ki + D * Kd;
            /*Type P = Now - Target;
            I += P * DT;
            Type D = (P - PreviousError) / DT;
            PreviousError = P;
            return P*Kp + I*Ki + D*Kd;*/
        }
    public:
        float Kp = 1.0f;
        float Ki = 0.0f;
        float Kd = 0.1f;
    private:
        Type PreviousError = {};
        Type I = {};
    };

    template<class T>
    T MoveTowards(const T Now, const T Target, const T Delta)
    {
        T Tmp = Target - Now;
        if (Abs(Tmp) <= Delta)
        {
            return Target;
        }
        return Now + Sign(Tmp) * Delta;
    }

    // Min, Max: Min < Max
    int Random(const int, const int);

    // Min, Max: Min < Max
    float Random(const float, const float);
}
