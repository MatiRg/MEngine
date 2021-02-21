#include "Rect2.hpp"

std::ostream& operator<<(std::ostream& Stream, const Rect2& r)
{
    Stream << r.GetX() << ", " << r.GetY() << " : " << r.GetMaxX() << ", " << r.GetMaxY();
    return Stream;
}
