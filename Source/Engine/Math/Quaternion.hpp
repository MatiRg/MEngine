#pragma once
#include "Functions.hpp"
#include "Matrix4.hpp"

template<class T>
class TQuaternion
{
public:
    TQuaternion()
    {
        Identity();
    }

    TQuaternion(const T ax, const T ay, const T az)
    {
        FromEulerAngles(ax, ay, az);
    }

    TQuaternion(const TVector3<T>& v)
    {
        FromEulerAngles(v);
    }

    /// Degrees, AxisAngle, Axis normalized
    TQuaternion(const TVector3<T>& v, const T angle)
    {
        FromAxisAngle(v, angle);
    }

    /// Rotation from a to b
    /// Based Urho3D code
    TQuaternion(const TVector3<T>& a, const TVector3<T>& b)
    {
        TVector3<T> na = Math::Normalize(a);
        TVector3<T> nb = Math::Normalize(b);
        float d = Math::DotProduct(na, nb);

        if (d > T(-1) + Math::EPSILON)
        {
            TVector3<T> c = Math::CrossProduct(na, nb);
            float s = Math::Sqrt( (T(1) + d) * T(2) );
            float InvS = T(1) / s;

            x = c.x * InvS;
            y = c.y * InvS;
            z = c.z * InvS;
            w = T(0.5) * s;
        }
        else
        {
            TVector3<T> Axis = Math::CrossProduct(TVector3<T>::RIGHT, na);
            if (Math::Length(Axis) < Math::EPSILON)
            {
                Axis = Math::CrossProduct(TVector3<T>::UP, na);
            }
            FromAxisAngle(Axis, T(180));
        }
    }

    /// Construct from components
    TQuaternion(const T ax, const T ay, const T az, const T aw):
        x(ax),
        y(ay),
        z(az),
        w(aw)
    {
    }

    /// Pure Rotation matrix
    TQuaternion(const TMatrix4<T>& D)
    {
        FromMatrix(D);
    }


    TQuaternion(const TQuaternion<T>& D):
        x(D.x),
        y(D.y),
        z(D.z),
        w(D.w)
    {
    }

    ~TQuaternion()
    {
    }

    void Identity()
    {
        x = y = z = T(0);
        w = T(1);
    }

    TQuaternion<T> Conjugate() const
    {
        TQuaternion<T> Q1;
        Q1.x = -x;
        Q1.y = -y;
        Q1.z = -z;
        Q1.w = w;
        return Q1;
    }

    // Length(magnitude) of Quaternion
    T Length() const
    {
        return Math::Sqrt( LengthSquared() );
    }

    // Length(magnitude) of Quaternion Squared
    T LengthSquared() const
    {
        return x*x + y*y + z*z + w*w;
    }

    void Normalize()
    {
        T L = Length();
        if( L )
        {
            x /= L;
            y /= L;
            z /= L;
            w /= L;
        }
    }

    T DotProduct(const TQuaternion<T>& Other) const
    {
        return x*Other.x + y*Other.y + z*Other.z + w*Other.w;
    }

    TQuaternion<T> Inverse() const
    {
        TQuaternion<T> Tmp;
        Tmp.InverseHelper(*this);
        return Tmp;
    }

    void Invert()
    {
        InverseHelper(*this);
    }

    TVector3<T> ToEulerAngles() const
    {
        T Check = T(2) * (-y * z + w * x);

        if (Check < T(-0.995) )
        {
            return TVector3<T>(
                T(-90.0),
                T(0.0),
                Math::RadToDeg(-Math::Atan2(T(2) * (x * z - w * y), T(1) - T(2) * (y * y + z * z)))
            );
        }
        else if (Check > T(0.995))
        {
            return TVector3<T>(
                T(90.0),
                T(0.0),
                Math::RadToDeg(Math::Atan2(T(2) * (x * z - w * y), T(1) - T(2) * (y * y + z * z)))
            );
        }
        else
        {
            return TVector3<T>(
                Math::RadToDeg(Math::Asin(Check)),
                Math::RadToDeg(Math::Atan2(T(2) * (x * z + w * y), T(1) - T(2) * (x * x + y * y))),
                Math::RadToDeg(Math::Atan2(T(2) * (x * y + w * z), T(1) - T(2) * (x * x + z * z)))
            );
        }
    }

