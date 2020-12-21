#pragma once
#include "Vector2.hpp"
#include "Vector3.hpp"
#include "Functions.hpp"

template<class T>
class TMatrix3
{
public:
    TMatrix3()
    {
        Identity();
    }

    TMatrix3(const T a1, const T a2, const T a3,
            const T a4, const T a5, const T a6,
            const T a7, const T a8, const T a9):
                Values{ a1, a2, a3, a4, a5, a6, a7, a8, a9 }
    {
    }

    TMatrix3(const T* D)
    {
        for(int i = 0; i < 9; ++i) 
        {
            Values[i] = D[i];
        }
    }

    TMatrix3(const TMatrix3<T>& D)
    {
        for(int i = 0; i < 9; ++i) 
        {
            Values[i] = D.Values[i];
        }
    }

    ~TMatrix3()
    {
    }

    void Identity()
    {
        for(int i = 0; i < 3; ++i)
        {
            for(int j = 0; j < 3; ++j)
            {
                if( i == j ) Values[i*3+j] = T(1);
                else  Values[i*3+j] = T(0);
            }
        }
    }

    T Determinant() const
    {
        T t4 = Values[0]*Values[4];
        T t6 = Values[0]*Values[5];
        T t8 = Values[1]*Values[3];
        T t10 = Values[2]*Values[3];
        T t12 = Values[1]*Values[6];
        T t14 = Values[2]*Values[6];

        T t16 = (t4*Values[8] - t6*Values[7] - t8*Values[8] + t10*Values[7] + t12*Values[5] - t14*Values[4]);

        return t16;
    }

    TMatrix3<T> Inverse() const
    {
        TMatrix3<T> Mat;
        Mat.InverseHelper( *this );
        return Mat;
    }

    void Invert()
    {
        InverseHelper( *this );
    }

    TMatrix3<T> Transposition() const
    {
        TMatrix3<T> D;
        for(int i = 0; i < 3; ++i)
        {
            for(int j = 0; j < 3; ++j)
            {
                D.Values[j*3+i] = Values[i*3+j];
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

    TMatrix3<T>& operator=(const TMatrix3<T>& D)
    {
        if( this == &D )
        {
            return *this;
        }
        for(int i = 0; i < 9; ++i) 
        {
            Values[i] = D.Values[i];
        }
        return *this;
    }

    TVector3<T> operator*(const TVector3<T>& v) const
    {
        return
        {
            Values[0]*v.x + Values[1]*v.y + Values[2]*v.z,
            Values[3]*v.x + Values[4]*v.y + Values[5]*v.z,
            Values[6]*v.x + Values[7]*v.y + Values[8]*v.z
        };
    }

    TMatrix3<T> operator*(const TMatrix3<T>& o) const
    {
        return
        {
            Values[0]*o.Values[0] + Values[1]*o.Values[3] + Values[2]*o.Values[6],
            Values[0]*o.Values[1] + Values[1]*o.Values[4] + Values[2]*o.Values[7],
            Values[0]*o.Values[2] + Values[1]*o.Values[5] + Values[2]*o.Values[8],

            Values[3]*o.Values[0] + Values[4]*o.Values[3] + Values[5]*o.Values[6],
            Values[3]*o.Values[1] + Values[4]*o.Values[4] + Values[5]*o.Values[7],
            Values[3]*o.Values[2] + Values[4]*o.Values[5] + Values[5]*o.Values[8],
            
            Values[6]*o.Values[0] + Values[7]*o.Values[3] + Values[8]*o.Values[6],
            Values[6]*o.Values[1] + Values[7]*o.Values[4] + Values[8]*o.Values[7],
            Values[6]*o.Values[2] + Values[7]*o.Values[5] + Values[8]*o.Values[8]
        };
    }

    T* Data() { return Values; }
    const T* Data() const { return Values; }

    static TMatrix3<T> IDENTITY;
private:
    void InverseHelper(const TMatrix3<T>& D)
    {
        // Calculate the determinant.
        T t17 = D.Determinant();

        // Make sure the determinant is non-zero.
        if( t17 == T(0) ) return;
        t17 = T(1)/t17;

        T t4 = D.Values[0]*D.Values[4];
        T t6 = D.Values[0]*D.Values[5];
        T t8 = D.Values[1]*D.Values[3];
        T t10 = D.Values[2]*D.Values[3];
        T t12 = D.Values[1]*D.Values[6];
        T t14 = D.Values[2]*D.Values[6];

        Values[0] = (D.Values[4]*D.Values[8]-D.Values[5]*D.Values[7])*t17;
        Values[1] = -(D.Values[1]*D.Values[8]-D.Values[2]*D.Values[7])*t17;
        Values[2] = (D.Values[1]*D.Values[5]-D.Values[2]*D.Values[4])*t17;
        Values[3] = -(D.Values[3]*D.Values[8]-D.Values[5]*D.Values[6])*t17;
        Values[4] = (D.Values[0]*D.Values[8]-t14)*t17;
        Values[5] = -(t6-t10)*t17;
        Values[6] = (D.Values[3]*D.Values[7]-D.Values[4]*D.Values[6])*t17;
        Values[7] = -(D.Values[0]*D.Values[7]-t12)*t17;
        Values[8] = (t4-t8)*t17;
    }
private:
    /*
    * 0 1 2
    * 3 4 5
    * 6 7 8
    */
    T Values[9];
};

template<class T> TMatrix3<T> TMatrix3<T>::IDENTITY = {};

using Matrix3 = TMatrix3<float>;
using IntMatrix3 = TMatrix3<int>;

namespace Math
{
    template<class T>
    TMatrix3<T> Lerp(const TMatrix3<T>& a, const TMatrix3<T>& b, const T v)
    {
        TMatrix3<T> R;
        for(int i = 0; i < 9; ++i)
        {
            R[i] = Lerp( a[i], b[i], v );
        }
        return R;
    }
}

template<class T>
std::ostream& operator<<(std::ostream& Stream, const TMatrix3<T>& Other)
{
    Stream << Other[0] << " " << Other[1] << " " << Other[2] << "\n";
    Stream << Other[3] << " " << Other[4] << " " << Other[5] << "\n";
    Stream << Other[6] << " " << Other[7] << " " << Other[8];
    return Stream;
}
