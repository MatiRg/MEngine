#pragma once
#include "../Resources/Resource.hpp"
#include "../Resources/ResourceFactory.hpp"

class IAudio;

class ISoundData: public IResource
{
public:
    ISoundData(const std::string& Name, CResources* Resources):
        IResource( Name, Resources )
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

class CSoundDataFactory: public TResourceFactory<ISoundData>
{
public:
    CSoundDataFactory(IAudio*);

    std::unique_ptr<IResource> CreateResource(const std::string&, CResources*) override;
private:
    IAudio* Audio = nullptr;
};