#pragma once
#include "../Audio/Audio.hpp"
#include <AL/al.h>
#include <AL/alc.h>
#include <vector>

class COALAudio: public IAudio
{
public:
    COALAudio();
    ~COALAudio();

    bool Init(const SEngineParams&) override;
    void Exit() override;

    std::unique_ptr<ISoundData> CreateSoundData(const std::string&, CResources*) override;

    ISound* CreateSound() override { return CreateSound(nullptr); }
    ISound* CreateSound(ISoundData*) override;
    void DestroySound(ISound*) override;

    float GetVolume() const override { return Volume; }
    void SetVolume(const float) override;

    void SetMuted(const bool Value) override;
    bool IsMuted() const override { return Muted; }

    void SetListenerPosition(const Vector3&) override;
    void SetListenerVelocity(const Vector3&) override;

    void StopAll() override;
private:
    ALCdevice* Device = nullptr;
    ALCcontext* Context = nullptr;
    std::vector<std::unique_ptr<ISound>> Sounds;
    float Volume = 0.75f;
    bool Muted = false;
};


