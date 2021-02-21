#include "Quaternion.hpp"

namespace Math
{
    Quaternion Conjugate(const Quaternion& Qt)
    {
        return glm::conjugate(Qt);
    }

    float Length(const Quaternion& Qt)
    {
        return glm::length(Qt);
    }

    float LengthSquared(const Quaternion& Qt)
    {
        return glm::length2(Qt);
    }

    Quaternion Normalize(const Quaternion& Qt)
    {
        return glm::normalize(Qt);
    }

    float DotProduct(const Quaternion& v1, const Quaternion& v2)
    {
        return glm::dot(v1, v2);
    }

    Quaternion Inverse(const Quaternion& Qt)
    {
        return glm::inverse(Qt);
    }

    // Converts Quaternion to Euler Angles in Degrees
    // Pitch Yaw Roll
    Vector3 ToEulerAngles(const Quaternion& Qt)
    {
        return glm::degrees(glm::eulerAngles(Qt));
    }

    // Pitch Yaw Roll
    Quaternion FromEulerAngles(const float Pitch, const float Yaw, const float Roll)
    {
        return FromEulerAngles({Pitch, Yaw, Roll});
    }

    // Pitch Yaw Roll
    Quaternion FromEulerAngles(const Vector3& Angles)
    {
        return { glm::radians(Angles) };
    }

    // Degrees, Normalized Axis
    Quaternion FromAxisAngle(const Vector3& Axis, const float Angle)
    {
        return glm::angleAxis(glm::radians(Angle), Axis);
    }

    // From To Rotation, Vectors Normalized
    Quaternion FromToRotation(const Vector3& a, const Vector3& b)
    {
        return { a, b };
    }

    // Axis Normalized
    Quaternion LookRotation(const Vector3& Direction, const Vector3& Up)
    {
        return glm::quatLookAt(Direction, Up);
    }

    float Angle(const Quaternion& a, const Quaternion& b)
    {
        float Dot = DotProduct(a, b);
        return glm::degrees(glm::acos(glm::min(glm::abs(Dot), 1.0f)) * 2.0f);
    }

    Quaternion RotateTowards(const Quaternion& From, const Quaternion& To, float Delta)
    {
        float a = Angle(From, To);
        if( glm::epsilonEqual(a, 0.0f, 0.00000001f) )
        {
            return To;
        }
        //
		float t = glm::min(Delta/a, 1.0f);
        return SLerp(From, To, t);
    }

    // Rotation From Quaternion
    Matrix4 ToMatrix4(const Quaternion& Qt)
    {
        return glm::mat4_cast(Qt);
    }

    // Normalized
    Quaternion Lerp(const Quaternion& a, const Quaternion& b, const float t)
    {
        return glm::lerp(a, b, t);
    }

    // Normalized
    Quaternion SLerp(const Quaternion& a, const Quaternion& b, const float t)
    {
        return glm::slerp(a, b, t);
    }

    Matrix4 Transform(const Vector3& p, const Quaternion& r, const Vector3& s)
    {
        /*
        Matrix4 Tmp = MATRIX4_IDENTITY;
        Tmp = glm::translate(Tmp, p);
        Tmp = Tmp*glm::mat4_cast(r);
        Tmp = glm::scale(Tmp, s);
        return Tmp;
        */
        return Scale(s) * ToMatrix4(r) * Translation(p);
        //return Translation(p) * ToMatrix4(r) * Scale(s);
    }

    // In Degrees
    Matrix4 Transform2D(const Vector2& o, const float a, const Vector2& s)
    {
        return Translation(-o.x, -o.y, 0.0f) * Scale(s.x, s.y, 1.0f) * 
            ToMatrix4( FromAxisAngle({0.0f, 0.0f, 1.0f}, a) ) * Translation(o.x, o.y, 1.0f);
    }
}

std::ostream& operator<<(std::ostream& Stream, const Quaternion& Other)
{
    Stream << Other.x << ", " << Other.y << ", " << Other.z << ": " << Other.w;
    return Stream;
}
