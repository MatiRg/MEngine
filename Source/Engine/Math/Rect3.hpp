#pragma once
#include "Vector3.hpp"

template<class T>
class TRect3
{
public:
    TRect3()
    {
    }

    TRect3(const T X, const T Y, const T Z, const T W, const T H, const T D):
        Pos( X, Y, Z ),
        Size( Math::Max(W, T(0)), Math::Max(H, T(0)), Math::Max(D, T(0)) )
    {
    }

    TRect3(const TVector3<T>& P, const TVector3<T>& S):
        Pos( P ),
        Size( Math::Max(S.x, T(0)), Math::Max(S.y, T(0)), Math::Max(S.z, T(0)) )
    {
    }

    TRect3(const TRect3<T>& Other):
        Pos( Other.Pos ),
        Size( Other.Size )
    {
    }

    ~TRect3() {}

    TRect3<T>& operator=(const TRect3<T>& Other)
    {
        if( this != &Other )
        {
            Pos = Other.Pos;
            Size = Other.Size;
        }
        return *this;
    }

    void SetPosition(const TVector3<T>& Position)
    {
        Pos = Position;
    }

    const TVector3<T>& GetPosition() const
    {
        return Pos;
    }

    void SetSize(const TVector3<T>& S)
    {
        Size = S;
    }

    const TVector3<T>& GetSize() const
    {
        return Size;
    }

    void SetX(const T v) { Pos.x = v; }
    T GetX() const { return Pos.x; }

    void SetWidth(const T v) { Size.x = Math::Max(v, T(0)); }
    T GetWidth() const { return Size.x; }
    T GetMaxX() const { return Pos.x + Size.x; }

    void SetY(const T v) { Pos.y = v; }
    T GetY() const { return Pos.y; }
	
    void SetHeight(const T v) { Size.y = Math::Max(v, T(0)); }
    T GetHeight() const { return Size.y; }
    T GetMaxY() const { return Pos.y + Size.y; }

    void SetZ(const T v) { Pos.z = v; }
    T GetZ() const { return Pos.z; }
	
	void SetDepth(const T v) { Size.z = Math::Max(v, T(0)); }
    T GetDepth() const { return Size.z; }
    T GetMaxZ() const { return Pos.z + Size.z; }
	
    void Move(const TVector3<T>& By)
    {
        Pos += By;
    }

    void Translate(const TVector3<T>& By)
    {
        Pos -= By;
    }

    bool Intersect(const TVector3<T>& p) const
    {
        return ( (p.x >= GetX() && p.x <= GetMaxX()) && (p.y >= GetY() && p.y <= GetMaxY()) && (p.z >= GetZ() && p.z <= GetMaxZ()) );
    }

    bool Intersect(const TRect3<T>& r2) const
    {
        if( GetMaxX() < r2.GetX() || GetX() > r2.GetMaxX() ) return false;
        if( GetMaxY() < r2.GetY() || GetY() > r2.GetMaxY() ) return false;
        if( GetMaxZ() < r2.GetZ() || GetZ() > r2.GetMaxZ() ) return false;
        return true;
    }
private:
    TVector3<T> Pos;
    TVector3<T> Size;
};

using Rect3 = TRect3<float>;
using InRect3 = TRect3<int>;

template<class T>
std::ostream& operator<<(std::ostream& Stream, const TRect3<T>& r)
{
    Stream << r.GetX() << ", " << r.GetY() << ", " << r.GetZ() << " : " << r.GetMaxX() << ", " << r.GetMaxY() << ", " << r.GetMaxZ();
    return Stream;
}
