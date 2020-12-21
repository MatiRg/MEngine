#pragma once
#include "../../Audio/Audio.hpp"
#include <vector>

class CNullAudio: public IAudio
{
public:
    CNullAudio();
    ~CNullAudio();

    bool Init(const SEngineParams&) override;
    void Exit() override;

    std::unique_ptr<ISoundData> CreateSoundData(const std::string&) override;
    
    ISound* CreateSound() override { return CreateSound(nullptr); }
    ISound* CreateSound(ISoundData*) override;
    void DestroySound(ISound*) override;

    float GetVolume() const override { return 0.0f; }
    void SetVolume(const float) override;

    void SetMuted(const bool Value) override { Muted = Value; }
    bool IsMuted() const override { return Muted; }

    void SetListenerPosition(const Vector3&) override {}
    void SetListenerVelocity(const Vector3&) override {}

    void StopAll() override;
private:
    std::vector<std::unique_ptr<ISound>> Sounds;
    bool Muted = false;
};


