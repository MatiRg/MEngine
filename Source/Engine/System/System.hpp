#pragma once
#include "../Engine/EngineModule.hpp"
#include "FileRecord.hpp"
#include <cstdint>
#include <vector>

enum class EMessageBoxIcon
{
    Info,
    Warning,
    Error
};

class ISystem: public IEngineModule
{
public:
    ISystem(): IEngineModule( "System" ) {}
    virtual ~ISystem() {}

    ENGINE_MODULE(ISystem)

    // Check if file exist
    virtual bool FileExist(const std::string&) const = 0;
    // Check if directory exist
    virtual bool DirectoryExist(const std::string&) const = 0;
    // Create Directory
    virtual bool CreateDirectory(const std::string&) = 0;
    // Copy File From, To
    virtual bool CopyFile(const std::string&, const std::string&) = 0;
    // Remove
    virtual bool Remove(const std::string&) = 0;
    // Path, Include Subdirectories
    virtual std::vector<CFileRecord> ListDirectory(const std::string&, const bool) = 0;

    // Returns time in ms
    virtual uint32_t GetTime() const = 0;

    // Title, Text, Icon
    virtual void MessageBox(const std::string&, const std::string&, const EMessageBoxIcon) = 0;
    // Title, Start Path
    virtual std::string OpenFileDialog(const std::string&, const std::string&) = 0;
    // Title, Start Path
    virtual std::string SaveFileDialog(const std::string&, const std::string&) = 0;
    // Title, Start Path
    virtual std::string SelectFolderDialog(const std::string&, const std::string&) = 0;
    // Title, Start Path
    virtual std::string SelectFileDialog(const std::string&, const std::string&) = 0;
    // Title, Start Path
    virtual std::vector<std::string> SelectFilesDialog(const std::string&, const std::string&) = 0;
};
