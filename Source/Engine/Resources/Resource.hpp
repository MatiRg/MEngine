#pragma once
#include "../Core/NonCopyable.hpp"
#include "../Core/Log.hpp"
#include <string>

class CResources;

// Define in it`s children for basic reflection
#define RESOURCE(X) std::string GetType() const override final { return #X; } \
                    static std::string GetTypeStatic() { return #X; }

class IResource: public NonCopyableMovable
{
public:
    IResource(const std::string& aName, CResources* aResources):
        Resources(aResources),
        Path(""),
        Name(aName)
    {
        LOG( ESeverity::Debug ) << "Resource Created - " << (Name.empty() ? "<Annoymous>" : Name) << "\n";
    }

    virtual ~IResource() 
    {
        LOG( ESeverity::Debug ) << "Resource Destroyed - " << (Name.empty() ? "<Annoymous>" : Name) << "\n";
    }

    virtual std::string GetType() const = 0;

    // Find Resource Path & Check it Exist
    virtual bool Find();
    //
    virtual bool Exist();

    // Load From Path
    virtual bool Load() = 0;
    //
    virtual bool IsValid() const = 0;

    const std::string& GetName() const { return Name; }
    const std::string& GetPath() const { return Path; }
protected:
    CResources* Resources = nullptr;
    std::string Path; // Path + Name
private:
    std::string Name; 
};