    void FromEulerAngles(const T ax, const T ay, const T az)
    {
        FromEulerAngles({ax, ay, az});
    }

    void FromEulerAngles(const TVector3<T>& v)
    {
        // ZXY
        T ax = Math::DegToRad(v.x);
        T ay = Math::DegToRad(v.y);
        T az = Math::DegToRad(v.z);

        T sinX = Math::Sin(ax);
        T cosX = Math::Cos(ax);
        T sinY = Math::Sin(ay);
        T cosY = Math::Cos(ay);
        T sinZ = Math::Sin(az);
        T cosZ = Math::Cos(az);

        w = cosY * cosX * cosZ + sinY * sinX * sinZ;
        x = cosY * sinX * cosZ + sinY * cosX * sinZ;
        y = sinY * cosX * cosZ - cosY * sinX * sinZ;
        z = cosY * cosX * sinZ - sinY * sinX * cosZ;
    }

    // http://www.euclideanspace.com/maths/geometry/rotations/conversions/quaternionToAngle/index.htm
    void ToAxisAngle(TVector3<T>& v, T& angle) const
    {
        if( w > T(1) ) Normalize(); // if w>1 acos and sqrt will produce errors, this cant happen if quaternion is normalised
        angle = Math::RadToDeg(T(2)* Math::Acos(w));

        T s = Math::Sqrt( T(1)- w*w); // assuming quaternion normalised then w is less than 1, so term always positive.
        if( s < T(0.001) || s == T(0) )
        { // test to avoid divide by zero, s is always positive due to sqrt
            // if s close to zero then direction of axis not important
            v.x = x; // if it is important that axis is normalised then replace with x=1; y=z=0;
            v.y = y;
            v.z = z;
        }
        else
        {
            v.x = x / s; // normalise axis
            v.y = y / s;
            v.z = z / s;
        }
    }

    void FromAxisAngle(const TVector3<T>& v, const T angle)
    {
        T ww = Math::DegToRad(angle /T(2));
        T k = Math::Sin(ww);

        x = v.x*k;
        y = v.y*k;
        z = v.z*k;
        w = Math::Cos(ww);
    }

    TMatrix4<T> ToMatrix() const
    {
		TMatrix4<T> D;

        T qxx(x * x);
        T qyy(y * y);
        T qzz(z * z);
        T qxz(x * z);
        T qxy(x * y);
        T qyz(y * z);
        T qwx(w * x);
        T qwy(w * y);
        T qwz(w * z);

        D[0] = T(1) - T(2) * (qyy + qzz);
        D[1] = T(2) * (qxy + qwz);
        D[2] = T(2) * (qxz - qwy);

        D[4] = T(2) * (qxy - qwz);
        D[5] = T(1) - T(2) * (qxx + qzz);
        D[6] = T(2) * (qyz + qwx);

        D[8] = T(2) * (qxz + qwy);
        D[9] = T(2) * (qyz - qwx);
        D[10] = T(1) - T(2) * (qxx + qyy);

		return D;
    }

