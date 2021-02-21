#include "Vector2.hpp"

namespace Math
{
    Vector2 Normalize(const Vector2& a)
    {
        float L = Length(a);
        if( L )
        {
            return {a.x/L, a.y/L};
        }
        return a;
    }

    float Distance(const Vector2& a, const Vector2& b)
    {
        return Length(a-b);
    }

    float DistanceSquared(const Vector2& a, const Vector2& b)
    {
        return LengthSquared(a-b);
    }

    float Length(const Vector2& a)
    {
        return Sqrt( LengthSquared(a) );
    }

    float LengthSquared(const Vector2& a)
    {
        return a.x*a.x + a.y*a.y;
    }

    float DotProduct(const Vector2& a, const Vector2& b)
    {
        return a.x*b.x + a.y*b.y;
    }

    float CrossProduct(const Vector2& a, const Vector2& b)
    {
        return a.x * b.y - a.y * b.x;
    }

    float Angle(const Vector2& From, const Vector2& To)
    {
        float a = Acos(Clamp(DotProduct(From, To), -1.0f, 1.0f));
        return RadToDeg(a);
    }

    Vector2 MoveTowards(const Vector2& Now, const Vector2& Target, const float Delta)
    {
        Vector2 Tmp = Target - Now;
        float Distance = Length(Tmp);
        if( Distance <= Delta || IsEqual(Distance, 0.0f, 0.000001f) )
        {
            return Target;
        }
        return Now + (Tmp / Distance) * Delta;
    }

    Vector2 Lerp(const Vector2& a, const Vector2& b, const float t)
    {
        return {Lerp(a.x, b.x, t), Lerp(a.y, b.y, t)};
    }

    Vector2 Max(const Vector2& a, const Vector2& b)
    {
        return {Max(a.x, b.x), Max(a.y, b.y)};
    }

    Vector2 Min(const Vector2& a, const Vector2& b)
    {
        return {Min(a.x, b.x), Min(a.y, b.y)};
    }
}

std::ostream& operator<<(std::ostream& Stream, const Vector2& Other)
{
    Stream << Other.x << ", " << Other.y;
    return Stream;
}

std::ostream& operator<<(std::ostream& Stream, const IntVector2& Other)
{
    Stream << Other.x << ", " << Other.y;
    return Stream;
}