#pragma once
#include "../Resources/ResourceManager.hpp"

class IAudio;

// For Resource Var Map
static constexpr char RESOURCES_VAR_IS_STREAM[] = "__IS_STREAM";

class ISoundData: public IResource
{
public:
    ISoundData(const std::string& Name):
        IResource( Name )
    {
    }

    virtual ~ISoundData() 
    {
    }

    RESOURCE(ISoundData)

    virtual bool IsStream() const = 0;

    // Clip Length in milliseconds
    virtual int GetDuration() const = 0;
};

//

class CSoundDataManager: public TResourceManager<ISoundData>
{
public:
    CSoundDataManager(IAudio*);

    RESOURCE_MANAGER(CMaterialManager)
protected:
    std::unique_ptr<IResource> MakeResource(const std::string&, const ResourceCreateMap&) override;
private:
    IAudio* Audio = nullptr;
};