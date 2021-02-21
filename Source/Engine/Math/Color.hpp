#pragma once
#include "Functions.hpp"
#include "Vector4.hpp"
#include <ostream>

class Color
{
public:
    constexpr Color(const float Val = 0.0f):
        r( Val ), g( Val ), b( Val ), a( Val )
    {
    }

    constexpr Color(const float Val, const float Alpha):
        r( Val ), g( Val ), b( Val ), a( Alpha )
    {
    }

    constexpr Color(const float Val1, const float Val2, const float Val3, const float Val4 = 1.0f):
        r( Val1 ), g( Val2 ), b( Val3 ), a( Val4 )
    {
    }

    constexpr Color(const Color& Val):
        r( Val.r ), g( Val.g ), b( Val.b ), a( Val.a )
    {
    }

    constexpr Color& operator=(const Color& Other)
    {
        r = Other.r;
        g = Other.g;
        b = Other.b;
        a = Other.a;
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

    uint8_t GetR_UInt() const { return static_cast<uint8_t>(r * 255.0f); }
    uint8_t GetG_UInt() const { return static_cast<uint8_t>(g * 255.0f); }
    uint8_t GetB_UInt() const { return static_cast<uint8_t>(b * 255.0f); }
    uint8_t GetA_UInt() const { return static_cast<uint8_t>(a * 255.0f); }
public:
    float r, g, b, a;
};

static constexpr Color COLOR_WHITE = { 1.0f, 1.0f };
static constexpr Color COLOR_BLACK = { 0.0f, 1.0f };
static constexpr Color COLOR_RED = { 1.0f, 0.0f, 0.0f };
static constexpr Color COLOR_GREEN = { 0.0f, 1.0f, 0.0f };
static constexpr Color COLOR_BLUE = { 0.0f, 0.0f, 1.0f };
static constexpr Color COLOR_GREY = { 0.5f, 0.5f, 0.5f };
static constexpr Color COLOR_YELLOW = { 1.0f, 1.0f, 0.0f };
static constexpr Color COLOR_CYAN = { 0.0f, 1.0f, 1.0f };
static constexpr Color COLOR_PINK = { 1.0f, 0.0f, 1.0f };
static constexpr Color COLOR_MAROON = { 0.5f, 0.0f, 0.0f };

namespace Math
{
    Color Lerp(const Color& a, const Color& b, const float t);
    Vector4 ToVector4(const Color& a);
}

std::ostream& operator<<(std::ostream& Stream, const Color& Other);