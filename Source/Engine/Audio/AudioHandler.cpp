#include "AudioHandler.hpp"
#include "Audio.hpp"
#include "Sound.hpp"
#include "SoundData.hpp"
#include "../Resources/Resources.hpp"
#include "../Core/Log.hpp"

CAudioHandler::CAudioHandler(IAudio* aAudio, CResources* aResources): 
    IEngineModule("Audio Handler"),
    Audio(aAudio),
    Resources(aResources)
{
}

CAudioHandler::~CAudioHandler()
{
}

void CAudioHandler::OnExit()
{
	StopMusic();
    Audio->DestroySound(Music);
	Music = nullptr;

	for(auto& i: Sounds)
	{
		i.second->Stop();
        Audio->DestroySound(i.second);
	}
	Sounds.clear();
}

void CAudioHandler::PlayMusic(const std::string& Name)
{
    StopMusic();
    if( !Music )
    {
        Music = Audio->CreateSound( Resources->CreateResource<ISoundData>( Name ) );
    }
    else
    {
        Music->SetSoundData( Resources->CreateResource<ISoundData>( Name ) );
    }

    if( Music ) 
    {
        Music->SetLooped(true);
        Music->Play();
    }
}

void CAudioHandler::StopMusic()
{
    if( Music ) 
    {
        Music->Stop();
    }
}

void CAudioHandler::PlaySound(const std::string& Name)
{
    if( Sounds.count(Name) && Sounds[Name] )
    {
        Sounds[Name]->SetLooped(false);
        Sounds[Name]->Play();
    }
    else
    {
        ISound* Sound = Audio->CreateSound( Resources->CreateResource<ISoundData>( Name ) );
        if( Sound )
        {
            Sounds[Name] = Sound;
            Sounds[Name]->SetLooped(false);
            Sounds[Name]->Play();
        }
    }
}