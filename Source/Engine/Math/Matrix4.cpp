#include "Matrix4.hpp"

void Matrix4::SetIdentity()
{
    for(int i = 0; i < 4; ++i)
    {
        for(int j = 0; j < 4; ++j)
        {
            if( i == j ) Values[i*4+j] = 1.0f;
            else  Values[i*4+j] = 0.0f;
        }
    }
}

Matrix4 Matrix4::Inverse() const
{
    return Math::Inverse(*this);
}

void Matrix4::Invert()
{
    *this = Inverse();
}

Matrix4 Matrix4::Transposition() const
{
    return Math::Transpose(*this);
}

void Matrix4::Transpose()
{
    *this = Transposition();
}

Matrix4 Matrix4::operator*(const Matrix4& o) const
{
    return
    {
        Values[0]*o.Values[0] + Values[1]*o.Values[4] + Values[2]*o.Values[8] + Values[3]*o.Values[12],
        Values[0]*o.Values[1] + Values[1]*o.Values[5] + Values[2]*o.Values[9] + Values[3]*o.Values[13],
        Values[0]*o.Values[2] + Values[1]*o.Values[6] + Values[2]*o.Values[10] + Values[3]*o.Values[14],
        Values[0]*o.Values[3] + Values[1]*o.Values[7] + Values[2]*o.Values[11] + Values[3]*o.Values[15],

        Values[4]*o.Values[0] + Values[5]*o.Values[4] + Values[6]*o.Values[8] + Values[7]*o.Values[12],
        Values[4]*o.Values[1] + Values[5]*o.Values[5] + Values[6]*o.Values[9] + Values[7]*o.Values[13],
        Values[4]*o.Values[2] + Values[5]*o.Values[6] + Values[6]*o.Values[10] + Values[7]*o.Values[14],
        Values[4]*o.Values[3] + Values[5]*o.Values[7] + Values[6]*o.Values[11] + Values[7]*o.Values[15],

        Values[8]*o.Values[0] + Values[9]*o.Values[4] + Values[10]*o.Values[8] + Values[11]*o.Values[12],
        Values[8]*o.Values[1] + Values[9]*o.Values[5] + Values[10]*o.Values[9] + Values[11]*o.Values[13],
        Values[8]*o.Values[2] + Values[9]*o.Values[6] + Values[10]*o.Values[10] + Values[11]*o.Values[14],
        Values[8]*o.Values[3] + Values[9]*o.Values[7] + Values[10]*o.Values[11] + Values[11]*o.Values[15],

        Values[12]*o.Values[0] + Values[13]*o.Values[4] + Values[14]*o.Values[8] + Values[15]*o.Values[12],
        Values[12]*o.Values[1] + Values[13]*o.Values[5] + Values[14]*o.Values[9] + Values[15]*o.Values[13],
        Values[12]*o.Values[2] + Values[13]*o.Values[6] + Values[14]*o.Values[10] + Values[15]*o.Values[14],
        Values[12]*o.Values[3] + Values[13]*o.Values[7] + Values[14]*o.Values[11] + Values[15]*o.Values[15]
    };
}

Matrix4 Matrix4::IDENTITY = {};

namespace Math
{
    Matrix4 Transpose(const Matrix4& Mat)
    {
        Matrix4 D;
        for(int i = 0; i < 4; ++i)
        {
            for(int j = 0; j < 4; ++j)
            {
                D[j*4+i] = Mat[i*4+j];
            }
        }
        return D;
    }

