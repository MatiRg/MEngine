#pragma once
#include "Functions.hpp"
#include "Vector3.hpp"

class Matrix3
{
public:
    Matrix3()
    {
        SetIdentity();
    }

    Matrix3(const float a1, const float a2, const float a3,
            const float a4, const float a5, const float a6,
            const float a7, const float a8, const float a9):
                Values{ a1, a2, a3, a4, a5, a6, a7, a8, a9 }
    {
    }

    Matrix3(const float* D)
    {
        for(int i = 0; i < 9; ++i) 
        {
            Values[i] = D[i];
        }
    }

    Matrix3(const Matrix3& D)
    {
        for(int i = 0; i < 9; ++i) 
        {
            Values[i] = D.Values[i];
        }
    }

    ~Matrix3()
    {
    }

    void SetIdentity();
    Matrix3 Inverse() const;
    void Invert();
    Matrix3 Transposition() const;
    void Transpose();

    float& operator[](const std::size_t idx)
    {
        return Values[idx];
    }

    const float& operator[](const std::size_t idx) const
    {
        return Values[idx];
    }

    Matrix3& operator=(const Matrix3& D)
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

    Vector3 operator*(const Vector3& v) const
    {
        return
        {
            Values[0]*v.x + Values[1]*v.y + Values[2]*v.z,
            Values[3]*v.x + Values[4]*v.y + Values[5]*v.z,
            Values[6]*v.x + Values[7]*v.y + Values[8]*v.z
        };
    }

    Matrix3 operator*(const Matrix3& o) const;

    float* Data() { return Values; }
    const float* Data() const { return Values; }

    static Matrix3 IDENTITY;
private:
    /*
    * 0 1 2
    * 3 4 5
    * 6 7 8
    */
    float Values[9];
};

namespace Math
{
    Matrix3 Transpose(const Matrix3& Mat);
    Matrix3 Inverse(const Matrix3& Mat);
    Matrix3 Lerp(const Matrix3& a, const Matrix3& b, const float v);
}

std::ostream& operator<<(std::ostream& Stream, const Matrix3& Other);
