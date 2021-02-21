#pragma once
#include "Vector3.hpp"
#include "Vector4.hpp"

template<class T>
class TMatrix4
{
public:
    TMatrix4()
    {
        Identity();
    }

    TMatrix4(const T a1, const T a2, const T a3, const T a4,
            const T a5, const T a6, const T a7, const T a8,
            const T a9, const T a10, const T a11, const T a12,
            const T a13, const T a14, const T a15, const T a16):
                Values{ a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16 }
    {
    }

    TMatrix4(const T* D)
    {
        for(int i = 0; i < 16; ++i)
        {
            Values[i] = D[i];
        }
    }

    TMatrix4(const TMatrix4<T>& D)
    {
        for(int i = 0; i < 16; ++i)
        {
            Values[i] = D.Values[i];
        }
    }

    ~TMatrix4()
    {
    }

    void Identity()
    {
        for(int i = 0; i < 4; ++i)
        {
            for(int j = 0; j < 4; ++j)
            {
                if( i == j ) Values[i*4+j] = T(1);
                else  Values[i*4+j] = T(0);
            }
        }
    }


    T Determinant() const
    {
        T value =
        Values[3] * Values[6] * Values[9] * Values[12] - Values[2] * Values[7] * Values[9] * Values[12] - Values[3] * Values[5] * Values[10] * Values[12] + Values[1] * Values[7] * Values[10] * Values[12] +
        Values[2] * Values[5] * Values[11] * Values[12] - Values[1] * Values[6] * Values[11] * Values[12] - Values[3] * Values[6] * Values[8] * Values[13] + Values[2] * Values[7] * Values[8] * Values[13] +
        Values[3] * Values[4] * Values[10] * Values[13] - Values[0] * Values[7] * Values[10] * Values[13] - Values[2] * Values[4] * Values[11] * Values[13] + Values[0] * Values[6] * Values[11] * Values[13] +
        Values[3] * Values[5] * Values[8] * Values[14] - Values[1] * Values[7] * Values[8] * Values[14] - Values[3] * Values[4] * Values[9] * Values[14] + Values[0] * Values[7] * Values[9] * Values[14] +
        Values[1] * Values[4] * Values[11] * Values[14] - Values[0] * Values[5] * Values[11] * Values[14] - Values[2] * Values[5] * Values[8] * Values[15] + Values[1] * Values[6] * Values[8] * Values[15] +
        Values[2] * Values[4] * Values[9] * Values[15] - Values[0] * Values[6] * Values[9] * Values[15] - Values[1] * Values[4] * Values[10] * Values[15] + Values[0] * Values[5] * Values[10] * Values[15];
        return value;
    }

    TMatrix4<T> Inverse() const
    {
        TMatrix4<T> Mat;
        Mat.InverseHelper(*this);
        return Mat;
    }

    void Invert()
    {
        InverseHelper(*this);
    }

    TMatrix4<T> Transposition() const
    {
        TMatrix4<T> D;
        for(int i = 0; i < 4; ++i)
        {
            for(int j = 0; j < 4; ++j)
            {
                D.Values[j*4+i] = Values[i*4+j];
            }
        }
        return D;
    }

    void Transpose()
    {
        *this = Transposition();
    }

    TVector3<T> GetTranslation() const
    {
        return { Values[12], Values[13], Values[14] };
    }

    // Valid?
    TVector3<T> GetScale() const
    {
        return { 
            Math::Sqrt(Values[0] * Values[0] + Values[1] * Values[1] + Values[2] * Values[2]),
            Math::Sqrt(Values[4] * Values[4] + Values[5] * Values[5] + Values[6] * Values[6]),
            Math::Sqrt(Values[8] * Values[8] + Values[9] * Values[9] + Values[10] * Values[10])
        };
    }

    void SetForward(const TVector3<T>& v)
    {
        Values[0] = v.x;
        Values[1] = v.y;
        Values[2] = v.z;
    }

    TVector3<T> GetForward() const
    {
        return { Values[0], Values[1], Values[2] };
    }

    void SetUp(const TVector3<T>& v)
    {
        Values[4] = v.x;
        Values[5] = v.y;
        Values[6] = v.z;
    }

