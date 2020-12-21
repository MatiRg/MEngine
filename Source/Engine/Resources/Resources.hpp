#pragma once
#include "../Engine/EngineModule.hpp"
#include "ResourceManager.hpp"
#include <unordered_map>
#include <memory>
#include <vector>

class CEngine;
class ISystem;

class CResources final: public IEngineModule
{
public:
    CResources(CEngine*);
    ~CResources();

    ENGINE_MODULE(CResources)

    ISystem* GetSystem() const;

    bool Init(const SEngineParams&) override;
	void Exit() override;

    // Create Resource if exist return it from pool else is loaded if fail return nullptr
    // Name, Vars
    template<class T>
    T* CreateResource(const std::string& Name, const ResourceCreateMap& Vars = {})
    {
        static_assert(std::is_base_of<IResource, T>::value, "Must be base of IResource");
        return dynamic_cast<T*>( CreateResource( T::GetTypeStatic(), Name, Vars ) );
    }
    // Create Resource if exist return it from pool else is loaded if fail return nullptr
    // Type, Name, Vars
    IResource* CreateResource(const std::string&, const std::string&, const ResourceCreateMap& = {});

    // Name, Vars
    template<class T>
    void PreloadResource(const std::string& Name, const ResourceCreateMap& Vars = {})
    {
        static_assert(std::is_base_of<IResource, T>::value, "Must be base of IResource");
        CreateResource( T::GetTypeStatic(), Name, Vars );
    }
    // Type, Name, Vars
    void PreloadResource(const std::string& Type, const std::string& Name, const ResourceCreateMap& Vars = {}) { CreateResource(Type, Name, Vars); }

    template<class T, class...Args>
    void RegisterManager(Args&&...args)
    {
        static_assert(std::is_base_of<IResourceManager, T>::value, "Must be base of IResourceManager");
        Managers.emplace_back(std::make_unique<T>(std::forward<Args>(args)...))->SetResources(this);
    }

    template<class T>
    T* GetManagerFor()
    {
        static_assert(std::is_base_of<IResource, T>::value, "Must be base of IResource");
        return dynamic_cast<T*>(GetManagerFor(T::GetTypeStatic()));
    }

    // Return nullptr if unable to find manager for resource type
    IResourceManager* GetManagerFor(const std::string&) const;

    template<class T>
    T* GetManager()
    {
        static_assert(std::is_base_of<IResourceManager, T>::value, "Must be base of IResourceManager");
        return dynamic_cast<T*>(GetManager(T::GetTypeStatic()));
    }

    // Return nullptr if unable to find manager
    IResourceManager* GetManager(const std::string&) const;

    // Path, SubDirs Included
    void AddPath(const std::string&, const bool);
    // Add Persisting Path, SubDirs Included
    void AddStaticPath(const std::string&, const bool);
    // Return Path to resource, true - if success, false if failed
    bool FindPath(const std::string&, std::string&) const;
    // Clear Search Pathes excluding ones marked 'Persist'
    void ClearPath();
private:
    class CSearchPath
    {
    public:
        CSearchPath(const std::string& aPath, const bool aPersist):
            Path( aPath ),
            Persist( aPersist )
        {
        }

        std::string Path;
        bool Persist = false;
    };
private:
    // Path, SubDirs Included, Persist
    void AddPathInternal(const std::string&, const bool, const bool);
private:
    CEngine* Engine = nullptr;
    std::vector<CSearchPath> SearchPath;
    std::vector<std::unique_ptr<IResourceManager>> Managers;
};

