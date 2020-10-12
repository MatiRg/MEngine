#include "NullAudio.hpp"
#include "NullSound.hpp"
#include "NullSoundData.hpp"
#include "../../Core/Log.hpp"
#include <algorithm>

CNullAudio::CNullAudio()
{
}

CNullAudio::~CNullAudio()
{
}

bool CNullAudio::Init(const SEngineParams&)
{
    LOG( ESeverity::Info ) << "Audio Device: Null - Init\n";
    LOG( ESeverity::Info ) << "Audio - Init\n";
    return true;
}

void CNullAudio::Exit()
{
    Sounds.clear();
    LOG( ESeverity::Info ) << "Audio - Exit\n";
}

std::unique_ptr<ISoundData> CNullAudio::CreateSoundData(const std::string& Name, CResources* Resources)
{
    return std::make_unique<CNullSoundData>( Name, Resources );
}

ISound* CNullAudio::CreateSound(ISoundData*)
{
    auto Sound = std::make_unique<CNullSound>();
    auto Raw = Sound.get();
    Sounds.push_back( std::move(Sound) );
    return Raw;
}

void CNullAudio::DestroySound(ISound* aSound)
{
    auto Iterator = std::find_if(Sounds.begin(), Sounds.end(), [&](const std::unique_ptr<ISound>& Sound)
    {
        return Sound.get() == aSound;
    });
    if( Iterator != Sounds.end() )
    {
        (*Iterator)->Stop();
        Sounds.erase(Iterator);
    }
}

void CNullAudio::SetVolume(const float)
{
}

void CNullAudio::StopAll()
{
}
