#include "SoundData.hpp"
#include "Audio.hpp"

CSoundDataManager::CSoundDataManager(IAudio* aAudio):
    Audio(aAudio)
{
}

std::unique_ptr<IResource> CSoundDataManager::MakeResource(const std::string& Name, const ResourceCreateMap&)
{
    return Audio->CreateSoundData( Name );
}