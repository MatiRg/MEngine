#pragma once
#include <string>
#include <memory>
#include <vector>

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
}