    Matrix4 Inverse(const Matrix4& D)
    {
			float Coef00 = D[10] * D[15] - D[14] * D[11];
			float Coef02 = D[6] * D[15] - D[14] * D[7];
			float Coef03 = D[6] * D[11] - D[10] * D[7];

			float Coef04 = D[9] * D[15] - D[13] * D[11];
			float Coef06 = D[5] * D[15] - D[13] * D[7];
			float Coef07 = D[5] * D[11] - D[9] * D[7];

			float Coef08 = D[9] * D[14] - D[13] * D[10];
			float Coef10 = D[5] * D[14] - D[13] * D[6];
			float Coef11 = D[5] * D[10] - D[9] * D[6];

			float Coef12 = D[8] * D[15] - D[12] * D[11];
			float Coef14 = D[4] * D[15] - D[12] * D[7];
			float Coef15 = D[4] * D[11] - D[8] * D[7];

			float Coef16 = D[8] * D[14] - D[12] * D[10];
			float Coef18 = D[4] * D[14] - D[12] * D[6];
			float Coef19 = D[4] * D[10] - D[8] * D[6];

			float Coef20 = D[8] * D[13] - D[12] * D[9];
			float Coef22 = D[4] * D[13] - D[12] * D[5];
			float Coef23 = D[4] * D[9] - D[8] * D[5];

			Vector4 Fac0(Coef00, Coef00, Coef02, Coef03);
			Vector4 Fac1(Coef04, Coef04, Coef06, Coef07);
			Vector4 Fac2(Coef08, Coef08, Coef10, Coef11);
			Vector4 Fac3(Coef12, Coef12, Coef14, Coef15);
			Vector4 Fac4(Coef16, Coef16, Coef18, Coef19);
			Vector4 Fac5(Coef20, Coef20, Coef22, Coef23);

			Vector4 Vec0(D[4], D[0], D[0], D[0]);
			Vector4 Vec1(D[5], D[1], D[1], D[1]);
			Vector4 Vec2(D[6], D[2], D[2], D[2]);
			Vector4 Vec3(D[7], D[3], D[3], D[3]);

			Vector4 Inv0(Vec1 * Fac0 - Vec2 * Fac1 + Vec3 * Fac2);
			Vector4 Inv1(Vec0 * Fac0 - Vec2 * Fac3 + Vec3 * Fac4);
			Vector4 Inv2(Vec0 * Fac1 - Vec1 * Fac3 + Vec3 * Fac5);
			Vector4 Inv3(Vec0 * Fac2 - Vec1 * Fac4 + Vec2 * Fac5);

			Vector4 SignA(+1.0f, -1.0f, +1.0f, -1.0f);
			Vector4 SignB(-1.0f, +1.0f, -1.0f, +1.0f);
			Matrix4 Inverse(Inv0 * SignA, Inv1 * SignB, Inv2 * SignA, Inv3 * SignB);

			Vector4 Row0(Inverse[0], Inverse[4], Inverse[8], Inverse[12]);

            Vector4 MVec0(D[0], D[1], D[2], D[3]);
			Vector4 Dot0(MVec0 * Row0);
			float Dot1 = (Dot0.x + Dot0.y) + (Dot0.z + Dot0.w);

			float OneOverDeterminant = 1.0f / Dot1;

            for (int i = 0; i < 16; ++i)
            {
                Inverse[i] *= OneOverDeterminant;
            }
            return Inverse;
    }

    Matrix4 RotationX(const float Angle)
    {
        float SinF = Sin(DegToRad(Angle));
        float CosF = Cos(DegToRad(Angle));

        Matrix4 R;
        R[5] = CosF;
        R[6] = SinF;
        R[9] = -SinF;
        R[10] = CosF;
        return R;
    }

    Matrix4 RotationY(const float Angle)
    {
        float SinF = Sin(DegToRad(Angle));
        float CosF = Cos(DegToRad(Angle));

        Matrix4 R;
        R[0] = CosF;
        R[2] = -SinF;
        R[8] = SinF;
        R[10] = CosF;
        return R;
    }

    Matrix4 RotationZ(const float Angle)
    {
        float SinF = Sin(DegToRad(Angle));
        float CosF = Cos(DegToRad(Angle));

        Matrix4 R;
        R[0] = CosF;
        R[1] = SinF;
        R[4] = -SinF;
        R[5] = CosF;
        return R;
    }

