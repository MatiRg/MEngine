#include "Vector4.hpp"

namespace Math
{
    Vector4 Normalize(const Vector4& a)
    {
        return glm::normalize(a);
    }

    float Distance(const Vector4& a, const Vector4& b)
    {
        return glm::distance(a, b);
    }

    float DistanceSquared(const Vector4& a, const Vector4& b)
    {
        return glm::distance2(a, b);
    }

    float Length(const Vector4& a)
    {
        return glm::length(a);
    }

    float LengthSquared(const Vector4& a)
    {
        return glm::length2(a);
    }

    float DotProduct(const Vector4& a, const Vector4& b)
    {
        return glm::dot(a, b);
    }

    Vector4 MoveTowards(const Vector4& Now, const Vector4& Target, const float Delta)
    {
        Vector4 Tmp = Target - Now;
        float Distance = Length(Tmp);
        if( Distance <= Delta || glm::epsilonEqual(Distance, 0.0f, 0.000001f) )
        {
            return Target;
        }
        return Now + (Tmp / Distance) * Delta;
    }

    Vector4 Lerp(const Vector4& a, const Vector4& b, const float t)
    {
        return glm::mix(a, b, t);
    }

    Vector4 Max(const Vector4& a, const Vector4& b)
    {
        return glm::max(a, b);
    }

    Vector4 Min(const Vector4& a, const Vector4& b)
    {
        return glm::min(a, b);
    }

    const float* ToArray(const Vector4& a)
    {
        return glm::value_ptr(a);
    }
}

std::ostream& operator<<(std::ostream& Stream, const Vector4& Other)
{
    Stream << Other.x << ", " << Other.y << ", " << Other.z << ", " << Other.w;
    return Stream;
}