#pragma once
#include "../Core/NonCopyable.hpp"
#include "../Core/Log.hpp"
#include "../Core/Types.hpp"
#include "../System/MemoryManager.hpp"
#include <any>

using ResourceCreateVar = std::any;
using ResourceCreateMap = StringMap<ResourceCreateVar>;

class CResources;

// Define in it`s children for basic reflection
#define RESOURCE(X) std::string GetType() const override final { return #X; } \
                    static std::string GetTypeStatic() { return #X; }

class IResource: public NonCopyableMovable
{
public:
    IResource(const std::string& aName):
        Path(""),
        Name(aName)
    {
        LOG( ESeverity::Debug ) << "Resource Created - " << (Name.empty() ? "<Annoymous>" : Name) << "\n";
        ADD_MEMORY_RECORD(this);
    }

    virtual ~IResource() 
    {
        LOG( ESeverity::Debug ) << "Resource Destroyed - " << (Name.empty() ? "<Annoymous>" : Name) << "\n";
        ERASE_MEMORY_RECORD(this);
    }

    virtual std::string GetType() const = 0;

    // Find Resource Path & Check it Exist
    virtual bool Find(CResources*);
    //
    virtual bool Exist(CResources*);

    // Load From Path
    virtual bool Load(CResources*, const ResourceCreateMap&) = 0;

    bool IsValid() const { return Valid; }

    const std::string& GetName() const { return Name; }
    const std::string& GetPath() const { return Path; }
protected:
    bool Valid = false;
    std::string Path; // Full Path
private:
    std::string Name; 
};

using ResourceVec = std::vector<std::unique_ptr<IResource>>;