#pragma once
#include "../../Audio/Sound.hpp"

class CNullSound: public ISound
{
public:
    CNullSound();
    ~CNullSound();

    bool IsValid() const override { return true; }

    bool Play() override;
    bool Pause(const bool) override;
    bool Stop() override;

    ESoundState GetState() override { return ESoundState::Idle; }

    // From 0.0 To 1.0
    bool SetVolume(const float) override;

    void SetMuted(const bool aMuted) override { Muted = aMuted; }
    bool IsMuted() const override { return Muted; }

    void SetLooped(const bool) override {}

    void SetPosition(const Vector3&) override {}

    void SetOffset(const int) override {}
    int GetOffset() const override { return 0; }

    void SetSoundData(ISoundData* aData) override { Data = aData; }
    ISoundData* GetSoundData() const override { return Data; }
private:
    bool Muted = false;
    ISoundData* Data = nullptr;
};


