#include "Quaternion.hpp"

void Quaternion::SetIdentity()
{
    x = y = z = 0.0f;
    w = 1.0f;
}

Quaternion Quaternion::Conjugate() const
{
    return Math::Conjugate(*this);
}

float Quaternion::Length() const
{
    return Math::Length(*this);
}

float Quaternion::LengthSquared() const
{
    return Math::LengthSquared(*this);
}

void Quaternion::Normalize()
{
    *this = Math::Normalize(*this);
}

float Quaternion::DotProduct(const Quaternion& Other) const
{
    return Math::DotProduct(*this, Other);
}

Quaternion Quaternion::Inverse() const
{
    return Math::Inverse(*this);
}

void Quaternion::Invert()
{
    *this = Inverse();
}

float Quaternion::Angle(const Quaternion& Other) const
{
    return Math::Angle(*this, Other);
}

void Quaternion::FromTo(const Vector3& u, const Vector3& v)
{
    float norm_u_norm_v = Math::Sqrt(Math::LengthSquared(u)-Math::LengthSquared(v));
    float real_part = norm_u_norm_v + Math::DotProduct(u, v);
    Vector3 t;

	if(real_part < 1.e-6f * norm_u_norm_v)
    {
		real_part = 0;
		t = Math::Abs(u.x) > Math::Abs(u.z) ? Vector3(-u.y, u.x, 0.0f) : Vector3(0.0f, -u.z, u.y);
    }
    else
    {
       	t = Math::CrossProduct(u, v); 
    }
    *this = Math::Normalize(Quaternion{t.x, t.y, t.z, real_part});
}

Vector3 Quaternion::ToEulerAngles() const
{
    float Roll = Math::RadToDeg( Math::Atan2(2.0f * (x * y + w * z), w * w + x * x - y * y - z * z) );

    float yy = 2.0f * (y * z + w * x);
    float xx = w * w - x * x - y * y + z * z;

    float Pitch = 0.0f;
    if (Math::IsEqual(xx, 0.0f, Math::EPSILON) && Math::IsEqual(yy, 0.0f, Math::EPSILON))
    {
        Pitch = Math::RadToDeg( 2.0f * Math::Atan2(x, w) );
    }
    else
    {
        Pitch = Math::RadToDeg( Math::Atan2(yy, xx) );
    }

    float Yaw = Math::RadToDeg( Math::Asin(Math::Clamp(-2.0f * (x * z - w * y), -1.0f, 1.0f)) );

    return { Pitch, Yaw, Roll };
}

void Quaternion::FromEulerAngles(const Vector3& v)
{
    Vector3 eulerAngle{ Math::DegToRad(v.x)*0.5f, Math::DegToRad(v.y)*0.5f, Math::DegToRad(v.z)*0.5f};
	Vector3 c{Math::Cos(eulerAngle.x), Math::Cos(eulerAngle.y), Math::Cos(eulerAngle.z)};
	Vector3 s{Math::Sin(eulerAngle.x), Math::Sin(eulerAngle.y), Math::Sin(eulerAngle.z)};

	w = c.x * c.y * c.z + s.x * s.y * s.z;
	x = s.x * c.y * c.z - c.x * s.y * s.z;
	y = c.x * s.y * c.z + s.x * c.y * s.z;
	z = c.x * c.y * s.z - s.x * s.y * c.z;
}

void Quaternion::FromAxisAngle(const Vector3& v, const float angle)
{
    float a = Math::DegToRad(angle*0.5f);
    float s = Math::Sin(a);
    float c = Math::Cos(a);

    w = c;
    x = v.x*s;
    y = v.y*s;
    z = v.z*s;
}

