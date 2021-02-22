#include "Vector3.hpp"

Vector3 Vector3::Normalized() const
{
    return Math::Normalize(*this);
}

void Vector3::Normalize()
{
    *this = Math::Normalize(*this);
}

float Vector3::Distance(const Vector3& b) const
{
    return Math::Distance(*this, b);
}

float Vector3::DistanceSquared(const Vector3& b) const
{
    return Math::DistanceSquared(*this, b);
}

float Vector3::Length() const
{
    return Math::Length(*this);
}

float Vector3::LengthSquared() const
{
    return Math::LengthSquared(*this);
}

float Vector3::DotProduct(const Vector3& b) const
{
    return Math::DotProduct(*this, b);
}

Vector3 Vector3::CrossProduct(const Vector3& b) const
{
    return Math::CrossProduct(*this, b);
}

float Vector3::Angle(const Vector3& To) const
{
    return Math::Angle(*this, To);
}

float Vector3::SignedAngle(const Vector3& To, const Vector3& Axis) const
{
    return Math::SignedAngle(*this, To, Axis);
}

Vector3 Vector3::UP = { 0.0f, 1.0f, 0.0f };
Vector3 Vector3::DOWN = { 0.0f, -1.0f, 0.0f };
Vector3 Vector3::RIGHT = { 1.0f, 0.0f, 0.0f };
Vector3 Vector3::LEFT = { -1.0f, 0.0f, 0.0f };
Vector3 Vector3::FORWARD = { 0.0f, 0.0f, 1.0f };
Vector3 Vector3::BACK = { 0.0f, 0.0f, -1.0f };
Vector3 Vector3::ZERO = { 0.0f, 0.0f, 0.0f };
Vector3 Vector3::ONE = { 1.0f, 1.0f, 1.0f };

namespace Math
{
    Vector3 Normalize(const Vector3& a)
    {
        float L = Length(a);
        if( L )
        {
            return {a.x/L, a.y/L, a.z/L};
        }
        return a;
    }

    float Distance(const Vector3& a, const Vector3& b)
    {
        return Length(a-b);
    }

    float DistanceSquared(const Vector3& a, const Vector3& b)
    {
        return LengthSquared(a-b);
    }

    float Length(const Vector3& a)
    {
        return Sqrt( LengthSquared(a) );
    }

    float LengthSquared(const Vector3& a)
    {
        return a.x*a.x + a.y*a.y + a.z*a.z;
    }

    float DotProduct(const Vector3& a, const Vector3& b)
    {
        return a.x*b.x + a.y*b.y + a.z*b.z;
    }

    Vector3 CrossProduct(const Vector3& a, const Vector3& b)
    {
        return
        {
            a.y*b.z - a.z*b.y,
            a.z*b.x - a.x*b.z,
            a.x*b.y - a.y*b.x
        };
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
        return Dir - Norm * DotProduct(Norm, Dir) * 2.0f;
    }

    Vector3 Project(const Vector3& Vc, const Vector3& Norm)
    {
        float Mag = DotProduct(Norm, Norm);
        if (Mag < EPSILON)
        {
            return Vector3::ZERO;
        }
        else
        {
            float d = DotProduct(Vc, Norm);
            return 
            {
                Norm.x * d / Mag,
                Norm.y * d / Mag,
                Norm.z * d / Mag
            };
        }
    }

    // Vectors Normalized
    float Angle(const Vector3& From, const Vector3& To)
    {
        float a = Acos(Clamp(DotProduct(From, To), -1.0f, 1.0f));
        return RadToDeg(a);
    }

    // Vectors Normalized
    float SignedAngle(const Vector3& From, const Vector3& To, const Vector3& Axis)
    {
        float a = Angle(From, To);
        Vector3 cs = CrossProduct(From, To);
        float s = Sign(DotProduct(Axis, cs));
        return a * s;
    }

    Vector3 MoveTowards(const Vector3& Now, const Vector3& Target, const float Delta)
    {
        Vector3 Tmp = Target - Now;
        float Distance = Length(Tmp);
        if( Distance <= Delta || IsEqual(Distance, 0.0f, 0.000001f) )
        {
            return Target;
        }
        return Now + (Tmp / Distance) * Delta;
    }

    Vector3 Lerp(const Vector3& a, const Vector3& b, const float t)
    {
        return {Lerp(a.x, b.x, t), Lerp(a.y, b.y, t), Lerp(a.z, b.z, t)};
    }

    Vector3 Max(const Vector3& a, const Vector3& b)
    {
        return {Max(a.x, b.x), Max(a.y, b.y), Max(a.z, b.z)};
    }

    Vector3 Min(const Vector3& a, const Vector3& b)
    {
        return {Min(a.x, b.x), Min(a.y, b.y), Min(a.z, b.z)};
    }
}

template<class T>
std::ostream& operator<<(std::ostream& Stream, const Vector3& Other)
{
    Stream << Other.x << ", " << Other.y << ", " << Other.z;
    return Stream;
}
