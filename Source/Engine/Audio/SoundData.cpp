#include "SoundData.hpp"
#include "Audio.hpp"

CSoundDataFactory::CSoundDataFactory(IAudio* aAudio):
    Audio(aAudio)
{
}

std::unique_ptr<IResource> CSoundDataFactory::CreateResource(const std::string& Name, CResources* Resources)
{
    return Audio->CreateSoundData( Name, Resources );
}