Matrix4 Quaternion::ToMatrix() const
{
	Matrix4 D;

    float qxx(x * x);
    float qyy(y * y);
    float qzz(z * z);
    float qxz(x * z);
    float qxy(x * y);
    float qyz(y * z);
    float qwx(w * x);
    float qwy(w * y);
    float qwz(w * z);

    D[0] = 1.0f - 2.0f * (qyy + qzz);
    D[1] = 2.0f * (qxy + qwz);
    D[2] = 2.0f * (qxz - qwy);

    D[4] = 2.0f * (qxy - qwz);
    D[5] = 1.0f - 2.0f * (qxx + qzz);
    D[6] = 2.0f * (qyz + qwx);

    D[8] = 2.0f * (qxz + qwy);
    D[9] = 2.0f * (qyz - qwx);
    D[10] = 1.0f - 2.0f * (qxx + qyy);

	return D;
}

// ?
void Quaternion::FromMatrix(const Matrix4& D)
{
	float fourXSquaredMinus1 = D[0] - D[5] - D[10];
	float fourYSquaredMinus1 = D[5] - D[0] - D[10];
	float fourZSquaredMinus1 = D[10] - D[0] - D[5];
	float fourWSquaredMinus1 = D[0] + D[5] + D[10];

	int biggestIndex = 0;
	float fourBiggestSquaredMinus1 = fourWSquaredMinus1;
	if(fourXSquaredMinus1 > fourBiggestSquaredMinus1)
	{
		fourBiggestSquaredMinus1 = fourXSquaredMinus1;
		biggestIndex = 1;
	}
	if(fourYSquaredMinus1 > fourBiggestSquaredMinus1)
	{
		fourBiggestSquaredMinus1 = fourYSquaredMinus1;
		biggestIndex = 2;
	}
	if(fourZSquaredMinus1 > fourBiggestSquaredMinus1)
	{
		fourBiggestSquaredMinus1 = fourZSquaredMinus1;
		biggestIndex = 3;
	}

	float biggestVal = Math::Sqrt(fourBiggestSquaredMinus1 + 1.0f) * 0.5f;
	float mult = 0.25f / biggestVal;

	switch(biggestIndex)
	{
	case 0:
		*this = Quaternion((D[6] - D[9]) * mult, (D[8] - D[2]) * mult, (D[1] - D[4]) * mult, biggestVal);
        break;
	case 1:
		*this = Quaternion(biggestVal, (D[1] + D[4]) * mult, (D[8] + D[2]) * mult, (D[6] - D[9]) * mult);
        break;
	case 2:
		*this = Quaternion((D[1] + D[4]) * mult, biggestVal, (D[6] + D[9]) * mult, (D[8] - D[2]) * mult);
        break;
	case 3:
		*this = Quaternion((D[8] + D[2]) * mult, (D[6] + D[9]) * mult, biggestVal, (D[1] - D[4]) * mult);
        break;
	default:
		*this = Quaternion(0.0f, 0.0f, 0.0f, 1.0f);
        break;
	}
}

Quaternion Quaternion::IDENTITY = {};

namespace Math
{
    Quaternion Conjugate(const Quaternion& Qt)
    {
        return {-Qt.x, -Qt.y, -Qt.z, Qt.w};
    }

    float Length(const Quaternion& Qt)
    {
        return Sqrt( LengthSquared(Qt) );
    }

    float LengthSquared(const Quaternion& Qt)
    {
        return Qt.x*Qt.x + Qt.y*Qt.y + Qt.z*Qt.z + Qt.w*Qt.w;
    }

    Quaternion Normalize(const Quaternion& Qt)
    {
		float L = Length(Qt);
		if( IsEqual(L, 0.0f, Math::EPSILON) )
		{
            return Quaternion::IDENTITY;
        }
		return {Qt.x/L, Qt.y/L, Qt.z/L, Qt.w/L};
    }

    float DotProduct(const Quaternion& v1, const Quaternion& v2)
    {
        return v1.x*v2.x + v1.y*v2.y + v1.z*v2.z + v1.w*v2.w;
    }

    Quaternion Inverse(const Quaternion& Qt)
    {
        float L = LengthSquared(Qt);
        Quaternion C = Conjugate(Qt);
        return {C.x/L, C.y/L, C.z/L, C.w/L};
    }

    // Converts Quaternion to Euler Angles in Degrees
    // Pitch Yaw Roll
    Vector3 ToEulerAngles(const Quaternion& Qt)
    {
        return Qt.ToEulerAngles();
    }

