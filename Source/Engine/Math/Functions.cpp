#include "Functions.hpp"
#include <random>

namespace Math
{
    namespace
    {
        static std::random_device RandomDevice;
        static std::mt19937 Generator(RandomDevice());
    }

    int Random(const int a, const int b)
    {
        std::uniform_int_distribution<int> Distribution(Min(a, b), Max(a, b));
        return Distribution(Generator);
    }

    float Random(const float a, const float b)
    {
        std::uniform_real_distribution<float> Distribution(Min(a, b), Max(a, b));
        return Distribution(Generator);
    }
}