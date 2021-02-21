#pragma once
#include "../../Audio/Sound.hpp"

class CNullSound: public ISound
{
public:
    CNullSound(ISoundData*);
    ~CNullSound();

    bool IsValid() const override { return true; }

    void Play() override;
    void Pause(const bool) override;
    void Stop() override;

    ESoundState GetState() override { return ESoundState::Idle; }

    void SetRelative(const bool x) override { Relative = x; }
    bool IsRelative() const override { return Relative; }

    void SetMinDistance(const float x) override { MinDistance = x; }
    float GetMinDistance() const override { return MinDistance; }

    void SetAttenuation(const float x) override { Attenuation = x; }
    float GetAttenuation() const override { return Attenuation; }

    // From 0.0 To 1.0
    void SetVolume(const float x) override { Volume = x; }
    float GetVolume() const override { return Volume; }

    void SetMuted(const bool aMuted) override { Muted = aMuted; }
    bool IsMuted() const override { return Muted; }

    void SetLooped(const bool x) override { Looped = x; }
    bool IsLooped() const override { return Looped; }

    void SetPosition(const Vector3& x) override { Position = x; }
    const Vector3& GetPosition() const override { return Position; }

    void SetOffset(const int x) override { Offset = 0; }
    int GetOffset() const override { return Offset; }

    void SetSoundData(ISoundData* aData) override { Data = aData; }
    ISoundData* GetSoundData() const override { return Data; }
private:
    ISoundData* Data = nullptr;
    float Volume = 0.5f;
    bool Muted = false;
    bool Looped = false;
    bool Relative = false;
    Vector3 Position = VECTOR3_ZERO;
    float MinDistance = 1.0f;
    float Attenuation = 1.0f;
    int Offset = 0;
};


