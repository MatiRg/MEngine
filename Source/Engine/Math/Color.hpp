#pragma once
#include "Functions.hpp"
#include <ostream>

class Color
{
public:
     Color(const float Val = 0.0f):
        r( Val ), g( Val ), b( Val ), a( Val )
    {
        Clamp();
    }

    Color(const float Val, const float Alpha):
        r( Val ), g( Val ), b( Val ), a( Alpha )
    {
        Clamp();
    }

    Color(const float Val1, const float Val2, const float Val3, const float Val4 = 1.0f):
        r( Val1 ), g( Val2 ), b( Val3 ), a( Val4 )
    {
        Clamp();
    }

    Color(const Color& Val):
        r( Val.r ), g( Val.g ), b( Val.b ), a( Val.a )
    {
    }

    ~Color() {}

    Color& operator=(const Color& Other)
    {
        if( this != &Other)
        {
            r = Other.r;
            g = Other.g;
            b = Other.b;
            a = Other.a;
        }
        return *this;
    }

    void Clamp()
    {
        r = Math::Clamp( r, 0.0f, 1.0f );
        g = Math::Clamp( g, 0.0f, 1.0f );
        b = Math::Clamp( b, 0.0f, 1.0f );
        a = Math::Clamp( a, 0.0f, 1.0f );
    }

    float GetR() const { return r; }
    uint8_t GetR_UInt() const { return static_cast<uint8_t>(r*255.0f); }
    void SetR(const float x) { r = Math::Clamp( x, 0.0f, 1.0f ); }

    float GetG() const { return g; }
    uint8_t GetG_UInt() const { return static_cast<uint8_t>(g*255.0f); }
    void SetG(const float x) { g = Math::Clamp( x, 0.0f, 1.0f ); }

    float GetB() const { return b; }
    uint8_t GetB_UInt() const { return static_cast<uint8_t>(b*255.0f); }
    void SetB(const float x) { b = Math::Clamp( x, 0.0f, 1.0f ); }

    float GetA() const { return a; }
    uint8_t GetA_UInt() const { return static_cast<uint8_t>(a*255.0f); }
    void SetA(const float x) { a = Math::Clamp( x, 0.0f, 1.0f ); }

    static Color WHITE;
    static Color BLACK;
    static Color RED;
    static Color GREEN;
    static Color BLUE;
    static Color GREY;
    static Color YELLOW;
    static Color CYAN;
    static Color PINK;
    static Color MAROON;
private:
    float r, g, b, a;
};

namespace Math
{
    inline Color Lerp(const Color& a, const Color& b, const float v)
    {
        Color R(
            Lerp(a.GetR(), b.GetR(), v),
            Lerp(a.GetG(), b.GetG(), v),
            Lerp(a.GetB(), b.GetB(), v),
            Lerp(a.GetA(), b.GetA(), v)
        );
        return R;
    }
}

inline std::ostream& operator<<(std::ostream& Stream, const Color& Other)
{
    Stream << Other.GetR() << ", " << Other.GetG() << ", " << Other.GetB() << ", " << Other.GetA();
    return Stream;
}
