#include "OALAudio.hpp"
#include "OALSoundData.hpp"
#include "OALSound.hpp"
#include "../Core/Log.hpp"
#include "../Math/Functions.hpp"
#include "../Core/Utils.hpp"
#include "OALUtils.hpp"
#include <algorithm>

COALAudio::COALAudio()
{
}

COALAudio::~COALAudio()
{
    COALAudio::StopAll();
	alcMakeContextCurrent( nullptr );
	if( Context )
    {
        alcDestroyContext( Context );
        Context = nullptr;
	}
	if( Device )
	{
	    alcCloseDevice( Device );
        Device = nullptr;
	}
}

bool COALAudio::Init(const SEngineParams&)
{
    const ALCchar* DefaultDevice = alcGetString( nullptr, ALC_DEFAULT_DEVICE_SPECIFIER );
    if( !DefaultDevice )
    {
        LOG(ESeverity::Fatal) << "Invalid OpenAL device name\n";
        return false;
    }
    Device = alcOpenDevice( DefaultDevice );
    if( !Device )
    {
        LOG(ESeverity::Fatal) << "Unable to open OpenAL device\n";
        return false;
    }
    Context = alcCreateContext( Device, nullptr );
    if( !Context )
    {
        LOG(ESeverity::Fatal) << "Unable to create OpenAL context\n";
        return false;
    }
    alcMakeContextCurrent( Context );

    if( OAL::CheckOALError() ) 
    {
        return false;
    }

    Vector3 Forward = Vector3::FORWARD;
    Vector3 Up = Vector3::UP;
    ALfloat ListenerOrientation[] = { Forward.x, Forward.y, -Forward.z, Up.x, Up.y, Up.z }; // 1 Forward, 2 Up as Vector3::UP()
    alListener3f( AL_POSITION, 0.0f, 0.0f, 0.0f );
    alListener3f( AL_VELOCITY, 0.0f, 0.0f, 0.0f );
	alListenerfv( AL_ORIENTATION, ListenerOrientation );
    alListenerf( AL_GAIN, Volume );

    if( OAL::CheckOALError() ) 
    {
        return false;
    }

    LOG( ESeverity::Info ) << "Set Audio Device: " << DefaultDevice << "\n";
    LOG( ESeverity::Info ) << "Audio - Init\n";
    return true;
}

void COALAudio::Exit()
{
    StopAll();
    Sounds.clear();
    LOG( ESeverity::Info ) << "Audio - Exit\n";
}

std::unique_ptr<ISoundData> COALAudio::CreateSoundData(const std::string& Name)
{
    return std::make_unique<COALSoundData>( Name );
}

ISound* COALAudio::CreateSound(ISoundData* Data)
{
    auto Sound = std::make_unique<COALSound>( this, dynamic_cast<COALSoundData*>(Data) );
    if( !Sound->CreateSource() )
    {
        return nullptr;
    }
    auto Raw = Sound.get();
    Sounds.push_back( std::move(Sound) );
    return Raw;
}

void COALAudio::DestroySound(ISound* aSound)
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

void COALAudio::SetVolume(const float aVolume)
{
    Volume = Math::Clamp( aVolume, 0.0f, 1.0f );
    alListenerf( AL_GAIN, Volume );
}

void COALAudio::SetMuted(const bool Value) 
{ 
    Muted = Value; 
    alListenerf( AL_GAIN, Muted ? 0.0f : Volume );
}

void COALAudio::SetListenerPosition(const Vector3& Position)
{
    alListener3f( AL_POSITION, Position.x, Position.y, Position.z );
}

void COALAudio::SetListenerVelocity(const Vector3& Velocity)
{
    alListener3f( AL_VELOCITY, Velocity.x, Velocity.y, Velocity.z );
}

void COALAudio::StopAll()
{
    for(auto& i: Sounds)
    {
        if( i ) 
        {
            i->Stop();
        }
    }
}
