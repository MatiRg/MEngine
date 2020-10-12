#pragma once
#include "Vector2.hpp"

template<class T>
class TRect2
{
public:
    TRect2()
    {
    }

    TRect2(const T X, const T Y, const T W, const T H):
        Pos( X, Y ),
        Size( Math::Max(W, T(0)), Math::Max(H, T(0)) )
    {
    }

    TRect2(const TVector2<T>& P, const TVector2<T>& S):
        Pos( P ),
        Size( Math::Max(S.x, T(0)), Math::Max(S.y, T(0)) )
    {
    }

    TRect2(const TRect2<T>& Other):
        Pos( Other.Pos ),
        Size( Other.Size )
    {
    }

    ~TRect2() {}

    TRect2<T>& operator=(const TRect2<T>& Other)
    {
        if( this != &Other )
        {
            Pos = Other.Pos;
            Size = Other.Size;
        }
        return *this;
    }

    void SetPosition(const TVector2<T>& Position)
    {
        Pos = Position;
    }

    const TVector2<T>& GetPosition() const
    {
        return Pos;
    }

    void SetSize(const TVector2<T>& S)
    {
        Size = S;
    }

    const TVector2<T>& GetSize() const
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

    void Move(const TVector2<T>& By)
    {
        Pos += By;
    }

    void Translate(const TVector2<T>& By)
    {
        Pos -= By;
    }

    bool Intersect(const TVector2<T>& p) const
    {
        return ( (p.x >= GetX() && p.x <= GetMaxX()) && (p.y >= GetY() && p.y <= GetMaxY()) );
    }

    bool Intersect(const TRect2<T>& r2) const
    {
        if( GetMaxX() < r2.GetX() || GetX() > r2.GetMaxX() ) return false;
        if( GetMaxY() < r2.GetY() || GetY() > r2.GetMaxY() ) return false;
        return true;
    }
private:
    TVector2<T> Pos;
    TVector2<T> Size;
};

using Rect2 = TRect2<float>;
using IntRect2 = TRect2<int>;

template<class T>
std::ostream& operator<<(std::ostream& Stream, const TRect2<T>& r)
{
    Stream << r.GetX() << ", " << r.GetY() << " : " << r.GetMaxX() << ", " << r.GetMaxY();
    return Stream;
}
