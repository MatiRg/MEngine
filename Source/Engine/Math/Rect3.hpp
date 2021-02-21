#pragma once
#include "Functions.hpp"
#include "Vector3.hpp"

class Rect3
{
public:
    Rect3()
    {
    }

    Rect3(const float X, const float Y, const float Z, const float W, const float H, const float D):
        Pos( X, Y, Z ),
        Size( Math::Max(W, 0.0f), Math::Max(H, 0.0f), Math::Max(D, 0.0f) )
    {
    }

    Rect3(const Vector3& P, const Vector3& S):
        Pos( P ),
        Size( Math::Max(S.x, 0.0f), Math::Max(S.y, 0.0f), Math::Max(S.z, 0.0f) )
    {
    }

    Rect3(const Rect3& Other):
        Pos( Other.Pos ),
        Size( Other.Size )
    {
    }

    ~Rect3() {}

    Rect3& operator=(const Rect3& Other)
    {
        if( this != &Other )
        {
            Pos = Other.Pos;
            Size = Other.Size;
        }
        return *this;
    }

    void SetPosition(const Vector3& Position)
    {
        Pos = Position;
    }

    const Vector3& GetPosition() const
    {
        return Pos;
    }

    void SetSize(const Vector3& S)
    {
        Size = S;
    }

    const Vector3& GetSize() const
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

    void SetZ(const float v) { Pos.z = v; }
    float GetZ() const { return Pos.z; }
	
	void SetDepth(const float v) { Size.z = Math::Max(v, 0.0f); }
    float GetDepth() const { return Size.z; }
    float GetMaxZ() const { return Pos.z + Size.z; }
	
    void Move(const Vector3& By)
    {
        Pos += By;
    }

    void Translate(const Vector3& By)
    {
        Pos -= By;
    }

    bool Intersect(const Vector3& p) const
    {
        return ( (p.x >= GetX() && p.x <= GetMaxX()) && (p.y >= GetY() && p.y <= GetMaxY()) && (p.z >= GetZ() && p.z <= GetMaxZ()) );
    }

    bool Intersect(const Rect3& r2) const
    {
        if( GetMaxX() < r2.GetX() || GetX() > r2.GetMaxX() ) return false;
        if( GetMaxY() < r2.GetY() || GetY() > r2.GetMaxY() ) return false;
        if( GetMaxZ() < r2.GetZ() || GetZ() > r2.GetMaxZ() ) return false;
        return true;
    }
private:
    Vector3 Pos = Vector3::ZERO;
    Vector3 Size = Vector3::ONE;
};

std::ostream& operator<<(std::ostream&, const Rect3&);
