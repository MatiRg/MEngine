#include "IntVector2.hpp"

IntVector2 IntVector2::ZERO = { 0, 0 };
IntVector2 IntVector2::ONE = { 1, 1 };

std::ostream& operator<<(std::ostream& Stream, const IntVector2& Other)
{
    Stream << Other.x << ", " << Other.y;
    return Stream;
}