    // GLM
    void FromMatrix(const TMatrix4<T>& m)
    {
        T fourXSquaredMinus1 = m[0] - m[5] - m[10];
        T fourYSquaredMinus1 = m[5] - m[0] - m[10];
        T fourZSquaredMinus1 = m[10] - m[0] - m[5];
        T fourWSquaredMinus1 = m[0] + m[5] + m[10];

        int biggestIndex = 0;
        T fourBiggestSquaredMinus1 = fourWSquaredMinus1;
        if (fourXSquaredMinus1 > fourBiggestSquaredMinus1)
        {
            fourBiggestSquaredMinus1 = fourXSquaredMinus1;
            biggestIndex = 1;
        }
        if (fourYSquaredMinus1 > fourBiggestSquaredMinus1)
        {
            fourBiggestSquaredMinus1 = fourYSquaredMinus1;
            biggestIndex = 2;
        }
        if (fourZSquaredMinus1 > fourBiggestSquaredMinus1)
        {
            fourBiggestSquaredMinus1 = fourZSquaredMinus1;
            biggestIndex = 3;
        }

        T biggestVal = Math::Sqrt(fourBiggestSquaredMinus1 + T(1) ) * T(0.5);
        T mult = T(0.25) / biggestVal;

        switch (biggestIndex)
        {
        case 0:
            w = biggestVal;
            x = (m[6] - m[9]) * mult;
            y = (m[8] - m[2]) * mult;
            z = (m[1] - m[4]) * mult;
            break;
        case 1:
            w = (m[6] - m[9]) * mult;
            x = biggestVal;
            y = (m[1] + m[4]) * mult;
            z = (m[8] + m[2]) * mult;
            break;
        case 2:
            w = (m[8] - m[2]) * mult;
            x = (m[1] + m[4]) * mult;
            y = biggestVal;
            z = (m[6] + m[9]) * mult;
            break;
        case 3:
            w = (m[1] - m[4]) * mult;
            x = (m[8] + m[2]) * mult;
            y = (m[6] + m[9]) * mult;
            z = biggestVal;
            break;
        default:
            x = y = z = T(0);
            w = T(1);
            break;
        }
    }

    T& operator[](std::size_t idx)
    {
        return (&x)[idx];
    }

    const T& operator[](std::size_t idx) const
    {
        return (&x)[idx];
    }

    TQuaternion<T>& operator=(const TQuaternion<T>& D)
    {
        if( this == &D )
            return *this;
        x = D.x;
        y = D.y;
        z = D.z;
        w = D.w;
        return *this;
    }

    TQuaternion<T> operator*(const T Other) const
    {
        return TQuaternion( x * Other, y * Other, z * Other, w * Other );
    }

    TQuaternion<T>& operator*=(const T Other)
    {
        x *= Other;
        y *= Other;
        z *= Other;
        w *= Other;
        return *this;
    }

    // https://github.com/g-truc/glm/blob/master/glm/detail/type_quat.inl ?? GLM
    // https://gamedev.stackexchange.com/questions/28395/rotating-vector3-by-a-quaternion
    // rotated_point = orientation_quaternion *  point;
    // rotated_point = origin + (orientation_quaternion * (point-origin));
    TVector3<T> operator*(const TVector3<T>& Other) const
    {
        TVector3<T> QuatVector(x, y, z);
        TVector3<T> uv = Math::CrossProduct(QuatVector, Other);
        TVector3<T> uuv = Math::CrossProduct(QuatVector, uv);

        return Other + (uv * w + uuv) * T(2);
    }

	TQuaternion<T> operator*(const TQuaternion<T>& D) const
	{
		TQuaternion<T> Q;
		Q.w = w*D.w - x*D.x - y*D.y - z*D.z;
		Q.x = w*D.x + x*D.w + y*D.z - z*D.y;
		Q.y = w*D.y + y*D.w + z*D.x - x*D.z;
		Q.z = w*D.z + z*D.w + x*D.y - y*D.x;
		return Q;
    }

    TQuaternion<T>& operator *=(const TQuaternion<T>& D)
    {
        TQuaternion<T> Q = *this;
        w = Q.w*D.w - Q.x*D.x - Q.y*D.y - Q.z*D.z;
        x = Q.w*D.x + Q.x*D.w + Q.y*D.z - Q.z*D.y;
        y = Q.w*D.y + Q.y*D.w + Q.z*D.x - Q.x*D.z;
        z = Q.w*D.z + Q.z*D.w + Q.x*D.y - Q.y*D.x;
        return *this;
    }

    TQuaternion<T> operator+(const TQuaternion<T>& Other) const
    {
        return TQuaternion( x + Other.x, y + Other.y, z + Other.z, w + Other.w );
    }

    TQuaternion<T>& operator+=(const TQuaternion<T>& Other)
    {
        x += Other.x;
        y += Other.y;
        z += Other.z;
        w += Other.w;
        return *this;
    }

