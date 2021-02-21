#include "Vector2.hpp"

namespace Math
{
    Vector2 Normalize(const Vector2& a)
    {
        return glm::normalize(a);
    }

    float Distance(const Vector2& a, const Vector2& b)
    {
        return glm::distance(a, b);
    }

    float DistanceSquared(const Vector2& a, const Vector2& b)
    {
        return glm::distance2(a, b);
    }

    float Length(const Vector2& a)
    {
        return glm::length(a);
    }

    float LengthSquared(const Vector2& a)
    {
        return glm::length2(a);
    }

    float DotProduct(const Vector2& a, const Vector2& b)
    {
        return glm::dot(a, b);
    }

    float CrossProduct(const Vector2& a, const Vector2& b)
    {
        return a.x * b.y - a.y * b.x;
    }

    float Angle(const Vector2& From, const Vector2& To)
    {
        return glm::degrees(glm::angle(From, To));
    }

    Vector2 MoveTowards(const Vector2& Now, const Vector2& Target, const float Delta)
    {
        Vector2 Tmp = Target - Now;
        float Distance = Length(Tmp);
        if( Distance <= Delta || glm::epsilonEqual(Distance, 0.0f, 0.000001f) )
        {
            return Target;
        }
        return Now + (Tmp / Distance) * Delta;
    }

    Vector2 Lerp(const Vector2& a, const Vector2& b, const float t)
    {
        return glm::mix(a, b, t);
    }

    Vector2 Max(const Vector2& a, const Vector2& b)
    {
        return glm::max(a, b);
    }

    Vector2 Min(const Vector2& a, const Vector2& b)
    {
        return glm::min(a, b);
    }

    const float* ToArray(const Vector2& a)
    {
        return glm::value_ptr(a);
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