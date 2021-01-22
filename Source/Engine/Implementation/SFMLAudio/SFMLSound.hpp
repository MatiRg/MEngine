#pragma once
#include "../../Audio/Sound.hpp"
#include "SFMLSoundData.hpp"

class CSFMLSound: public ISound
{
public:
    CSFMLSound(CSFMLSoundData*);
    ~CSFMLSound();

    bool CreateSource();

    bool IsValid() const override { return Valid; }

    void Play() override;
    void Pause(const bool) override;
    void Stop() override;

    ESoundState GetState() override;

    void SetRelative(const bool) override;
    bool IsRelative() const override { return Relative; }

    void SetMinDistance(const float) override;
    float GetMinDistance() const override { return MinDistance; }

    void SetAttenuation(const float) override;
    float GetAttenuation() const override { return Attenuation; }

    // From 0.0 To 1.0
    void SetVolume(const float) override;
    float GetVolume() const override { return Volume; }

    void SetMuted(const bool) override;
    bool IsMuted() const override { return Muted; }

    void SetLooped(const bool) override;
    bool IsLooped() const override { return Looped; }

    void SetPosition(const Vector3&) override;
    const Vector3& GetPosition() const override { return Position; }

    void SetOffset(const int) override;
    int GetOffset() const override;

    void SetSoundData(ISoundData* aData) override;
    ISoundData* GetSoundData() const override { return Data; }
private:
    CSFMLSoundData* Data = nullptr;
    sf::Sound Sound;
    bool Valid = false;
    float Volume = 0.5f;
    bool Muted = false;
    bool Looped = false;
    bool Relative = false;
    Vector3 Position = Vector3::ZERO;
    float MinDistance = 1.0f;
    float Attenuation = 1.0f;
};


