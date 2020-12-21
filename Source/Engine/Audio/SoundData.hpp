#pragma once
#include "../Resources/ResourceManager.hpp"

class IAudio;

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

    // Clip Length in Samples 
    virtual int GetSamples() const = 0;
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