    TQuaternion<T> operator-(const TQuaternion<T>& Other) const
    {
        return TQuaternion( x - Other.x, y - Other.y, z - Other.z, w - Other.w );
    }

    TQuaternion<T>& operator-=(const TQuaternion<T>& Other)
    {
        x -= Other.x;
        y -= Other.y;
        z -= Other.z;
        w -= Other.w;
        return *this;
    }

    //
    void AddScaledVector(const TVector3<T>& v, const T s)
    {
        TQuaternion<T> q( v.x * s, v.y * s, v.z * s, T(0) );
        q *= *this;
        w += q.w * T(0.5);
        x += q.x * T(0.5);
        y += q.y * T(0.5);
        z += q.z * T(0.5);
    }

    void RotateByVector(const TVector3<T>& v)
    {
        TQuaternion<T>& Self = *this;
        TQuaternion<T> q( v.x, v.y, v.z, T(0) );
        Self *= q;
    }
    //

    static TQuaternion<T> IDENTITY;
private:
    void InverseHelper(const TQuaternion<T>& D)
    {
        TQuaternion<T>& Self = *this;
        T L = D.LengthSquared();
        if( L == 1.0f )
            Self = D.Conjugate();
        else if (L >= Math::EPSILON)
            Self = D.Conjugate() * ( T(1) / L);
        else
            Self = IDENTITY;
    }
public:
    T x, y, z, w;
};

template<class T> TQuaternion<T> TQuaternion<T>::IDENTITY = {};

using Quaternion = TQuaternion<float>;

namespace Math
{
    template<class T>
    TMatrix4<T> Transform(const TVector3<T>& aPosition, const TQuaternion<T>& aRotation, const TVector3<T>& aScale)
    {
        return Scale(aScale) * Rotation(aRotation) * Translation(aPosition);
    }

    template<class T>
    TMatrix4<T> Transform2D(const TVector2<T>& aOrigin, const T aAngle, const TVector2<T>& aScale)
    {
        return Translation(-aOrigin.x, -aOrigin.y, T(0)) * Scale(aScale.x, aScale.y, T(1)) *
            Rotation( TQuaternion<T>{ { T(0), T(0), T(1) }, aAngle } ) * Translation(aOrigin.x, aOrigin.y, T(0));
    }

    template<class T>
    TQuaternion<T> Normalize(const TQuaternion<T>& a)
    {
        TQuaternion<T> t = a;
        t.Normalize();
        return t;
    }

    template<class T>
    T DotProduct(const TQuaternion<T>& a, const TQuaternion<T>& b)
    {
        return a.DotProduct(b);
    }

    // Euler
    template<class T>
    TMatrix4<T> Rotation(const T x, const T y, const T z)
    {
        return TQuaternion<T>({ x, y, z }).ToMatrix();
    }

    // Euler
    template<class T>
    TMatrix4<T> Rotation(const TVector3<T>& v)
    {
        return TQuaternion<T>(v).ToMatrix();
    }

    template<class T>
    TMatrix4<T> Rotation(const TQuaternion<T>& a)
    {
        return a.ToMatrix();
    }

    // ?
    // https://gist.github.com/aeroson/043001ca12fe29ee911e
    template<class T>
    TQuaternion<T> LookRotation(const TVector3<T>& Dir, const TVector3<T>& Up)
    {
        TVector3<T> Forward = Normalize(Dir);
        TVector3<T> Right = Normalize(CrossProduct(Up, Forward));
        TVector3<T> NewUp = CrossProduct(Forward, Right);

        TMatrix4<T> Result(
            Right[0], Right[1], Right[2], T(0),
            NewUp[0], NewUp[1], NewUp[2], T(0),
            Forward[0], Forward[1], Forward[2], T(0),
            T(0), T(0), T(0), T(1)
        );
        return { Result };

        /*TVector3<T> Dir = -aDir;
        TVector3<T> Right = CrossProduct(Up, Dir);
        TVector3<T> Tmp = Right * InvSqrt(Max(T(0.00001), DotProduct(Right, Right)));
        TVector3<T> Tmp2 = CrossProduct(Dir, Tmp);

        TMatrix4<T> Result(
            Tmp[0], Tmp[1], Tmp[2], T(0),
            Tmp2[0], Tmp2[1], Tmp2[2], T(0),
            Dir[0], Dir[1], Dir[2], T(0),
            T(0), T(0), T(0), T(1)
        );
        return { Result }; */
    }

