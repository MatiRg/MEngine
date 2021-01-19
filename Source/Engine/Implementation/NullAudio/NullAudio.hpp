#pragma once
#include "../../Audio/Audio.hpp"

class CNullAudio: public IAudio
{
public:
    CNullAudio();
    ~CNullAudio();

    bool Init(const SEngineParams&) override;
    void Exit() override;

    std::unique_ptr<ISoundData> CreateSoundData(const std::string&) override;
    
    std::unique_ptr<ISound> CreateSound() override { return CreateSound(nullptr); }
    std::unique_ptr<ISound> CreateSound(ISoundData*) override;

    float GetVolume() const override { return 0.0f; }
    void SetVolume(const float) override;

    void SetMuted(const bool Value) override { Muted = Value; }
    bool IsMuted() const override { return Muted; }

    void SetListenerPosition(const Vector3&) override {}
    void SetListenerVelocity(const Vector3&) override {}
private:
    bool Muted = false;
};


