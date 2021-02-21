#include "Vector3.hpp"

namespace Math
{
    Vector3 Normalize(const Vector3& a)
    {
        return glm::normalize(a);
    }

    float Distance(const Vector3& a, const Vector3& b)
    {
        return glm::distance(a, b);
    }

    float DistanceSquared(const Vector3& a, const Vector3& b)
    {
        return glm::distance2(a, b);
    }

    float Length(const Vector3& a)
    {
        return glm::length(a);
    }

    float LengthSquared(const Vector3& a)
    {
        return glm::length2(a);
    }

    float DotProduct(const Vector3& a, const Vector3& b)
    {
        return glm::dot(a, b);
    }

    Vector3 CrossProduct(const Vector3& a, const Vector3& b)
    {
        return glm::cross(a, b);
    }

    float ScalarTripleProduct(const Vector3& a, const Vector3& b, const Vector3& c)
    {
        return DotProduct( a, CrossProduct(b, c) );
    }

    Vector3 TripleProduct(const Vector3& a, const Vector3& b, const Vector3& c)
    {
        return CrossProduct(a, CrossProduct(b, c));
    }

    Vector3 Reflect(const Vector3& Dir, const Vector3& Norm)
    {
        return glm::reflect(Dir, Norm);
    }

    Vector3 Project(const Vector3& Vc, const Vector3& Norm)
    {
        return glm::proj(Vc, Norm);
    }

    // Vectors Normalized
    float Angle(const Vector3& From, const Vector3& To)
    {
        return glm::degrees(glm::angle(From, To));
    }

    // Vectors Normalized
    float SignedAngle(const Vector3& From, const Vector3& To, const Vector3& Axis)
    {
        return glm::degrees(glm::orientedAngle(From, To, Axis));
    }

    Vector3 MoveTowards(const Vector3& Now, const Vector3& Target, const float Delta)
    {
        Vector3 Tmp = Target - Now;
        float Distance = Length(Tmp);
        if( Distance <= Delta || glm::epsilonEqual(Distance, 0.0f, 0.000001f) )
        {
            return Target;
        }
        return Now + (Tmp / Distance) * Delta;
    }

    Vector3 Lerp(const Vector3& a, const Vector3& b, const float t)
    {
        return glm::mix(a, b, t);
    }

    Vector3 Max(const Vector3& a, const Vector3& b)
    {
        return glm::max(a, b);
    }

    Vector3 Min(const Vector3& a, const Vector3& b)
    {
        return glm::min(a, b);
    }

    Vector2 ToXZ(const Vector3& a)
    {
        return { a.x, a.z };
    }

    Vector2 ToXY(const Vector3& a)
    {
        return { a.x, a.y };
    }

    Vector2 ToYZ(const Vector3& a)
    {
        return { a.y, a.z };
    }

    const float* ToArray(const Vector3& a)
    {
        return glm::value_ptr(a);
    }
}

template<class T>
std::ostream& operator<<(std::ostream& Stream, const Vector3& Other)
{
    Stream << Other.x << ", " << Other.y << ", " << Other.z;
    return Stream;
}

template<class T>
std::ostream& operator<<(std::ostream& Stream, const IntVector3& Other)
{
    Stream << Other.x << ", " << Other.y << ", " << Other.z;
    return Stream;
}