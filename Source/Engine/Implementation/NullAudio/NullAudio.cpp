#include "NullAudio.hpp"
#include "NullSound.hpp"
#include "NullSoundData.hpp"
#include "../../Core/Log.hpp"

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
    LOG( ESeverity::Info ) << "Audio - Exit\n";
}

std::unique_ptr<ISoundData> CNullAudio::CreateSoundData(const std::string& Name)
{
    return std::make_unique<CNullSoundData>( Name );
}

std::unique_ptr<ISound> CNullAudio::CreateSound(ISoundData* aData)
{
    auto Sound = std::make_unique<CNullSound>(aData);
    return Sound;
}
