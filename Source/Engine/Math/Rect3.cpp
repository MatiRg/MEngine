#include "Rect3.hpp"

std::ostream& operator<<(std::ostream& Stream, const Rect3& r)
{
    Stream << r.GetX() << ", " << r.GetY() << ", " << r.GetZ() << " : " << r.GetMaxX() << ", " << r.GetMaxY() << ", " << r.GetMaxZ();
    return Stream;
}
