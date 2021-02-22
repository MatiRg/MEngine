#include "Vector4.hpp"

Vector4 Vector4::Normalized() const
{
    return Math::Normalize(*this);
}

void Vector4::Normalize()
{
    *this = Math::Normalize(*this);
}

float Vector4::Distance(const Vector4& b) const
{
    return Math::Distance(*this, b);
}

float Vector4::DistanceSquared(const Vector4& b) const
{
    return Math::DistanceSquared(*this, b);
}

float Vector4::Length() const
{
    return Math::Length(*this);
}

float Vector4::LengthSquared() const
{
    return Math::LengthSquared(*this);
}

float Vector4::DotProduct(const Vector4& b) const
{
    return Math::DotProduct(*this, b);
}

Vector4 Vector4::ZERO = { 0.0f, 0.0f, 0.0f, 0.0f };
Vector4 Vector4::ONE = { 1.0f, 1.0f, 1.0f, 1.0f };

namespace Math
{
    Vector4 Normalize(const Vector4& a)
    {
        float L = Length(a);
        if( L )
        {
            return {a.x/L, a.y/L, a.z/L, a.w/L};
        }
        return a;
    }

    float Distance(const Vector4& a, const Vector4& b)
    {
        return Length(a-b);
    }

    float DistanceSquared(const Vector4& a, const Vector4& b)
    {
        return LengthSquared(a-b);
    }

    float Length(const Vector4& a)
    {
        return Sqrt( LengthSquared(a) );
    }

    float LengthSquared(const Vector4& a)
    {
        return a.x*a.x + a.y*a.y + a.z*a.z + a.w*a.w;
    }

    float DotProduct(const Vector4& a, const Vector4& b)
    {
        return a.x*b.x + a.y*b.y + a.z*b.z + a.w*b.w;
    }

    Vector4 MoveTowards(const Vector4& Now, const Vector4& Target, const float Delta)
    {
        Vector4 Tmp = Target - Now;
        float Distance = Length(Tmp);
        if( Distance <= Delta || IsEqual(Distance, 0.0f, 0.000001f) )
        {
            return Target;
        }
        return Now + (Tmp / Distance) * Delta;
    }

    Vector4 Lerp(const Vector4& a, const Vector4& b, const float t)
    {
        return {Lerp(a.x, b.x, t), Lerp(a.y, b.y, t), Lerp(a.z, b.z, t), Lerp(a.w, b.w, t)};
    }

    Vector4 Max(const Vector4& a, const Vector4& b)
    {
        return {Max(a.x, b.x), Max(a.y, b.y), Max(a.z, b.z), Max(a.w, b.w)};
    }

    Vector4 Min(const Vector4& a, const Vector4& b)
    {
        return {Min(a.x, b.x), Min(a.y, b.y), Min(a.z, b.z), Min(a.w, b.w)};
    }
}

std::ostream& operator<<(std::ostream& Stream, const Vector4& Other)
{
    Stream << Other.x << ", " << Other.y << ", " << Other.z << ", " << Other.w;
    return Stream;
}