#include "Color.hpp"

Color Color::WHITE = { 1.0f, 1.0f };
Color Color::BLACK = { 0.0f, 1.0f };
Color Color::RED = { 1.0f, 0.0f, 0.0f };
Color Color::GREEN = { 0.0f, 1.0f, 0.0f };
Color Color::BLUE = { 0.0f, 0.0f, 1.0f };
Color Color::GREY = { 0.5f, 0.5f, 0.5f };
Color Color::YELLOW = { 1.0f, 1.0f, 0.0f };
Color Color::CYAN = { 0.0f, 1.0f, 1.0f };
Color Color::PINK = { 1.0f, 0.0f, 1.0f };
Color Color::MAROON = { 0.5f, 0.0f, 0.0f };

namespace Math
{
    Color Lerp(const Color& a, const Color& b, const float t)
    {
        return {
            Lerp(a.GetR(), b.GetR(), t),
            Lerp(a.GetG(), b.GetG(), t),
            Lerp(a.GetB(), b.GetB(), t),
            Lerp(a.GetA(), b.GetA(), t)
        };
    }
}

std::ostream& operator<<(std::ostream& Stream, const Color& Other)
{
    Stream << Other.GetR() << ", " << Other.GetG() << ", " << Other.GetB() << ", " << Other.GetA();
    return Stream;
}
