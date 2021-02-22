#pragma once
#include <string>
#include <memory>
#include <vector>
#include <utility>
#include "../Math/Vector3.hpp"

namespace Utils
{
    std::string Trim(const std::string&);
    bool StartWith(const std::string&, const std::string&);

    // Remove File
    std::string GetPath(const std::string&);
    std::string GetNameFromPath(const std::string&);
    // Remove Extension
    std::string GetNameFromExt(const std::string&);
    std::string GetFileExt(const std::string&);
    std::string NormalizePath(const std::string&);
    // Add Dir Separator e.g. Test/Two will return Test/Two/
    std::string AddTrailingDirSeparator(const std::string&);

    std::string Format(const char*, ...);
    std::string ToLower(const std::string&);
    std::string ToUpper(const std::string&);
    std::vector<std::string> Split(const std::string&, const char);
    
    bool IsInteger(const std::string&);

    std::string ToUTF8(const std::u16string&);
    std::u16string ToUTF16(const std::string&);

    int GetHexDigit(char, const int);

    // See boost::​hash_combine
    // Pass hash values starting from 0
    void HashCombine(std::size_t&, const std::size_t);

    std::string ToString(const Vector2& v);
    std::string ToString(const IntVector2& v);
    std::string ToString(const Vector3& v);
    std::string ToString(const bool v);

    template<class T>
    std::string ToString(const T v)
    {
        return std::to_string(v);
    }
}

struct PairHash
{
    template<class T1, class T2>
    std::size_t operator()(const std::pair<T1, T2>& Pair) const
    {
        std::size_t a1 = std::hash<T1>()(Pair.first);
        std::size_t a2 = std::hash<T2>()(Pair.second);
        //
        std::size_t Out = 0u;
        Utils::HashCombine(Out, a1);
        Utils::HashCombine(Out, a2);
        return Out;
        //return a1 ^ (a2 << 1); // Replace with something better
    }
};