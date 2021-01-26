#pragma once
#include "../Engine/EngineModule.hpp"
#include "ResourceManager.hpp"
#include <unordered_map>
#include <memory>
#include <vector>

class CEngine;
class ISystem;

/**
  \class CResources
  \brief Class for managing Resource like Textures, Models, Sound Data
*/
class CResources final: public IEngineModule
{
public:
    CResources(CEngine*);
    ~CResources();

    ENGINE_MODULE(CResources)

    ISystem* GetSystem() const;

    bool Init(const SEngineParams&) override;
	void Exit() override;

    /**
     * \brief Create or Get Resource with given name
     * \tparam T type of resource must inherints from IResource
     * \param Name Name of the resource
     * \param Vars Parameters used during loading
     * \return Return Pointer for given resource or nullptr if unable to load it
     */
    template<class T>
    T* CreateResource(const std::string& Name, const ResourceCreateMap& Vars = {})
    {
        static_assert(std::is_base_of<IResource, T>::value, "Must be base of IResource");
        return dynamic_cast<T*>( CreateResource( T::GetTypeStatic(), Name, Vars ) );
    }

    /**
     * \brief Create or Get Resource with given name
     * \param Type type of resource
     * \param Name Name of the resource
     * \param Vars Parameters used during loading
     * \return Return Pointer for given resource or nullptr if unable to load it
     */
    IResource* CreateResource(const std::string& Type, const std::string& Name, const ResourceCreateMap& Vars = {});

    /**
     * \brief Preload Resource with given name
     * \tparam T type of resource must inherints from IResource
     * \param Name Name of the resource
     * \param Vars Parameters used during loading
     */
    template<class T>
    void PreloadResource(const std::string& Name, const ResourceCreateMap& Vars = {})
    {
        static_assert(std::is_base_of<IResource, T>::value, "Must be base of IResource");
        CreateResource( T::GetTypeStatic(), Name, Vars );
    }
    /**
     * \brief Preload Resource with given name
     * \param Type type of resource
     * \param Name Name of the resource
     * \param Vars Parameters used during loading
     */
    void PreloadResource(const std::string& Type, const std::string& Name, const ResourceCreateMap& Vars = {}) { CreateResource(Type, Name, Vars); }

    /**
     * \brief Register resource manager
     * \tparam T type of resource manager must inherints from IResourceManager
     * \tparam Args Parameter pack used to construct manager
     * \param args Parameter pack used to construct manager
     */
    template<class T, class...Args>
    void RegisterManager(Args&&...args)
    {
        static_assert(std::is_base_of<IResourceManager, T>::value, "Must be base of IResourceManager");
        Managers.emplace_back(std::make_unique<T>(std::forward<Args>(args)...))->SetResources(this);
    }

    /**
     * \brief Gets resource manager for given resource type
     * \tparam T type of resource must inherints from IResource
     * \return Return Pointer for resource manager or nullptr if unable to find valid manager
     */
    template<class T>
    T* GetManagerFor()
    {
        static_assert(std::is_base_of<IResource, T>::value, "Must be base of IResource");
        return dynamic_cast<T*>(GetManagerFor(T::GetTypeStatic()));
    }

    /**
     * \brief Gets resource manager for given resource type
     * \param Type type of resource
     * \return Return Pointer for resource manager or nullptr if unable to find valid manager
     */
    IResourceManager* GetManagerFor(const std::string& Type) const;

    /**
     * \brief Gets resource manager for given resource type
     * \tparam T type of Resource Manager must inherints from IResourceManager
     * \return Return Pointer for resource manager or nullptr if unable to find valid manager
     */
    template<class T>
    T* GetManager()
    {
        static_assert(std::is_base_of<IResourceManager, T>::value, "Must be base of IResourceManager");
        return dynamic_cast<T*>(GetManager(T::GetTypeStatic()));
    }
    /**
     * \brief Gets resource manager for given resource type
     * \param Type type of Resource Manager
     * \return Return Pointer for resource manager or nullptr if unable to find valid manager
     */
    IResourceManager* GetManager(const std::string& Type) const;

    /**
     * \brief Add Search Path - Path where resource will be searched for
     * \param Path Path where to search
     * \param SubDirs Include Subdirectories
     */
    void AddPath(const std::string& Path, const bool SubDirs);
    /**
     * \brief Add Persisten Search Path - Path where resource will be searched for and it will not be removed during cleanup of pathes
     * \param Path Path where to search
     * \param SubDirs Include Subdirectories
     */
    void AddStaticPath(const std::string& Path, const bool SubDirs);
    /**
     * \brief Find Path for given resource name
     * \param Name Resource Name
     * \param Out Outputs Path
     * \return Returns true if found resource else false
     */
    bool FindPath(const std::string& Name, std::string& Out) const;
    //! Clear Search Pathes excluding ones marked 'Persist'
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

