#include "SFMLSound.hpp"
#include "../../Math/Functions.hpp"
#include "../../Core/Log.hpp"

template<class T>
class TSFMLSoundSource: public ISFMLSoundSource
{
public:
    TSFMLSoundSource() = default;
    virtual ~TSFMLSoundSource() = default;

    void Play() override
    {
        Sound.play();
    }

    void Pause(const bool State) override
    {
        if (State)
        {
            Sound.pause();
        }
        else
        {
            Sound.play();
        }
    }

    void Stop() override
    {
        Sound.stop();
    }

    ESoundState GetState() override
    {
        switch (Sound.getStatus())
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

    void SetVolume(const float Volume) override
    {
        Sound.setVolume(Volume * 100.0f);
    }

    void SetMuted(const bool Muted, const float Volume) override
    {
        Sound.setVolume(Muted ? 0.0f : (Volume * 100.0f));
    }

    void SetRelative(const bool Relative) override
    {
        Sound.setRelativeToListener(Relative);
    }

    void SetMinDistance(const float MinDistance) override
    {
        Sound.setMinDistance(MinDistance);
    }

    void SetAttenuation(const float Attenuation) override
    {
        Sound.setAttenuation(Attenuation);
    }

    void SetPosition(const Vector3& Position) override
    {
        Sound.setPosition(sf::Vector3f(Position.x, Position.y, Position.z));
    }

    void SetLooped(const bool Looped) override
    {
        Sound.setLoop(Looped);
    }

    int GetOffset() const override
    {
        return Sound.getPlayingOffset().asMilliseconds();
    }
protected:
    T Sound;
};

class CSFMLSoundSource : public TSFMLSoundSource<sf::Sound>
{
public:
    CSFMLSoundSource() = default;
    ~CSFMLSoundSource() = default;

    void SetOffset(const int Offset) override
    {
        if( Offset < Data->GetDuration() )
        {
            Sound.setPlayingOffset(sf::milliseconds(Offset));
        }
    }

    bool SetSoundData(CSFMLSoundData* aData) override
    {
        Data = aData;
        Sound.setBuffer( Data->GetBuffer() );
        return true;
    }
private:
    CSFMLSoundData* Data = nullptr;
};

class CSFMLStreamSource : public TSFMLSoundSource<sf::Music>
{
public:
    CSFMLStreamSource() = default;
    ~CSFMLStreamSource() = default;

    void SetOffset(const int Offset) override
    {
        if (Offset < Sound.getDuration().asMilliseconds() )
        {
            Sound.setPlayingOffset(sf::milliseconds(Offset));
        }
    }

    bool SetSoundData(CSFMLSoundData* Data) override
    {
        if( !Sound.openFromFile(Data->GetPath()) )
        {
            return false;
        }
        return true;
    }
};

///////////////////////////////////////////////////////////////

CSFMLSound::CSFMLSound(CSFMLSoundData* aData):
    Data( aData )
{
}

CSFMLSound::~CSFMLSound()
{
}

bool CSFMLSound::CreateSource()
{
    Sound.reset();
    if (Data)
    {
        if( Data->IsStream() )
        {
            Sound = std::make_unique<CSFMLStreamSource>();
        }
        else
        {
            Sound = std::make_unique<CSFMLSoundSource>();
        }

        if( !Sound->SetSoundData(Data) )
        {
            LOG(ESeverity::Error) << "Unable to create ISound form File: " << Data->GetName() << "\n";
            Valid = false;
            return false;
        }

        Sound->SetVolume(Volume);
        Sound->SetMuted(Muted, Volume);
        Sound->SetRelative(Relative);
        Sound->SetLooped(Looped);
        Sound->SetMinDistance(MinDistance);
        Sound->SetAttenuation(Attenuation);
        Sound->SetPosition(Position);
    }

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

    if (Sound)
    {
        Sound->Play();
    }
}

void CSFMLSound::Pause(const bool State)
{
    if( !IsValid() )
    {
        LOG( ESeverity::Error ) << "Using Invalid Sound\n";
        return;
    }

    if (Sound)
    {
        Sound->Pause(State);
    }
}

void CSFMLSound::Stop()
{
    if( !IsValid() )
    {
        LOG( ESeverity::Error ) << "Using Invalid Sound\n";
        return;
    }

    if (Sound)
    {
        Sound->Stop();
    }
}

ESoundState CSFMLSound::GetState()
{
    if( !IsValid() )
    {
        LOG( ESeverity::Error ) << "Using Invalid Sound\n";
        return ESoundState::Invalid;
    }

    if( !Sound )
    {
        return ESoundState::Invalid;
    }

    return Sound->GetState();
}

void CSFMLSound::SetVolume(const float aVolume)
{
    if( !IsValid() )
    {
        LOG( ESeverity::Error ) << "Using Invalid Sound\n";
        return;
    }

    Volume = Math::Clamp( aVolume, 0.0f, 1.0f );
    if (Sound)
    {
        Sound->SetVolume(Volume);
    }
}

void CSFMLSound::SetMuted(const bool aMute)
{
    if( !IsValid() )
    {
        LOG( ESeverity::Error ) << "Using Invalid Sound\n";
        return;
    }

    Muted = aMute;
    if (Sound)
    {
        Sound->SetMuted(Muted, Volume);
    }
}

void CSFMLSound::SetRelative(const bool aRelative)
{
    if (!IsValid())
    {
        LOG(ESeverity::Error) << "Using Invalid Sound\n";
        return;
    }

    Relative = aRelative;
    if (Sound)
    {
        Sound->SetRelative(Relative);
    }
}

void CSFMLSound::SetMinDistance(const float aMinDistance)
{
    if (!IsValid())
    {
        LOG(ESeverity::Error) << "Using Invalid Sound\n";
        return;
    }

    MinDistance = aMinDistance;
    if (Sound)
    {
        Sound->SetMinDistance(MinDistance);
    }
}

void CSFMLSound::SetAttenuation(const float aAttenuation)
{
    if (!IsValid())
    {
        LOG(ESeverity::Error) << "Using Invalid Sound\n";
        return;
    }

    Attenuation = aAttenuation;
    if (Sound)
    {
        Sound->SetAttenuation(Attenuation);
    }
}

void CSFMLSound::SetPosition(const Vector3& aPosition)
{
    if( !IsValid() )
    {
        LOG( ESeverity::Error ) << "Using Invalid Sound\n";
        return;
    }

    Position = aPosition;
    if (Sound)
    {
        Sound->SetPosition(Position);
    }
}

void CSFMLSound::SetLooped(const bool aLooped)
{
    if( !IsValid() )
    {
        LOG( ESeverity::Error ) << "Using Invalid Sound\n";
        return;
    }

    Looped = aLooped;
    if (Sound)
    {
        Sound->SetLooped(Looped);
    }
}

void CSFMLSound::SetSoundData(ISoundData* aData)
{
    Stop();
    Data = dynamic_cast<CSFMLSoundData*>(aData);
    if( Data )
    {
        CreateSource();
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

    if (Sound)
    {
        Sound->SetOffset(Offset);
    }
}

int CSFMLSound::GetOffset() const
{
    if( !IsValid() )
    {
        LOG( ESeverity::Error ) << "Using Invalid Sound\n";
        return 0;
    }

    if (!Sound)
    {
        return 0;
    }

    return Sound->GetOffset();
}