    TVector3<T> GetUp() const
    {
        return { Values[4], Values[5], Values[6] };
    }

    void SetRight(const TVector3<T>& v)
    {
        Values[8] = v.x;
        Values[9] = v.y;
        Values[10] = v.z;
    }

    TVector3<T> GetRight() const
    {
        return { Values[8], Values[9], Values[10] };
    }

    T& operator[](const std::size_t idx)
    {
        return Values[idx];
    }

    const T& operator[](const std::size_t idx) const
    {
        return Values[idx];
    }

    TMatrix4<T>& operator=(const TMatrix4<T>& D)
    {
        if( this == &D )
        {
            return *this;
        }
        for(int i = 0; i < 16; ++i)
        {
            Values[i] = D.Values[i];
        }
        return *this;
    }

    /*
    You only seem to be taking the 3x3 part of your 4x4 matrix into account when doing the multiplication.
    If you want the translation part to be applied, you've got to do a full Vec4 * Mat4 multiplication (use
    your Vec3 as input, but pretend it has a w value of 1.0)
    */
    TVector3<T> operator*(const TVector3<T>& v) const
    {
        TVector4<T> Tmp = Mul({ v, T(1) });
        return {Tmp.x, Tmp.y, Tmp.z};
    }

    TVector4<T> operator*(const TVector4<T>& v) const
    {
        Mul(v);
    }

    TVector4<T> Mul(const TVector4<T>& v) const
    {
        return
        {
            Values[0] * v.x + Values[1] * v.y + Values[2] * v.z + Values[3] * v.w,
            Values[4] * v.x + Values[5] * v.y + Values[6] * v.z + Values[7] * v.w,
            Values[8] * v.x + Values[9] * v.y + Values[10] * v.z + Values[11] * v.w,
            Values[12] * v.x + Values[13] * v.y + Values[14] * v.z + Values[15] * v.w,
        };
    }

    TVector4<T> Column(const std::size_t idx) const
    {
        return { Values[4 * idx + 0],  Values[4 * idx + 1],  Values[4 * idx + 2],  Values[4 * idx + 3] };
    }

