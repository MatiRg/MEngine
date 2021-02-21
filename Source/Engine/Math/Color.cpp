#include "Color.hpp"

namespace Math
{
    Color Lerp(const Color& a, const Color& b, const float t)
    {
        Color R(
            Lerp(a.GetR(), b.GetR(), t),
            Lerp(a.GetG(), b.GetG(), t),
            Lerp(a.GetB(), b.GetB(), t),
            Lerp(a.GetA(), b.GetA(), t)
        );
        return R;
    }

    Vector4 ToVector4(const Color& a)
    {
        return Vector4(a.r, a.g, a.b, a.a);
    }
}

std::ostream& operator<<(std::ostream& Stream, const Color& Other)
{
    Stream << Other.GetR() << ", " << Other.GetG() << ", " << Other.GetB() << ", " << Other.GetA();
    return Stream;
}
