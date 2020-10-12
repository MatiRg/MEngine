#pragma once
#include "../System/System.hpp"

class CSDLSystem: public ISystem
{
public:
    CSDLSystem();
    ~CSDLSystem();

    bool FileExist(const std::string&) const override;
    bool DirectoryExist(const std::string&) const override;
    bool CreateDirectory(const std::string&) override;
    bool CopyFile(const std::string&, const std::string&) override;
    bool Remove(const std::string&) override;
    std::vector<CFileRecord> ListDirectory(const std::string&, const bool) override;

    uint32_t GetTime() const override;

    void MessageBox(const std::string&, const std::string&, const EMessageBoxIcon) override;
    std::string OpenFileDialog(const std::string&, const std::string&) override;
    std::string SaveFileDialog(const std::string&, const std::string&) override;
    std::string SelectFolderDialog(const std::string&, const std::string&) override;
    std::string SelectFileDialog(const std::string&, const std::string&) override;
    std::vector<std::string> SelectFilesDialog(const std::string&, const std::string&) override;
};
