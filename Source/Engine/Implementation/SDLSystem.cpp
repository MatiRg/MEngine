#include "SDLSystem.hpp"
#include "../Core/Log.hpp"
#include "../Core/Utils.hpp"
#include <SDL2/SDL.h>
#include <tinyfiledialogs.h>
#include <filesystem>
#include <fstream>

CSDLSystem::CSDLSystem()
{
}

CSDLSystem::~CSDLSystem()
{
}

bool CSDLSystem::FileExist(const std::string& Path) const
{
    return std::filesystem::exists(Path) && std::filesystem::is_regular_file(Path);
}

bool CSDLSystem::DirectoryExist(const std::string& Path) const
{
    return std::filesystem::exists(Path) && std::filesystem::is_directory(Path);
}

bool CSDLSystem::CreateDirectory(const std::string& Path)
{
    std::error_code Ec;
    bool r = std::filesystem::create_directory( Path, Ec );
    return r && Ec.value() == 0;
}

bool CSDLSystem::CopyFile(const std::string& From, const std::string& To)
{
    if( FileExist(To) )
    {
        Remove(To);
    }
    std::ifstream Src(From, std::ios::binary);
    std::ofstream Dst(To, std::ios::binary);
    if( !Src || !Dst )
    {
        LOG(ESeverity::Error) << "Unable to Copy File from: " << From << " To: " << To << "\n";
        return false;
    }
    Dst << Src.rdbuf();
    return true;
    // Not working?
    //std::error_code Ec;
    //bool r = std::filesystem::copy_file( From, To, std::filesystem::copy_options::overwrite_existing, Ec );
    //return r && Ec.value() == 0;
}

bool CSDLSystem::Remove(const std::string& Path)
{
    std::error_code Ec;
    std::filesystem::remove_all( Path, Ec );
    return Ec.value() == 0;
}

std::vector<CFileRecord> CSDLSystem::ListDirectory(const std::string& Path, const bool Sub)
{
    std::vector<CFileRecord> Recs;
    if( !Sub )
    {
        for(const std::filesystem::directory_entry& p: std::filesystem::directory_iterator(Path))
        {
            if( !(p.is_directory() || p.is_regular_file()) )
            {
                continue;
            }
            Recs.emplace_back( p.path().filename().string(), p.path().string(), p.is_directory() );
        }
    }
    else
    {
        for(const std::filesystem::directory_entry& p: std::filesystem::recursive_directory_iterator(Path))
        {
            if( !(p.is_directory() || p.is_regular_file()) )
            {
                continue;
            }
            Recs.emplace_back( p.path().filename().string(), p.path().string(), p.is_directory() );
        }
    }
    return Recs;
}

uint32_t CSDLSystem::GetTime() const
{
    return SDL_GetTicks();
}

void CSDLSystem::MessageBox(const std::string& Title, const std::string& Text, const EMessageBoxIcon Icon)
{
    std::string Icn;
    switch(Icon)
    {
    case EMessageBoxIcon::Warning:
        Icn = "warning";
        break;
    case EMessageBoxIcon::Error:
        Icn = "error";
        break;
    case EMessageBoxIcon::Info:
    default:
        Icn = "info";
        break;
    }
    tinyfd_messageBox( Title.c_str(), Text.c_str(), "ok", Icn.c_str(), 1 );
}

std::string CSDLSystem::OpenFileDialog(const std::string& Title, const std::string& Path)
{
    const char* r = tinyfd_openFileDialog( Title.c_str(), Path.c_str(), 0, nullptr, nullptr, false );
    return r ? r : "";
}

std::string CSDLSystem::SaveFileDialog(const std::string& Title, const std::string& Path)
{
    const char* r = tinyfd_saveFileDialog( Title.c_str(), Path.c_str(), 0, nullptr, nullptr );
    return r ? r : "";
}

std::string CSDLSystem::SelectFolderDialog(const std::string& Title, const std::string& Path)
{
    const char* r = tinyfd_selectFolderDialog( Title.c_str(), Path.c_str() );
    return r ? r : "";
}

std::string CSDLSystem::SelectFileDialog(const std::string& Title, const std::string& Path)
{
    const char* r = tinyfd_openFileDialog( Title.c_str(), Path.c_str(), 0, nullptr, nullptr, false );
    return r ? r : "";
}

std::vector<std::string> CSDLSystem::SelectFilesDialog(const std::string& Title, const std::string& Path)
{
    const char* r = tinyfd_openFileDialog( Title.c_str(), Path.c_str(), 0, nullptr, nullptr, true );
    if( !r )
    {
        return std::vector<std::string>();
    }
    return Utils::Split( r, '|' );
}
