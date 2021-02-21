#pragma once
#include "Functions.hpp"
#include <ostream>

class Color
{
public:
    Color(const float Val = 0.0f):
        r( Val ), g( Val ), b( Val ), a( Val )
    {
    }

    Color(const float Val, const float Alpha):
        r( Val ), g( Val ), b( Val ), a( Alpha )
    {
    }

    Color(const float Val1, const float Val2, const float Val3, const float Val4 = 1.0f):
        r( Val1 ), g( Val2 ), b( Val3 ), a( Val4 )
    {
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

    //! Clamp to [0, 1]
    void Clamp()
    {
        r = Math::Clamp( r, 0.0f, 1.0f );
        g = Math::Clamp( g, 0.0f, 1.0f );
        b = Math::Clamp( b, 0.0f, 1.0f );
        a = Math::Clamp( a, 0.0f, 1.0f );
    }

    float GetR() const { return r; }
    void SetR(const float x) { r = x; }

    float GetG() const { return g; }
    void SetG(const float x) { g = x; }

    float GetB() const { return b; }
    void SetB(const float x) { b = x; }

    float GetA() const { return a; }
    void SetA(const float x) { a = x; }

    uint8_t GetR_UInt() const { return static_cast<uint8_t>(r*255.0f); }
    uint8_t GetG_UInt() const { return static_cast<uint8_t>(g*255.0f); }
    uint8_t GetB_UInt() const { return static_cast<uint8_t>(b*255.0f); }
    uint8_t GetA_UInt() const { return static_cast<uint8_t>(a*255.0f); }

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
public:
    float r, g, b, a;
};

namespace Math
{
    Color Lerp(const Color& a, const Color& b, const float t);
}

std::ostream& operator<<(std::ostream& Stream, const Color& Other);