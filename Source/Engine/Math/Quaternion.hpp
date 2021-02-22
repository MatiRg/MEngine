#pragma once
#include "Functions.hpp"
#include "Vector2.hpp"
#include "Vector3.hpp"
#include "Matrix4.hpp"

class Quaternion
{
public:
    Quaternion()
    {
        SetIdentity();
    }

    // Pitch Yaw Roll in Degrees
    Quaternion(const float ax, const float ay, const float az)
    {
        FromEulerAngles(ax, ay, az);
    }

    // Pitch Yaw Roll in Degrees
    Quaternion(const Vector3& v)
    {
        FromEulerAngles(v);
    }

    /// Degrees, AxisAngle, Axis normalized
    Quaternion(const Vector3& v, const float angle)
    {
        FromAxisAngle(v, angle);
    }

    /// Rotation from a to b, Normalized
    Quaternion(const Vector3& a, const Vector3& b)
    {
        FromTo(a, b);
    }

    Quaternion(const Matrix4& v)
    {
        FromMatrix(v);
    }

    /// Construct from components
    Quaternion(const float ax, const float ay, const float az, const float aw):
        x(ax),
        y(ay),
        z(az),
        w(aw)
    {
    }

    Quaternion(const Quaternion& D):
        x(D.x),
        y(D.y),
        z(D.z),
        w(D.w)
    {
    }

    ~Quaternion()
    {
    }

    void SetIdentity();
    //
    Quaternion Conjugate() const;
    // Length(magnitude) of Quaternion
    float Length() const;
    // Length(magnitude) of Quaternion Squared
    float LengthSquared() const;
    //
    void Normalize();
    //
    float DotProduct(const Quaternion& Other) const;
    //
    Quaternion Inverse() const;
    //
    void Invert();
    // Degrees
    float Angle(const Quaternion& Other) const;

    /// Rotation from a to b, Normalized
    void FromTo(const Vector3& a, const Vector3& b);
    // Pitch Yaw Roll in Degrees
    Vector3 ToEulerAngles() const;
    // Pitch Yaw Roll in Degrees
    void FromEulerAngles(const float ax, const float ay, const float az) { FromEulerAngles({ax, ay, az}); }
    // Pitch Yaw Roll in Degrees
    void FromEulerAngles(const Vector3& v);
    // Degrees
    void FromAxisAngle(const Vector3& v, const float angle);
    //
    Matrix4 ToMatrix() const;
    //
    void FromMatrix(const Matrix4&);

    float& operator[](std::size_t idx)
    {
        return (&x)[idx];
    }

    const float& operator[](std::size_t idx) const
    {
        return (&x)[idx];
    }

    Quaternion& operator=(const Quaternion& D)
    {
        if( this == &D )
        {
            return *this;
        }
        x = D.x;
        y = D.y;
        z = D.z;
        w = D.w;
        return *this;
    }

    Quaternion operator*(const float Other) const
    {
        return Quaternion( x * Other, y * Other, z * Other, w * Other );
    }

    Quaternion& operator*=(const float Other)
    {
        x *= Other;
        y *= Other;
        z *= Other;
        w *= Other;
        return *this;
    }

    Vector3 operator*(const Vector3& Other) const
    {
        Vector3 QuatVector(x, y, z);
        Vector3 uv = Math::CrossProduct(QuatVector, Other);
        Vector3 uuv = Math::CrossProduct(QuatVector, uv);

        return Other + (uv * w + uuv) * 2.0f;
    }

	Quaternion operator*(const Quaternion& D) const
	{
		Quaternion Q;
		Q.w = w*D.w - x*D.x - y*D.y - z*D.z;
		Q.x = w*D.x + x*D.w + y*D.z - z*D.y;
		Q.y = w*D.y + y*D.w + z*D.x - x*D.z;
		Q.z = w*D.z + z*D.w + x*D.y - y*D.x;
		return Q;
    }

    Quaternion& operator *=(const Quaternion& D)
    {
        Quaternion Q = *this;
        w = Q.w*D.w - Q.x*D.x - Q.y*D.y - Q.z*D.z;
        x = Q.w*D.x + Q.x*D.w + Q.y*D.z - Q.z*D.y;
        y = Q.w*D.y + Q.y*D.w + Q.z*D.x - Q.x*D.z;
        z = Q.w*D.z + Q.z*D.w + Q.x*D.y - Q.y*D.x;
        return *this;
    }

    Quaternion operator/(const float Other) const
    {
        return Quaternion( x / Other, y / Other, z / Other, w / Other );
    }

    Quaternion operator+(const Quaternion& Other) const
    {
        return Quaternion( x + Other.x, y + Other.y, z + Other.z, w + Other.w );
    }

    Quaternion& operator+=(const Quaternion& Other)
    {
        x += Other.x;
        y += Other.y;
        z += Other.z;
        w += Other.w;
        return *this;
    }

    Quaternion operator-() const
    {
        return Quaternion(-x, -y, -z, -w);
    }

    Quaternion operator-(const Quaternion& Other) const
    {
        return Quaternion( x - Other.x, y - Other.y, z - Other.z, w - Other.w );
    }

    Quaternion& operator-=(const Quaternion& Other)
    {
        x -= Other.x;
        y -= Other.y;
        z -= Other.z;
        w -= Other.w;
        return *this;
    }

    static Quaternion IDENTITY;
public:
    float x, y, z, w;
};

namespace Math
{
    Quaternion Conjugate(const Quaternion& Qt);
    float Length(const Quaternion& Qt);
    float LengthSquared(const Quaternion& Qt);
    Quaternion Normalize(const Quaternion& Qt);
    float DotProduct(const Quaternion& v1, const Quaternion& v2);
    Quaternion Inverse(const Quaternion& Qt);

    // Converts Quaternion to Euler Angles in Degrees
    // Pitch Yaw Roll, Degrees
    Vector3 ToEulerAngles(const Quaternion& Qt);
    // Pitch Yaw Roll, Degrees
    Quaternion FromEulerAngles(const float Pitch, const float Yaw, const float Roll);
    // Pitch Yaw Roll, Degrees
    Quaternion FromEulerAngles(const Vector3& Angles);
    // Degrees, Normalized Axis
    Quaternion FromAxisAngle(const Vector3& Axis, const float Angle);
    // From To Rotation, Vectors Normalized
    Quaternion FromToRotation(const Vector3& a, const Vector3& b);
    // Axis Normalized
    Quaternion LookRotation(const Vector3& Direction, const Vector3& Up);

    // In Degrees
    float Angle(const Quaternion& a, const Quaternion& b);
    Quaternion RotateTowards(const Quaternion& From, const Quaternion& To, float Delta);

    // Normalized
    Quaternion Lerp(const Quaternion& a, const Quaternion& b, const float t);
    // Normalized
    Quaternion SLerp(const Quaternion& a, const Quaternion& b, const float t);

    // Rotation From Quaternion
    Matrix4 ToMatrix4(const Quaternion& Qt);
    //
    Matrix4 Transform(const Vector3& Position, const Quaternion& Rotation, const Vector3& Scale);
    //
    Matrix4 Rotation(const Quaternion& Qt);

    // In Degrees
    Matrix4 Transform2D(const Vector2& Origin, const float Angle, const Vector2& Scale);
}

std::ostream& operator<<(std::ostream& Stream, const Quaternion& Other);
