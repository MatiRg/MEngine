#include "SFMLSound.hpp"
#include "../../Math/Functions.hpp"
#include "../../Core/Log.hpp"

CSFMLSound::CSFMLSound(CSFMLSoundData* aData):
    Data( aData )
{
}

CSFMLSound::~CSFMLSound()
{
}

bool CSFMLSound::CreateSource()
{
    if( Data )
    {
        Sound.setBuffer( Data->GetBuffer() );
    }

    Sound.setVolume(Volume * 100.0f);
    Sound.setRelativeToListener(Relative);
    Sound.setLoop(Looped);
    Sound.setMinDistance(MinDistance);
    Sound.setAttenuation(Attenuation);
    Sound.setPosition(sf::Vector3f(Position.x, Position.y, Position.z));

    Valid = true;
    return true;
}

void CSFMLSound::Play()
{
    if( !IsValid() )
    {
        LOG( ESeverity::Error ) << "Using Invalid Sound\n";
        return;
    }

    Sound.play();
}

void CSFMLSound::Pause(const bool State)
{
    if( !IsValid() )
    {
        LOG( ESeverity::Error ) << "Using Invalid Sound\n";
        return;
    }

    if( State )
    {
        Sound.pause();
    }
    else
    {
        Sound.play();
    }
}

void CSFMLSound::Stop()
{
    if( !IsValid() )
    {
        LOG( ESeverity::Error ) << "Using Invalid Sound\n";
        return;
    }

    Sound.stop();
}

ESoundState CSFMLSound::GetState()
{
    if( !IsValid() )
    {
        LOG( ESeverity::Error ) << "Using Invalid Sound\n";
        return ESoundState::Invalid;
    }

    switch(Sound.getStatus())
    {
        case sf::SoundSource::Status::Playing:
            return ESoundState::Playing;
        case sf::SoundSource::Status::Paused:
            return ESoundState::Paused;
        case sf::SoundSource::Status::Stopped:
            return ESoundState::Idle;
        default:
            return ESoundState::Invalid;
    }
}

void CSFMLSound::SetVolume(const float aVolume)
{
    if( !IsValid() )
    {
        LOG( ESeverity::Error ) << "Using Invalid Sound\n";
        return;
    }

    Volume = Math::Clamp( aVolume, 0.0f, 1.0f );

    Sound.setVolume( Volume * 100.0f );
}

void CSFMLSound::SetMuted(const bool aMute)
{
    if( !IsValid() )
    {
        LOG( ESeverity::Error ) << "Using Invalid Sound\n";
        return;
    }

    Muted = aMute;

    Sound.setVolume(Muted ? 0.0f : (Volume * 100.0f) );
}

void CSFMLSound::SetRelative(const bool aRelative)
{
    if (!IsValid())
    {
        LOG(ESeverity::Error) << "Using Invalid Sound\n";
        return;
    }

    Relative = aRelative;
    Sound.setRelativeToListener(Relative);
}

void CSFMLSound::SetMinDistance(const float aMinDistance)
{
    if (!IsValid())
    {
        LOG(ESeverity::Error) << "Using Invalid Sound\n";
        return;
    }

    MinDistance = aMinDistance;
    Sound.setMinDistance(MinDistance);
}

void CSFMLSound::SetAttenuation(const float aAttenuation)
{
    if (!IsValid())
    {
        LOG(ESeverity::Error) << "Using Invalid Sound\n";
        return;
    }

    Attenuation = aAttenuation;
    Sound.setAttenuation(Attenuation);
}

void CSFMLSound::SetPosition(const Vector3& aPosition)
{
    if( !IsValid() )
    {
        LOG( ESeverity::Error ) << "Using Invalid Sound\n";
        return;
    }

    Position = aPosition;
    Sound.setPosition(sf::Vector3f(Position.x, Position.y, Position.z));
}

void CSFMLSound::SetLooped(const bool aLooped)
{
    if( !IsValid() )
    {
        LOG( ESeverity::Error ) << "Using Invalid Sound\n";
        return;
    }

    Looped = aLooped;
    Sound.setLoop(Looped);
}

void CSFMLSound::SetSoundData(ISoundData* aData)
{
    Stop();
    Data = dynamic_cast<CSFMLSoundData*>(aData);
    if( Data )
    {
        Sound.setBuffer( Data->GetBuffer() );
    }
    else
    {
        LOG(ESeverity::Error) << "Using Invalid Sound Data - Fallback data not changed\n";
    }
}

void CSFMLSound::SetOffset(const int Offset)
{
    if( !IsValid() )
    {
        LOG( ESeverity::Error ) << "Using Invalid Sound\n";
        return;
    }

    if( Data && (Offset < Data->GetDuration()) )
    {
        Sound.setPlayingOffset( sf::milliseconds(Offset) );
    }
}

int CSFMLSound::GetOffset() const
{
    if( !IsValid() )
    {
        LOG( ESeverity::Error ) << "Using Invalid Sound\n";
        return 0;
    }

    return Sound.getPlayingOffset().asMilliseconds();
}