#pragma once
#include <string>
#include "../Core/Utils.hpp"

class CFileRecord
{
public:
    CFileRecord(const std::string& aName, const std::string& aPath, const bool aDirectory):
        Name(aName),
        Path(aPath),
        Directory(aDirectory)
    {
        if( !Directory )
        {
            Extension = Utils::ToLower( Utils::GetFileExt(Name) );
        }
    }

    std::string Name = "";
    std::string Path = "";
    bool Directory = false;
    std::string Extension = "";
};