    TMatrix4<T> operator*(const TMatrix4<T>& o) const
    {
        // GLM
        /*TVector4<T> SrcA0 = Column(0);
        TVector4<T> SrcA1 = Column(1);
        TVector4<T> SrcA2 = Column(2);
        TVector4<T> SrcA3 = Column(3);

        TVector4<T> SrcB0 = o.Column(0);
        TVector4<T> SrcB1 = o.Column(1);
        TVector4<T> SrcB2 = o.Column(2);
        TVector4<T> SrcB3 = o.Column(3);

        TVector4<T> r0 = SrcA0 * SrcB0[0] + SrcA1 * SrcB0[1] + SrcA2 * SrcB0[2] + SrcA3 * SrcB0[3];
        TVector4<T> r1 = SrcA0 * SrcB1[0] + SrcA1 * SrcB1[1] + SrcA2 * SrcB1[2] + SrcA3 * SrcB1[3];
        TVector4<T> r2 = SrcA0 * SrcB2[0] + SrcA1 * SrcB2[1] + SrcA2 * SrcB2[2] + SrcA3 * SrcB2[3];
        TVector4<T> r3 = SrcA0 * SrcB3[0] + SrcA1 * SrcB3[1] + SrcA2 * SrcB3[2] + SrcA3 * SrcB3[3];


        return { r0.x, r1.x, r2.x, r3.x,
            r0.y, r1.y, r2.y, r3.y,
            r0.z, r1.z, r2.z, r3.z,
            r0.w, r1.w, r2.w, r3.w,
        };*/

        // Mono Game
        TMatrix4<T> result;

        result[0] = Values[0] * o.Values[0] + Values[1] * o.Values[4] + Values[2] * o.Values[8] + Values[3] * o.Values[12];
        result[1] = Values[0] * o.Values[1] + Values[1] * o.Values[5] + Values[2] * o.Values[9] + Values[3] * o.Values[13];
        result[2] = Values[0] * o.Values[2] + Values[1] * o.Values[6] + Values[2] * o.Values[10] + Values[3] * o.Values[14];
        result[3] = Values[0] * o.Values[3] + Values[1] * o.Values[7] + Values[2] * o.Values[11] + Values[3] * o.Values[15];

        result[4] = Values[4] * o.Values[0] + Values[5] * o.Values[4] + Values[6] * o.Values[8] + Values[7] * o.Values[12];
        result[5] = Values[4] * o.Values[1] + Values[5] * o.Values[5] + Values[6] * o.Values[9] + Values[7] * o.Values[13];
        result[6] = Values[4] * o.Values[2] + Values[5] * o.Values[6] + Values[6] * o.Values[10] + Values[7] * o.Values[14];
        result[7] = Values[4] * o.Values[3] + Values[5] * o.Values[7] + Values[6] * o.Values[11] + Values[7] * o.Values[15];

        result[8] = Values[8] * o.Values[0] + Values[9] * o.Values[4] + Values[10] * o.Values[8] + Values[11] * o.Values[12];
        result[9] = Values[8] * o.Values[1] + Values[9] * o.Values[5] + Values[10] * o.Values[9] + Values[11] * o.Values[13];
        result[10] = Values[8] * o.Values[2] + Values[9] * o.Values[6] + Values[10] * o.Values[10] + Values[11] * o.Values[14];
        result[11] = Values[8] * o.Values[3] + Values[9] * o.Values[7] + Values[10] * o.Values[11] + Values[11] * o.Values[15];

        result[12] = Values[12] * o.Values[0] + Values[13] * o.Values[4] + Values[14] * o.Values[8] + Values[15] * o.Values[12];
        result[13] = Values[12] * o.Values[1] + Values[13] * o.Values[5] + Values[14] * o.Values[9] + Values[15] * o.Values[13];
        result[14] = Values[12] * o.Values[2] + Values[13] * o.Values[6] + Values[14] * o.Values[10] + Values[15] * o.Values[14];
        result[15] = Values[12] * o.Values[3] + Values[13] * o.Values[7] + Values[14] * o.Values[11] + Values[15] * o.Values[15];

        return result;
        /*return
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
        };*/
    }

    T* Data() { return Values; }
    const T* Data() const { return Values; }

