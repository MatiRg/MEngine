#include "MemoryManager.hpp"
#include "../Core/Log.hpp"
#include "../Core/Utils.hpp"
#include <algorithm>

CMemoryManager::CMemoryManager()
{
    // Empty
}

CMemoryManager::~CMemoryManager()
{
    // Empty
}

CMemoryManager& CMemoryManager::GetInstance()
{
    static CMemoryManager MemoryManager;
    return MemoryManager;
}

void CMemoryManager::AddRecord(void* Ptr, const std::string& File, const int Line)
{
    Records.push_back( {Ptr, Utils::GetNameFromPath(File), Line} );
}

void CMemoryManager::EraseRecord(void* Ptr)
{
    auto Iterator = std::find_if(Records.begin(), Records.end(), [&](const SRecord& Record)
    {
        return Record.Ptr == Ptr;
    });
    if( Iterator != Records.end() )
    {
        Records.erase(Iterator);
    }
}

void CMemoryManager::Report()
{
    std::string LineDivider(64u, '*');
    if( Records.size() != 0u )
    {
        LOG(ESeverity::Fatal) << LineDivider << "\n";
        LOG(ESeverity::Fatal) << "* Leaks: " << Records.size() << "\n";
        for(const auto& i: Records)
        {
           LOG(ESeverity::Fatal) << "* Leak: " << i.Ptr << " " << i.File << " " << i.Line << "\n"; 
        }
        LOG(ESeverity::Fatal) << LineDivider << "\n";
    }
    else
    {
        LOG(ESeverity::Info) << LineDivider << "\n";
        LOG(ESeverity::Info) << "* No Leaks\n";
        LOG(ESeverity::Info) << LineDivider << "\n";
    }
}