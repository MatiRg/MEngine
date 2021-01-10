#include "ResourceManager.hpp"
    
IResourceManager::IResourceManager()
{
}

IResourceManager::~IResourceManager()
{
    ResourcesArray.clear();
}

IResource* IResourceManager::GetResource(const std::string& Name, const ResourceCreateMap&)
{
    for (const auto& i : ResourcesArray)
    {
        if( i->GetName() == Name )
        {
            return i.get();
        }
    }
    return nullptr;
}

IResource* IResourceManager::AddResource(std::unique_ptr<IResource>&& Ptr, const ResourceCreateMap&)
{
    IResource* Tmp = Ptr.get();
    ResourcesArray.push_back( std::move(Ptr) );
    return Tmp;
}

IResource* IResourceManager::CreateResource(const std::string& Name, const ResourceCreateMap& Vars)
{
    IResource* Tmp = GetResource(Name, Vars);
    if( Tmp )
    {
        return Tmp;
    }

    std::unique_ptr<IResource> Ptr = MakeResource(Name, Vars);
    if( !Ptr )
    {
        LOG(ESeverity::Error) << "Unable to Create Resource: " << Name << "\n";
        return nullptr;
    }

    if( !Ptr->Find(Resources) )
    {
        return nullptr;
    }

    if( !Ptr->Load(Resources, Vars) )
    {
        LOG(ESeverity::Error) << "Unable to Load Resource: " << Name << "\n";
        return nullptr;
    }

    return AddResource( std::move(Ptr), Vars );
}

void IResourceManager::SetResources(CResources* aResources)
{ 
    Resources = aResources; 
}