    static TMatrix4<T> IDENTITY;
private:
    // https://www.euclideanspace.com/maths/algebra/matrix/functions/inverse/fourD/index.htm
    void InverseHelper(const TMatrix4<T>& D)
    {
        T inv[16], det;
        const T* m = D.Data();

        inv[0] = m[5] * m[10] * m[15] -
            m[5] * m[11] * m[14] -
            m[9] * m[6] * m[15] +
            m[9] * m[7] * m[14] +
            m[13] * m[6] * m[11] -
            m[13] * m[7] * m[10];

        inv[4] = -m[4] * m[10] * m[15] +
            m[4] * m[11] * m[14] +
            m[8] * m[6] * m[15] -
            m[8] * m[7] * m[14] -
            m[12] * m[6] * m[11] +
            m[12] * m[7] * m[10];

        inv[8] = m[4] * m[9] * m[15] -
            m[4] * m[11] * m[13] -
            m[8] * m[5] * m[15] +
            m[8] * m[7] * m[13] +
            m[12] * m[5] * m[11] -
            m[12] * m[7] * m[9];

        inv[12] = -m[4] * m[9] * m[14] +
            m[4] * m[10] * m[13] +
            m[8] * m[5] * m[14] -
            m[8] * m[6] * m[13] -
            m[12] * m[5] * m[10] +
            m[12] * m[6] * m[9];

        inv[1] = -m[1] * m[10] * m[15] +
            m[1] * m[11] * m[14] +
            m[9] * m[2] * m[15] -
            m[9] * m[3] * m[14] -
            m[13] * m[2] * m[11] +
            m[13] * m[3] * m[10];

        inv[5] = m[0] * m[10] * m[15] -
            m[0] * m[11] * m[14] -
            m[8] * m[2] * m[15] +
            m[8] * m[3] * m[14] +
            m[12] * m[2] * m[11] -
            m[12] * m[3] * m[10];

        inv[9] = -m[0] * m[9] * m[15] +
            m[0] * m[11] * m[13] +
            m[8] * m[1] * m[15] -
            m[8] * m[3] * m[13] -
            m[12] * m[1] * m[11] +
            m[12] * m[3] * m[9];

        inv[13] = m[0] * m[9] * m[14] -
            m[0] * m[10] * m[13] -
            m[8] * m[1] * m[14] +
            m[8] * m[2] * m[13] +
            m[12] * m[1] * m[10] -
            m[12] * m[2] * m[9];

        inv[2] = m[1] * m[6] * m[15] -
            m[1] * m[7] * m[14] -
            m[5] * m[2] * m[15] +
            m[5] * m[3] * m[14] +
            m[13] * m[2] * m[7] -
            m[13] * m[3] * m[6];

        inv[6] = -m[0] * m[6] * m[15] +
            m[0] * m[7] * m[14] +
            m[4] * m[2] * m[15] -
            m[4] * m[3] * m[14] -
            m[12] * m[2] * m[7] +
            m[12] * m[3] * m[6];

        inv[10] = m[0] * m[5] * m[15] -
            m[0] * m[7] * m[13] -
            m[4] * m[1] * m[15] +
            m[4] * m[3] * m[13] +
            m[12] * m[1] * m[7] -
            m[12] * m[3] * m[5];

        inv[14] = -m[0] * m[5] * m[14] +
            m[0] * m[6] * m[13] +
            m[4] * m[1] * m[14] -
            m[4] * m[2] * m[13] -
            m[12] * m[1] * m[6] +
            m[12] * m[2] * m[5];

        inv[3] = -m[1] * m[6] * m[11] +
            m[1] * m[7] * m[10] +
            m[5] * m[2] * m[11] -
            m[5] * m[3] * m[10] -
            m[9] * m[2] * m[7] +
            m[9] * m[3] * m[6];

        inv[7] = m[0] * m[6] * m[11] -
            m[0] * m[7] * m[10] -
            m[4] * m[2] * m[11] +
            m[4] * m[3] * m[10] +
            m[8] * m[2] * m[7] -
            m[8] * m[3] * m[6];

        inv[11] = -m[0] * m[5] * m[11] +
            m[0] * m[7] * m[9] +
            m[4] * m[1] * m[11] -
            m[4] * m[3] * m[9] -
            m[8] * m[1] * m[7] +
            m[8] * m[3] * m[5];

        inv[15] = m[0] * m[5] * m[10] -
            m[0] * m[6] * m[9] -
            m[4] * m[1] * m[10] +
            m[4] * m[2] * m[9] +
            m[8] * m[1] * m[6] -
            m[8] * m[2] * m[5];

        det = m[0] * inv[0] + m[1] * inv[4] + m[2] * inv[8] + m[3] * inv[12];

        if (det == T(0))
            return;

        det = T(1) / det;

        for (int i = 0; i < 16; ++i)
            Values[i] = inv[i]*det;


        /*T det = D.Determinant();
        if (det == T(0)) return;
        det = T(1) / det;

        Values[0] = D.Values[6] * D.Values[11] * D.Values[13] - D.Values[7] * D.Values[10] * D.Values[13] + D.Values[7] * D.Values[9] * D.Values[14] - D.Values[5] * D.Values[11] * D.Values[14] - D.Values[6] * D.Values[9] * D.Values[15] + D.Values[5] * D.Values[10] * D.Values[15];
        Values[1] = D.Values[3] * D.Values[10] * D.Values[13] - D.Values[2] * D.Values[11] * D.Values[13] - D.Values[3] * D.Values[9] * D.Values[14] + D.Values[1] * D.Values[11] * D.Values[14] + D.Values[2] * D.Values[9] * D.Values[15] - D.Values[1] * D.Values[10] * D.Values[15];
        Values[2] = D.Values[2] * D.Values[7] * D.Values[13] - D.Values[3] * D.Values[6] * D.Values[13] + D.Values[3] * D.Values[5] * D.Values[14] - D.Values[1] * D.Values[7] * D.Values[14] - D.Values[2] * D.Values[5] * D.Values[15] + D.Values[1] * D.Values[6] * D.Values[15];
        Values[3] = D.Values[3] * D.Values[6] * D.Values[9] - D.Values[2] * D.Values[7] * D.Values[9] - D.Values[3] * D.Values[5] * D.Values[10] + D.Values[1] * D.Values[7] * D.Values[10] + D.Values[2] * D.Values[5] * D.Values[11] - D.Values[1] * D.Values[6] * D.Values[11];
        Values[4] = D.Values[7] * D.Values[10] * D.Values[12] - D.Values[6] * D.Values[11] * D.Values[12] - D.Values[7] * D.Values[8] * D.Values[14] + D.Values[4] * D.Values[11] * D.Values[14] + D.Values[6] * D.Values[8] * D.Values[15] - D.Values[4] * D.Values[10] * D.Values[15];
        Values[5] = D.Values[2] * D.Values[11] * D.Values[12] - D.Values[3] * D.Values[10] * D.Values[12] + D.Values[3] * D.Values[8] * D.Values[14] - D.Values[0] * D.Values[11] * D.Values[14] - D.Values[2] * D.Values[8] * D.Values[15] + D.Values[0] * D.Values[10] * D.Values[15];
        Values[6] = D.Values[3] * D.Values[6] * D.Values[12] - D.Values[2] * D.Values[7] * D.Values[12] - D.Values[3] * D.Values[4] * D.Values[14] + D.Values[0] * D.Values[7] * D.Values[14] + D.Values[2] * D.Values[4] * D.Values[15] - D.Values[0] * D.Values[6] * D.Values[15];
        Values[7] = D.Values[2] * D.Values[7] * D.Values[8] - D.Values[3] * D.Values[6] * D.Values[8] + D.Values[3] * D.Values[4] * D.Values[10] - D.Values[0] * D.Values[7] * D.Values[10] - D.Values[2] * D.Values[4] * D.Values[11] + D.Values[0] * D.Values[6] * D.Values[11];
        Values[8] = D.Values[5] * D.Values[11] * D.Values[12] - D.Values[7] * D.Values[9] * D.Values[12] + D.Values[7] * D.Values[8] * D.Values[13] - D.Values[4] * D.Values[11] * D.Values[13] - D.Values[5] * D.Values[8] * D.Values[15] + D.Values[4] * D.Values[9] * D.Values[15];
        Values[9] = D.Values[3] * D.Values[9] * D.Values[12] - D.Values[1] * D.Values[11] * D.Values[12] - D.Values[3] * D.Values[8] * D.Values[13] + D.Values[0] * D.Values[11] * D.Values[13] + D.Values[1] * D.Values[8] * D.Values[15] - D.Values[0] * D.Values[9] * D.Values[15];
        Values[10] = D.Values[1] * D.Values[7] * D.Values[12] - D.Values[3] * D.Values[5] * D.Values[12] + D.Values[3] * D.Values[4] * D.Values[13] - D.Values[0] * D.Values[7] * D.Values[13] - D.Values[1] * D.Values[4] * D.Values[15] + D.Values[0] * D.Values[5] * D.Values[15];
        Values[11] = D.Values[3] * D.Values[5] * D.Values[8] - D.Values[1] * D.Values[7] * D.Values[8] - D.Values[3] * D.Values[4] * D.Values[9] + D.Values[0] * D.Values[7] * D.Values[9] + D.Values[1] * D.Values[4] * D.Values[11] - D.Values[0] * D.Values[5] * D.Values[11];
        Values[12] = D.Values[6] * D.Values[9] * D.Values[12] - D.Values[5] * D.Values[10] * D.Values[12] - D.Values[6] * D.Values[8] * D.Values[13] + D.Values[4] * D.Values[10] * D.Values[13] + D.Values[5] * D.Values[8] * D.Values[14] - D.Values[4] * D.Values[9] * D.Values[14];
        Values[13] = D.Values[1] * D.Values[10] * D.Values[12] - D.Values[2] * D.Values[9] * D.Values[12] + D.Values[2] * D.Values[8] * D.Values[13] - D.Values[0] * D.Values[10] * D.Values[13] - D.Values[1] * D.Values[8] * D.Values[14] + D.Values[0] * D.Values[9] * D.Values[14];
        Values[14] = D.Values[2] * D.Values[5] * D.Values[12] - D.Values[1] * D.Values[6] * D.Values[12] - D.Values[2] * D.Values[4] * D.Values[13] + D.Values[0] * D.Values[6] * D.Values[13] + D.Values[1] * D.Values[4] * D.Values[14] - D.Values[0] * D.Values[5] * D.Values[14];
        Values[15] = D.Values[1] * D.Values[6] * D.Values[8] - D.Values[2] * D.Values[5] * D.Values[8] + D.Values[2] * D.Values[4] * D.Values[9] - D.Values[0] * D.Values[6] * D.Values[9] - D.Values[1] * D.Values[4] * D.Values[10] + D.Values[0] * D.Values[5] * D.Values[10];

        for (int i = 0; i < 16; ++i)
        {
            Values[i] *= det;
        }*/
    }
private:
    /*
    * 0  1  2  3
    * 4  5  6  7
    * 8  9  10 11
    * 12 13 14 15
    */
    T Values[16];
};

