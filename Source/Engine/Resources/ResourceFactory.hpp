#pragma once
#include "Resource.hpp"
#include "../Core/Log.hpp"
#include "../Core/NonCopyable.hpp"
#include <string>
#include <memory>
#include <type_traits>

class CResources;

class IResourceFactory: public NonCopyableMovable
{
public:
    IResourceFactory() = default;
    virtual ~IResourceFactory() = default;

    virtual std::string GetName() const = 0; // Factory Name

    // Name, Path
    virtual std::unique_ptr<IResource> CreateResource(const std::string&, CResources*) = 0;
};

template<class T>
class TResourceFactory: public IResourceFactory
{
    static_assert(std::is_base_of<IResource, T>::value, "Must be base of IResource");
public:
    TResourceFactory()
    {
        LOG( ESeverity::Debug ) << "Created - " << T::GetTypeStatic() << " Resource Factory\n";
    }

    virtual ~TResourceFactory()
    {
        LOG( ESeverity::Debug ) << "Destroyed - " << T::GetTypeStatic() << " Resource Factory\n";
    }

    std::string GetName() const override { return T::GetTypeStatic(); }
};
