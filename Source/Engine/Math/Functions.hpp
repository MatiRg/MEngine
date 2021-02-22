#pragma once
#include <limits>
#include <vector>
#include <algorithm>

namespace Math
{
    static constexpr float PI = 3.1415926535897932385f;
    static constexpr float EPSILON = std::numeric_limits<float>::epsilon();

    bool IsEqual(const float a, const float b, const float Eps);
    // Returns 1, 0, -1
    float Sign(const float value);

    float Max(const float a, const float b);
    float Min(const float a, const float b);

    float Ceil(const float Val);
    float Floor(const float Val);
    float Round(const float Val);

    float Clamp(const float Val, const float Min, const float Max);
    float Clamp01(const float Val);

    float Mod(const float a, const float b);

    // Wrap Val To <0, Max)
    float Repeat(const float Val, const float Max);
    // Wrap Val To <Min, Max)
    float Wrap(float Val, float Min, float Max);
    // Wrap Angle to <0, 359>
    float WrapAngle(float Angle);
    // Shortest Angle like Unity
    float DeltaAngle(float Now, float Target);

    float DegToRad(const float r);
    float RadToDeg(const float r);

    float Abs(const float r);

    float Sqrt(const float r);
    float InvSqrt(const float r);
    float Pow(const float r, const float t);
    float Log(const float r);
    float Log2(const float r);
    float Exp(const float r);

    // Radians
    float Sin(const float r);
    // Radians
    float Cos(const float r);
    // Radians
    float Tan(const float r);
    // Radians
    float Asin(const float r);
    // Radians
    float Acos(const float r);
    // Radians
    float Atan(const float r);
    // Radians
    float Atan2(const float y, const float x);

    float Lerp(const float a, const float b, const float t);
    // Cosine Interpolation
    float CosineIp(const float a, const float b, const float t);
    // Smooth Interpolation
    float SmothIp(const float a, const float b, const float t);
    // Acceleration Interpolation
    float AccelerationIp(const float a, const float b, const float t);
    // Deceleration Interpolation
    float DecelerationIp(const float a, const float b, const float t);

    //! Value, Old Min, Old Max, New Min, New Max
    float ReMap(const float Value, const float Start1, const float Stop1, const float Start2, const float Stop2);
    //
    float MoveTowards(const float Now, const float Target, const float Delta);

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
        }
    public:
        float Kp = 1.0f;
        float Ki = 0.0f;
        float Kd = 0.1f;
    private:
        Type PreviousError = {};
        Type I = {};
    };

    // Min, Max: Min < Max
    int Random(const int Min, const int Max);
    // Min, Max: Min < Max
    float Random(const float Min, const float Max);
}
