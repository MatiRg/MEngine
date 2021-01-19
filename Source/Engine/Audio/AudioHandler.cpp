#include "AudioHandler.hpp"
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
	Music.reset();

	for(auto& i: Sounds)
	{
		i.second->Stop();
	}
	Sounds.clear();
}

void CAudioHandler::PlayMusic(const std::string& Name)
{
    StopMusic();
    Music.reset();
    
    Music = Audio->CreateSound( Resources->CreateResource<ISoundData>( Name ) );

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
        std::unique_ptr<ISound> Sound = Audio->CreateSound( Resources->CreateResource<ISoundData>( Name ) );
        if( Sound )
        {
            Sounds[Name] = std::move(Sound);
            Sounds[Name]->SetLooped(false);
            Sounds[Name]->Play();
        }
    }
}