#include "Resource.hpp"
#include "Resources.hpp"
#include "../System/System.hpp"

bool IResource::Find()
{
    bool FoundPath;
    std::tie(FoundPath, Path) = Resources->FindPath(Name);
    if( !FoundPath )
    {
        LOG( ESeverity::Error ) << "Unable to Find Path to Resource: " << Name << "\n";
        return false;
    }
    return true;
}

bool IResource::Exist()
{
    if( Resources->GetSystem()->FileExist( Path ) )
    {
        return true;
    }
    return false;
}