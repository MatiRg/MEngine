#include "Matrix3.hpp"

void Matrix3::SetIdentity()
{
    for(int i = 0; i < 3; ++i)
    {
        for(int j = 0; j < 3; ++j)
        {
            if( i == j ) Values[i*3+j] = 1.0f;
            else  Values[i*3+j] = 0.0f;
        }
    }
}

Matrix3 Matrix3::Inverse() const
{
    return Math::Inverse(*this);
}

void Matrix3::Invert()
{
    *this = Inverse();
}

Matrix3 Matrix3::Transposition() const
{
    return Math::Transpose(*this);
}

void Matrix3::Transpose()
{
    *this = Transposition();
}

Matrix3 Matrix3::operator*(const Matrix3& o) const
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

Matrix3 Matrix3::IDENTITY = {};

namespace Math
{
    Matrix3 Transpose(const Matrix3& Mat)
    {
        Matrix3 D;
        for(int i = 0; i < 3; ++i)
        {
            for(int j = 0; j < 3; ++j)
            {
                D[j*3+i] = Mat[i*3+j];
            }
        }
        return D;
    }

    Matrix3 Inverse(const Matrix3& D)
    {
			float OneOverDeterminant = 1.0f / (
				+ D[0] * (D[4] * D[8] - D[7] * D[5])
				- D[3] * (D[1] * D[8] - D[7] * D[2])
				+ D[6] * (D[1] * D[5] - D[4] * D[2]));

			Matrix3 Inverse;
			Inverse[0] = + (D[4] * D[8] - D[7] * D[5]) * OneOverDeterminant;
			Inverse[3] = - (D[3] * D[8] - D[6] * D[5]) * OneOverDeterminant;
			Inverse[6] = + (D[3] * D[7] - D[6] * D[4]) * OneOverDeterminant;
			Inverse[1] = - (D[1] * D[8] - D[7] * D[2]) * OneOverDeterminant;
			Inverse[4] = + (D[0] * D[8] - D[6] * D[2]) * OneOverDeterminant;
			Inverse[7] = - (D[0] * D[7] - D[6] * D[1]) * OneOverDeterminant;
			Inverse[2] = + (D[1] * D[5] - D[4] * D[2]) * OneOverDeterminant;
			Inverse[5] = - (D[0] * D[5] - D[3] * D[2]) * OneOverDeterminant;
			Inverse[8] = + (D[0] * D[4] - D[3] * D[1]) * OneOverDeterminant;
			return Inverse;
    }

    Matrix3 Lerp(const Matrix3& a, const Matrix3& b, const float t)
    {
        Matrix3 R;
        for(int i = 0; i < 9; ++i)
        {
            R[i] = Lerp( a[i], b[i], t );
        }
        return R;
    }
}

std::ostream& operator<<(std::ostream& Stream, const Matrix3& Other)
{
    Stream << Other[0] << " " << Other[1] << " " << Other[2] << "\n";
    Stream << Other[3] << " " << Other[4] << " " << Other[5] << "\n";
    Stream << Other[6] << " " << Other[7] << " " << Other[8];
    return Stream;
}