    // Angle Axis - Degrees, Normalized Axis
    Matrix4 Rotation(const Vector3& Axis, const float Angle)
    {
		float a = DegToRad(Angle);
		float c = Cos(a);
		float s = Sin(a);

		Vector3 axis(Normalize(Axis));
		Vector3 temp(axis*(1.0f - c));

		Matrix4 Rotate;
		Rotate[0] = c + temp[0] * axis[0];
		Rotate[1] = temp[0] * axis[1] + s * axis[2];
		Rotate[2] = temp[0] * axis[2] - s * axis[1];

		Rotate[4] = temp[1] * axis[0] - s * axis[2];
		Rotate[5] = c + temp[1] * axis[1];
		Rotate[6] = temp[1] * axis[2] + s * axis[0];

		Rotate[8] = temp[2] * axis[0] + s * axis[1];
		Rotate[9] = temp[2] * axis[1] - s * axis[0];
		Rotate[10] = c + temp[2] * axis[2];

        return Rotate;
    }

    Matrix4 Scale(const float x)
    {
        return Scale({x,x,x});
    }

    Matrix4 Scale(const float x, const float y, const float z)
    {
        return Scale({x,y,z});
    }

    Matrix4 Scale(const Vector3& Scale)
    {
        Matrix4 R;
        R[0] = Scale.x;
        R[5] = Scale.y;
        R[10] = Scale.z;
        return R;
    }

    Matrix4 Translation(const float x, const float y, const float z)
    {
        return Translation( {x, y, z} );
    }

    Matrix4 Translation(const Vector3& Translation)
    {
        Matrix4 R;
        R[12] = Translation.x;
        R[13] = Translation.y;
        R[14] = Translation.z;
        return R;
    }

    // World Space Arguments
    Matrix4 LookAt(const Vector3& Camera, const Vector3& Position, const Vector3& Up)
    {
		Vector3 f(Normalize(Position - Camera));
		Vector3 s(Normalize(CrossProduct(Up, f)));
		Vector3 u(CrossProduct(f, s));

		Matrix4 Result;
		Result[0] = s.x;
		Result[4] = s.y;
		Result[8] = s.z;
		Result[1] = u.x;
		Result[5] = u.y;
		Result[9] = u.z;
		Result[2] = f.x;
		Result[6] = f.y;
		Result[10] = f.z;
		Result[12] = -DotProduct(s, Camera);
		Result[13] = -DotProduct(u, Camera);
		Result[14] = -DotProduct(f, Camera);
		return Result;
    }

    // In Degrees, [-1, 1]
    Matrix4 Perspective(const float Fov, const float Aspect, const float Near, const float Far)
    {
		float tanHalfFovy = Tan(DegToRad(Fov/2.0f));

		Matrix4 Result;
		Result[0] = 1.0f / (Aspect * tanHalfFovy);
		Result[5] = 1.0f / (tanHalfFovy);
		Result[10] = (Far + Near) / (Far - Near);
		Result[11] = 1.0f;
		Result[14] = - (2.0f * Far * Near) / (Far - Near);
        Result[15] = 0.0f;
		return Result;
    }

    // [-1, 1]
    Matrix4 Ortho(const float Right, const float Left, const float Top, const float Bottom, const float Far, const float Near)
    {
		Matrix4 Result;
		Result[0] = 2.0f / (Right - Left);
		Result[5] = 2.0f / (Top - Bottom);
		Result[10] = 2.0f / (Far - Near);
		Result[12] = - (Right + Left) / (Right - Left);
		Result[13] = - (Top + Bottom) / (Top - Bottom);
		Result[14] = - (Far + Near) / (Far - Near);
		return Result;
    }

    Matrix4 Lerp(const Matrix4& a, const Matrix4& b, const float t)
    {
        Matrix4 R;
        for(int i = 0; i < 16; ++i)
        {
            R[i] = Lerp( a[i], b[i], t );
        }
        return R;
    }
}

std::ostream& operator<<(std::ostream& Stream, const Matrix4& Other)
{
    Stream << Other[0] << " " << Other[1] << " " << Other[2] << " " << Other[3] << "\n";
    Stream << Other[4] << " " << Other[5] << " " << Other[6] << " " << Other[7] << "\n";
    Stream << Other[8] << " " << Other[9] << " " << Other[10] << " " << Other[11] << "\n";
    Stream << Other[12] << " " << Other[13] << " " << Other[14] << " " << Other[15];
    return Stream;
}
