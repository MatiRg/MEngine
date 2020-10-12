#pragma once
#include "../Audio/Sound.hpp"
#include "OALSoundData.hpp"
#include <memory>
#include <AL/al.h>

class COALAudio;
class COALSoundData;

class COALSound: public ISound
{
public:
    COALSound(COALAudio*, COALSoundData* = nullptr);
    ~COALSound();

    bool CreateSource();

    bool IsValid() const override { return Valid; }

    bool Play() override;
    bool Pause(const bool) override;
    bool Stop() override;

    ESoundState GetState() override;

    // From 0.0 To 1.0
    bool SetVolume(const float) override;

    void SetMuted(const bool) override;
    bool IsMuted() const override { return Muted; }

    void SetLooped(const bool) override;

    void SetPosition(const Vector3&) override;

    void SetOffset(const int) override;
    int GetOffset() const override;

    void SetSoundData(ISoundData* aData) override;
    ISoundData* GetSoundData() const override { return Data; }
private:
    COALAudio* Audio = nullptr;
    COALSoundData* Data = nullptr;
    ALuint Source = 0u;
    bool Valid = false;
    float Volume = 0.5f;
    bool Muted = false;
    bool Looped = false;
};


