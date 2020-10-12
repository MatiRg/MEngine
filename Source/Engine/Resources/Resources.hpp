#pragma once
#include "../Engine/EngineModule.hpp"
#include <unordered_map>
#include <memory>
#include <vector>
#include <tuple>

class CEngine;
class ITexture2D;
class IFont;
class ISoundData;
class ISound;
class IResource;
class CSpriteData;
class IResourceFactory;
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
    template<class T>
    T* CreateResource(const std::string& Name)
    {
        static_assert(std::is_base_of<IResource, T>::value, "Must be base of IResource");
        return dynamic_cast<T*>( CreateResource( T::GetTypeStatic(), Name ) );
    }
    // Create Resource if exist return it from pool else is loaded if fail return nullptr
    // Type, Name
    IResource* CreateResource(const std::string&, const std::string&);

    template<class T>
    void PreloadResource(const std::string& Name)
    {
        static_assert(std::is_base_of<IResource, T>::value, "Must be base of IResource");
        CreateResource( T::GetTypeStatic(), Name );
    }
    void PreloadResource(const std::string& Type, const std::string& Name) { CreateResource(Type, Name); }

    template<class T, class...Args>
    void RegisterFactory(Args&&...args)
    {
        static_assert(std::is_base_of<IResourceFactory, T>::value, "Must be base of IResourceFactory");
        Factories.emplace_back( std::make_unique<T>( std::forward<Args>(args)... ) );
    }

    // Return nullptr if unable to find factory
    IResourceFactory* GetFactory(const std::string&) const;

    // Path, Persist
    void AddPath(const std::string&, const bool);
    // Return Path to resource, true - if success, false if failed
    std::tuple<bool, std::string> FindPath(const std::string&) const;
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
    CEngine* Engine = nullptr;
    std::vector<CSearchPath> SearchPath;
    std::vector<std::unique_ptr<IResourceFactory>> Factories;
    std::unordered_map<std::string, std::unique_ptr<IResource>> Resources;
};

