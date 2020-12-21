#pragma once
#include "../Core/NonCopyable.hpp"
#include "../Core/Log.hpp"
#include "../System/MemoryManager.hpp"
#include "Resource.hpp"
#include <type_traits>
#include <memory>

class CResources;

// Define in it`s children for basic reflection
#define RESOURCE_MANAGER(X) std::string GetType() const override final { return #X; } \
                    static std::string GetTypeStatic() { return #X; }

class IResourceManager: public NonCopyableMovable
{
    friend class CResources;
public:
    IResourceManager();
    virtual ~IResourceManager();

    // Name, Params, Returns Non Owning Pointer
    IResource* CreateResource(const std::string&, const ResourceCreateMap&);

    virtual const StringVec& GetSupported() const = 0;
    virtual bool IsSupported(const std::string&) const = 0;

    virtual std::string GetType() const = 0;
protected:
    virtual std::unique_ptr<IResource> MakeResource(const std::string&, const ResourceCreateMap&) = 0;
    //
    virtual IResource* GetResource(const std::string&, const ResourceCreateMap&);
    virtual IResource* AddResource(std::unique_ptr<IResource>&&);
protected:
    CResources* Resources = nullptr;
    ResourceVec ResourcesArray;
private:
    void SetResources(CResources* aResources);
};

template<class...Args>
class TResourceManager: public IResourceManager
{
    template<class...Ts>
    using AreT = std::conjunction<std::is_base_of<IResource, Ts>...>;
    static_assert(AreT<Args...>::value, "Must be base of IResource");
public:
    TResourceManager()
    {
        LOG(ESeverity::Debug) << "Resource Manager Created - " << DebugString() << "\n";
        ADD_MEMORY_RECORD(this);
    }

    virtual ~TResourceManager()
    {
        LOG( ESeverity::Debug ) << "Resource Manager Destroyed - " << DebugString() << "\n";
        ERASE_MEMORY_RECORD(this);
    }

    const StringVec& GetSupported() const override { return SUPPORTED; }

    bool IsSupported(const std::string& x) const override
    {
        for(const auto& i: SUPPORTED)
        {
            if (i == x)
            {
                return true;
            }
        }
        return false;
    }
private:
    std::string DebugString() const
    {
        std::string Tmp;
        for (std::size_t i = 0u; i < SUPPORTED.size(); ++i)
        {
            Tmp += SUPPORTED[i];
            if (i + 1u < SUPPORTED.size())
            {
                Tmp += ", ";
            }
        }
        return Tmp;
    }
private:
    static StringVec SUPPORTED;
};

template<class...Args> StringVec TResourceManager<Args...>::SUPPORTED = { Args::GetTypeStatic()... };