    // http://www.opengl-tutorial.org/intermediate-tutorials/tutorial-17-quaternions/
    template<class T>
    TQuaternion<T> RotationBetweenVectors(const TVector3<T>& aStart, const TVector3<T>& aDest) 
    {
        TVector3<T> start = Normalize(aStart);
        TVector3<T> dest = Normalize(aDest);

        T cosTheta = DotProduct(start, dest);
        TVector3<T> rotationAxis;

        if (cosTheta < T(-1) + T(0.001) ) 
        {
            // special case when vectors in opposite directions:
            // there is no "ideal" rotation axis
            // So guess one; any will do as long as it's perpendicular to start
            rotationAxis = CrossProduct(TVector3<T>(T(0), T(0), T(1)), start);
            T len = LengthSquared(rotationAxis);
            if ( len < T(0.01) ) // bad luck, they were parallel, try again!
            {
                rotationAxis = CrossProduct(TVector3<T>(T(1), T(0), T(0)), start);
            }
            rotationAxis = Normalize(rotationAxis);
            return TQuaternion<T>(rotationAxis, T(180));
        }

        rotationAxis = CrossProduct(start, dest);

        T s = Sqrt((T(1) + cosTheta) * T(2));
        T invs = T(1) / s;

        return TQuaternion<T>(
            rotationAxis.x * invs,
            rotationAxis.y * invs,
            rotationAxis.z * invs,
            s * T(0.5)
        );
    }

    /// Normalized
    template<class T>
    TQuaternion<T> Lerp(const TQuaternion<T>& a, const TQuaternion<T>& b, const T v)
    {
        TQuaternion<T> R;
        for (int i = 0; i < 4; ++i)
        {
            R[i] = Lerp(a[i], b[i], v);
        }
        R.Normalize();
        return R;
    }

    /// Normalized
    template<class T>
    TQuaternion<T> SLerp(const TQuaternion<T>& a, const TQuaternion<T>& b, const T v)
    {
        TQuaternion<T> R;

        // Calculate angle between them.
        T CosHalfTheta = a.DotProduct(b);

        // if qa=qb or qa=-qb then theta = 0 and we can return qa
        if (Abs(CosHalfTheta) >= T(1))
        {
            R.w = a.w;
            R.x = a.x;
            R.y = a.y;
            R.z = a.z;
            return R;
        }

        // Calculate temporary values.
        T HalfTheta = Acos(CosHalfTheta);
        T SinHalfTheta = Sqrt(T(1) - CosHalfTheta * CosHalfTheta);

        // if theta = 180 degrees then result is not fully defined
        // we could rotate around any axis normal to qa or qb
        if (Abs(SinHalfTheta) < T(0.001))
        { // fabs is floating point absolute
            R.w = (a.w * T(0.5) + b.w * T(0.5));
            R.x = (a.x * T(0.5) + b.x * T(0.5));
            R.y = (a.y * T(0.5) + b.y * T(0.5));
            R.z = (a.z * T(0.5) + b.z * T(0.5));
            return R;
        }

        T RatioA = Sin((T(1) - v) * HalfTheta) / SinHalfTheta;
        T RatioB = Sin(v * HalfTheta) / SinHalfTheta;

        //calculate Quaternion.
        R.w = (a.w * RatioA + b.w * RatioB);
        R.x = (a.x * RatioA + b.x * RatioB);
        R.y = (a.y * RatioA + b.y * RatioB);
        R.z = (a.z * RatioA + b.z * RatioB);

        return R;
    }
}

template<class T>
std::ostream& operator<<(std::ostream& Stream, const TQuaternion<T>& Other)
{
    Stream << Other.x << ", " << Other.y << ", " << Other.z << ": " << Other.w;
    return Stream;
}