template<class T> TMatrix4<T> TMatrix4<T>::IDENTITY = {};

using Matrix4 = TMatrix4<float>;
using IntMatrix4 = TMatrix4<int>;

namespace Math
{
    template<class T>
    TMatrix4<T> Lerp(const TMatrix4<T>& a, const TMatrix4<T>& b, const T v)
    {
        TMatrix4<T> R;
        for(int i = 0; i < 16; ++i)
        {
            R[i] = Lerp( a[i], b[i], v );
        }
        return R;
    }

    template<class T>
    TMatrix4<T> Scale(const T x)
    {
        TMatrix4<T> R;
        R[0] = x;
        R[5] = x;
        R[10] = x;
        return R;
    }

    template<class T>
    TMatrix4<T> Scale(const T x, const T y, const T z)
    {
        TMatrix4<T> R;
        R[0] = x;
        R[5] = y;
        R[10] = z;
        return R;
    }

    template<class T>
    TMatrix4<T> Scale(const TVector3<T>& Scale)
    {
        TMatrix4<T> R;
        R[0] = Scale.x;
        R[5] = Scale.y;
        R[10] = Scale.z;
        return R;
    }

    template<class T>
    TMatrix4<T> Translation(const T x, const T y, const T z)
    {
        TMatrix4<T> R;
        R[12] = x;
        R[13] = y;
        R[14] = z;
        return R;
    }

