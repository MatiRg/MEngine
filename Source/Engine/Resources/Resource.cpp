#include "Resource.hpp"
#include "Resources.hpp"
#include "../System/System.hpp"

bool IResource::Find(CResources* Resources)
{
    if( !Resources->FindPath(Name, Path) )
    {
        LOG( ESeverity::Error ) << "Unable to Find Path to Resource: " << Name << "\n";
        return false;
    }
    return true;
}

bool IResource::Exist(CResources* Resources)
{
    if( Resources->GetSystem()->FileExist( Path ) )
    {
        return true;
    }
    return false;
}