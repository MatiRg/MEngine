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
    your Vec3 as input, but pretend it has a [font="'Lucida Console"]w[/font] value of 1.0)
    */
    TVector3<T> operator*(const TVector3<T>& v) const
    {
        return
        {
            Values[0]*v.x + Values[1]*v.y + Values[2]*v.z + Values[3],
            Values[4]*v.x + Values[5]*v.y + Values[6]*v.z + Values[7],
            Values[8]*v.x + Values[9]*v.y + Values[10]*v.z + Values[11]
        };
    }

    TVector4<T> operator*(const TVector4<T>& v) const
    {
        return
        {
            Values[0]*v.x + Values[1]*v.y + Values[2]*v.z + Values[3]*v.w,
            Values[4]*v.x + Values[5]*v.y + Values[6]*v.z + Values[7]*v.w,
            Values[8]*v.x + Values[9]*v.y + Values[10]*v.z + Values[11]*v.w,
            Values[12]*v.x + Values[13]*v.y + Values[14]*v.z + Values[15]*v.w,
        };
    }

    TMatrix4<T> operator*(const TMatrix4<T>& o) const
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

    T* Data() { return Values; }
    const T* Data() const { return Values; }

    static TMatrix4<T> IDENTITY() { return TMatrix4<T>(); }
private:
    T Values[16];
};

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

    // https://en.wikipedia.org/wiki/Orthographic_projection
    template<class T>
    TMatrix4<T> Ortho(const T Right, const T Left, const T Top, const T Bottom, const T Far, const T Near)
    {
        TMatrix4<T> R;

        R[0] = T(2) / (Right-Left);
        R[3] = -((Right+Left) / (Right-Left));

        R[5] = T(2) / (Top-Bottom);
        R[7] = -((Top+Bottom) / (Top-Bottom));

        R[10] = T(-2) / (Far-Near);
        R[11] = -((Far+Near) / (Far-Near));

        return R;
    }

    static constexpr float Z_FAR = -1000000.0f;
    static constexpr float Z_NEAR = +1000000.0f;

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
    Stream << Other[12] << " " << Other[13] << " " << Other[14] << " " << Other[15] << "\n";
    return Stream;
}