    template<class T>
    TMatrix4<T> Translation(const TVector3<T>& Translation)
    {
        TMatrix4<T> R;
        R[12] = Translation.x;
        R[13] = Translation.y;
        R[14] = Translation.z;
        return R;
    }

    template<class T>
    TMatrix4<T> RotationX(const T Angle)
    {
        T SinF = Sin(DegToRad(Angle));
        T CosF = Cos(DegToRad(Angle));

        TMatrix4<T> R;
        R[5] = CosF;
        R[6] = -SinF;
        R[9] = SinF;
        R[10] = CosF;
        return R;
    }

    template<class T>
    TMatrix4<T> RotationY(const T Angle)
    {
        T SinF = Sin(DegToRad(Angle));
        T CosF = Cos(DegToRad(Angle));

        TMatrix4<T> R;
        R[0] = CosF;
        R[2] = SinF;
        R[8] = -SinF;
        R[10] = CosF;
        return R;
    }

    template<class T>
    TMatrix4<T> RotationZ(const T Angle)
    {
        T SinF = Sin(DegToRad(Angle));
        T CosF = Cos(DegToRad(Angle));

        TMatrix4<T> R;
        R[0] = CosF;
        R[1] = -SinF;
        R[4] = SinF;
        R[5] = CosF;
        return R;
    }

