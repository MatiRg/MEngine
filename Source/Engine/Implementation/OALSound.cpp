#include "OALSound.hpp"
#include "OALAudio.hpp"
#include "../Math/Functions.hpp"
#include "../Core/Log.hpp"
#include "OALUtils.hpp"

COALSound::COALSound(COALAudio* aAudio, COALSoundData* aData):
    Audio( aAudio ),
    Data( aData )
{
}

COALSound::~COALSound()
{
    alDeleteSources( 1, &Source );
}

bool COALSound::CreateSource()
{
    alGenSources( 1, &Source );
    if( OAL::CheckOALError() )
    {
        return false;
    }

    alSourcef( Source, AL_PITCH, 1.0f );
    alSourcef( Source, AL_GAIN, Volume );
    alSource3f( Source, AL_POSITION, 0.0f, 0.0f, 0.0f );
    alSource3f( Source, AL_VELOCITY, 0.0f, 0.0f, 0.0f );
    alSourcei( Source, AL_LOOPING, AL_FALSE );
    if( OAL::CheckOALError() )
    {
        return false;
    }

    if( Data )
    {
        alSourcei( Source, AL_BUFFER, Data->GetBufferID() );
    }

    if( OAL::CheckOALError() )
    {
        return false;
    }

    Valid = true;
    return true;
}

bool COALSound::Play()
{
    if( !IsValid() )
    {
        LOG( ESeverity::Error ) << "Using Invalid Sound\n";
        return false;
    }

    ALint SourceState;
    alGetSourcei( Source, AL_SOURCE_STATE, &SourceState );
    if( SourceState != AL_PLAYING )
    {
        alSourcePlay( Source );
    }

    OAL::CheckOALError();
    return true;
}

bool COALSound::Pause(const bool State)
{
    if( !IsValid() )
    {
        LOG( ESeverity::Error ) << "Using Invalid Sound\n";
        return false;
    }

    if( State )
    {
        ALint SourceState;
        alGetSourcei( Source, AL_SOURCE_STATE, &SourceState );
        if( SourceState == AL_PLAYING )
        {
            alSourcePause( Source );
        }
    }
    else
    {
        ALint SourceState;
        alGetSourcei( Source, AL_SOURCE_STATE, &SourceState );
        if( SourceState == AL_PAUSED )
        {
            alSourcePlay( Source );
        }
    }

    OAL::CheckOALError();
    return true;
}

bool COALSound::Stop()
{
    if( !IsValid() )
    {
        LOG( ESeverity::Error ) << "Using Invalid Sound\n";
        return false;
    }

    ALint SourceState;
    alGetSourcei( Source, AL_SOURCE_STATE, &SourceState );
    if( SourceState != AL_INITIAL )
    {
        alSourceStop( Source );
    }

    OAL::CheckOALError();
    return true;
}

ESoundState COALSound::GetState()
{
    if( !IsValid() )
    {
        LOG( ESeverity::Error ) << "Using Invalid Sound\n";
        return ESoundState::Invalid;
    }

    ALint SourceState;
    alGetSourcei( Source, AL_SOURCE_STATE, &SourceState );

    switch(SourceState)
    {
        case AL_PLAYING:
            return ESoundState::Playing;
        case AL_PAUSED:
            return ESoundState::Paused;
        case AL_INITIAL:
        case AL_STOPPED:
            return ESoundState::Idle;
        default:
            return ESoundState::Invalid;
    }
}

bool COALSound::SetVolume(const float aVolume)
{
    if( !IsValid() )
    {
        LOG( ESeverity::Error ) << "Using Invalid Sound\n";
        return false;
    }

    Volume = Math::Clamp( aVolume, 0.0f, 1.0f );
    alSourcef( Source, AL_GAIN, Muted ? 0.0f : Volume );

    OAL::CheckOALError();
    return true;
}

void COALSound::SetMuted(const bool aMute)
{
    if( !IsValid() )
    {
        LOG( ESeverity::Error ) << "Using Invalid Sound\n";
        return;
    }

    Muted = aMute;
    alSourcef( Source, AL_GAIN, Muted ? 0.0f : Volume );
    OAL::CheckOALError();
}

void COALSound::SetPosition(const Vector3& Position)
{
    if( !IsValid() )
    {
        LOG( ESeverity::Error ) << "Using Invalid Sound\n";
        return;
    }

    alSource3f( Source, AL_POSITION, Position.x, Position.y, Position.z );
    OAL::CheckOALError();
}

void COALSound::SetLooped(const bool aLooped)
{
    if( !IsValid() )
    {
        LOG( ESeverity::Error ) << "Using Invalid Sound\n";
        return;
    }

    Looped = aLooped;
    alSourcei( Source, AL_LOOPING, Looped ? AL_TRUE : AL_FALSE );
    OAL::CheckOALError();
}

void COALSound::SetSoundData(ISoundData* aData)
{
    Stop();
    Data = dynamic_cast<COALSoundData*>(aData);
    if( Data )
    {
        alSourcei( Source, AL_BUFFER, Data->GetBufferID() );
    }
    else
    {
        alSourcei( Source, AL_BUFFER, 0 );
    }
    OAL::CheckOALError();
}

void COALSound::SetOffset(const int Offset)
{
    if( !IsValid() )
    {
        LOG( ESeverity::Error ) << "Using Invalid Sound\n";
        return;
    }

    if( Data && (Offset < Data->GetSamples()) )
    {
        alSourcei( Source, AL_SAMPLE_OFFSET, Offset ); 
        OAL::CheckOALError();
    }
}

int COALSound::GetOffset() const
{
    if( !IsValid() )
    {
        LOG( ESeverity::Error ) << "Using Invalid Sound\n";
        return 0;
    }

    ALint Offset;
    alGetSourcei( Source, AL_SAMPLE_OFFSET, &Offset );
    OAL::CheckOALError();
    
    return static_cast<int>(Offset);
}