    // Pitch Yaw Roll
    Quaternion FromEulerAngles(const float Pitch, const float Yaw, const float Roll)
    {
        return Quaternion(Pitch, Yaw, Roll);
    }

    // Pitch Yaw Roll
    Quaternion FromEulerAngles(const Vector3& Angles)
    {
        return Quaternion(Angles.x, Angles.y, Angles.z);
    }

    // Degrees, Normalized Axis
    Quaternion FromAxisAngle(const Vector3& Axis, const float Angle)
    {
        return Quaternion(Axis, Angle);
    }

    // From To Rotation, Vectors Normalized
    Quaternion FromToRotation(const Vector3& a, const Vector3& b)
    {
        return Quaternion( a, b );
    }

    // Axis Normalized
    // ?
    Quaternion LookRotation(const Vector3& Direction, const Vector3& Up)
    {
        Vector3 c = Direction;
		Vector3 Right = CrossProduct(Up, c);
        Vector3 a = Right * InvSqrt(Max(0.00001f, DotProduct(Right, Right)));
        Vector3 b = CrossProduct(c, a);


        Matrix4 Result(
            a[0], a[1], a[2], 0.0f,
            b[0], b[1], b[2], 0.0f,
            c[0], c[1], c[2], 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
        );
        return Quaternion{ Result };
    }

    float Angle(const Quaternion& a, const Quaternion& b)
    {
        float Dot = DotProduct(a, b);
        return RadToDeg(Acos(Min(Abs(Dot), 1.0f)) * 2.0f);
    }

    Quaternion RotateTowards(const Quaternion& From, const Quaternion& To, float Delta)
    {
        float a = Angle(From, To);
        if( IsEqual(a, 0.0f, 0.00000001f) )
        {
            return To;
        }
        //
		float t = Min(Delta/a, 1.0f);
        return SLerp(From, To, t);
    }

    // Rotation From Quaternion
    Matrix4 ToMatrix4(const Quaternion& Qt)
    {
        return Qt.ToMatrix();
    }

    // Normalized
    Quaternion Lerp(const Quaternion& a, const Quaternion& b, const float t)
    {
        Quaternion R;
        for (int i = 0; i < 4; ++i)
        {
            R[i] = Lerp(a[i], b[i], t);
        }
        return R;
    }

    // Normalized
    // ?
    Quaternion SLerp(const Quaternion& x, const Quaternion& y, const float t)
    {
		Quaternion z = y;

		float cosTheta = DotProduct(x, y);

		if(cosTheta < 0.0f)
		{
			z = -y;
			cosTheta = -cosTheta;
		}

		if( cosTheta > 1.0f - EPSILON )
		{
			// Linear interpolation
			return Quaternion(
				Lerp(x.x, z.x, t),
				Lerp(x.y, z.y, t),
				Lerp(x.z, z.z, t),
                Lerp(x.w, z.w, t));
		}
		else
		{
			// Essential Mathematics, page 467
			float angle = Acos(cosTheta);
			return (x*Sin((1.0f - t) * angle) + z*Sin(t * angle)) / Sin(angle);
		}
    }

    Matrix4 Rotation(const Quaternion& Qt)
    {
        return ToMatrix4(Qt);
    }

    Matrix4 Transform(const Vector3& p, const Quaternion& r, const Vector3& s)
    {
        return Scale(s) * Rotation(r) * Translation(p);
    }

    // In Degrees
    Matrix4 Transform2D(const Vector2& o, const float a, const Vector2& s)
    {
        return Translation(-o.x, -o.y, 0.0f) * Scale(s.x, s.y, 1.0f) *
            Rotation( Quaternion{ { 0.0f, 0.0f, 1.0f }, a } ) * Translation(o.x, o.y, 0.0f);
    }
}

std::ostream& operator<<(std::ostream& Stream, const Quaternion& Other)
{
    Stream << Other.x << ", " << Other.y << ", " << Other.z << ": " << Other.w;
    return Stream;
}