    // Angle Axis
    template<class T>
    TMatrix4<T> Rotation(const TVector3<T>& v, const T k)
    {
        T a = DegToRad(k);
        T c = Cos(a);
        T s = Sin(a);
        T t = T(1) - c;
        TVector3<T> a1 = Normalize(v);

        TMatrix4<T> Rot;

        Rot[0] = c + a1.x * a1.x * t;
        Rot[5] = c + a1.y * a1.y * t;
        Rot[10] = c + a1.z * a1.z * t;

        T tmp1 = a1.x * a1.y * t;
        T tmp2 = a1.z * s;

        Rot[4] = tmp1 + tmp2;
        Rot[1] = tmp1 - tmp2;

        tmp1 = a1.x * a1.z * t;
        tmp2 = a1.y * s;

        Rot[8] = tmp1 - tmp2;
        Rot[2] = tmp1 + tmp2;    
        
        tmp1 = a1.y * a1.z * t;
        tmp2 = a1.x * s;

        Rot[9] = tmp1 + tmp2;
        Rot[6] = tmp1 - tmp2;

        return Rot;
    }

    // From GLM Library, World Space Arguments
    template<class T>
    TMatrix4<T> LookAt(const TVector3<T>& Camera, const TVector3<T>& Position, const TVector3<T>& Up)
    {
        TVector3<T> f = Normalize(Position - Camera);
        TVector3<T> s = Normalize(CrossProduct(Up, f)); // f, Up For RH
        TVector3<T> u = CrossProduct(f, s); // s, f For RH

        TMatrix4<T> R;

        R[0] = s.x;
        R[4]= s.y;
        R[8] = s.z;
        R[1] = u.x;
        R[5] = u.y;
        R[9] = u.z;
        R[2] = f.x; // - For RH
        R[6] = f.y; // - For RH
        R[10] = f.z; // - For RH
        R[12] = -DotProduct(s, Camera);
        R[13] = -DotProduct(u, Camera);
        R[14] = -DotProduct(f, Camera); // + For RH

        return R;
    }

    // https://github.com/g-truc/glm/blob/master/glm/ext/matrix_clip_space.inl
    template<class T>
    TMatrix4<T> Perspective(const T Fov, const T Aspect, const T Near, const T Far)
    {
        T HalfTan = Tan(DegToRad(Fov) / T(2));

        TMatrix4<T> Result;

        Result[0] = T(1) / (Aspect * HalfTan);
        Result[5] = T(1) / (HalfTan);
        Result[10] = (Far + Near) / (Far - Near); // - For RH
        Result[11] = T(1); // - For RH
        Result[14] = -(T(2) * Far * Near) / (Far - Near);
        Result[15] = T(0);

        return Result;
    }

    static constexpr float Z_FAR = -1000000.0f;
    static constexpr float Z_NEAR = +1000000.0f;

    // https://en.wikipedia.org/wiki/Orthographic_projection
    template<class T>
    TMatrix4<T> Ortho(const T Right, const T Left, const T Top, const T Bottom, const T Far, const T Near)
    {
        TMatrix4<T> R;

        R[0] = T(2) / (Right - Left);
        R[5] = T(2) / (Top - Bottom);
        R[10] = T(2) / (Far - Near); // - For RH
        R[12] = -(Right + Left) / (Right - Left);
        R[13] = -(Top + Bottom) / (Top - Bottom);
        R[14] = -(Far + Near) / (Far - Near);

        return R;
    }

    template<class T>
    TMatrix4<T> Ortho(const T Right, const T Left, const T Top, const T Bottom)
    {
        return Ortho(Right, Left, Top, Bottom, Z_FAR, Z_NEAR);
    }

    template<class T>
    TMatrix4<T> Ortho(const T Right, const T Bottom)
    {
        return Ortho(Right, 0.0f, 0.0f, Bottom, Z_FAR, Z_NEAR);
    }
}

template<class T>
std::ostream& operator<<(std::ostream& Stream, const TMatrix4<T>& Other)
{
    Stream << Other[0] << " " << Other[1] << " " << Other[2] << " " << Other[3] << "\n";
    Stream << Other[4] << " " << Other[5] << " " << Other[6] << " " << Other[7] << "\n";
    Stream << Other[8] << " " << Other[9] << " " << Other[10] << " " << Other[11] << "\n";
    Stream << Other[12] << " " << Other[13] << " " << Other[14] << " " << Other[15];
    return Stream;
}
