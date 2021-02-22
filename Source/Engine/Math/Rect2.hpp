#pragma once
#include "Functions.hpp"
#include "Vector2.hpp"

class Rect2
{
public:
    Rect2()
    {
    }

    Rect2(const float X, const float Y, const float W, const float H):
        Pos( X, Y ),
        Size( Math::Max(W, 0.0f), Math::Max(H, 0.0f) )
    {
    }

    Rect2(const Vector2& P, const Vector2& S):
        Pos( P ),
        Size( Math::Max(S.x, 0.0f), Math::Max(S.y, 0.0f) )
    {
    }

    Rect2(const Rect2& Other):
        Pos( Other.Pos ),
        Size( Other.Size )
    {
    }

    ~Rect2() {}

    Rect2& operator=(const Rect2& Other)
    {
        if( this != &Other )
        {
            Pos = Other.Pos;
            Size = Other.Size;
        }
        return *this;
    }

    void SetPosition(const Vector2& Position)
    {
        Pos = Position;
    }

    const Vector2& GetPosition() const
    {
        return Pos;
    }

    void SetSize(const Vector2& S)
    {
        Size = S;
    }

    const Vector2& GetSize() const
    {
        return Size;
    }

    void SetX(const float v) { Pos.x = v; }
    float GetX() const { return Pos.x; }

    void SetWidth(const float v) { Size.x = Math::Max(v, 0.0f); }
    float GetWidth() const { return Size.x; }
    float GetMaxX() const { return Pos.x + Size.x; }

    void SetY(const float v) { Pos.y = v; }
    float GetY() const { return Pos.y; }

    void SetHeight(const float v) { Size.y = Math::Max(v, 0.0f); }
    float GetHeight() const { return Size.y; }
    float GetMaxY() const { return Pos.y + Size.y; }

    void Move(const Vector2& By)
    {
        Pos += By;
    }

    void Translate(const Vector2& By)
    {
        Pos -= By;
    }

    bool Intersect(const Vector2& p) const
    {
        return ( (p.x >= GetX() && p.x <= GetMaxX()) && (p.y >= GetY() && p.y <= GetMaxY()) );
    }

    bool Intersect(const Rect2& r2) const
    {
        if( GetMaxX() < r2.GetX() || GetX() > r2.GetMaxX() ) return false;
        if( GetMaxY() < r2.GetY() || GetY() > r2.GetMaxY() ) return false;
        return true;
    }
private:
    Vector2 Pos = Vector2::ZERO;
    Vector2 Size = Vector2::ONE;
};

std::ostream& operator<<(std::ostream&, const Rect2&);