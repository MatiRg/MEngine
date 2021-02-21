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

    float GetVolume() const override { return Volume; }
    void SetVolume(const float x) override { Volume = x; }

    void SetMuted(const bool Value) override { Muted = Value; }
    bool IsMuted() const override { return Muted; }

    void SetListenerPosition(const Vector3& x) override { Position = x; }
    const Vector3& GetListenerPosition() const override { return Position; }

    void SetListenerDirection(const Vector3& x) override { Forward = x; }
    const Vector3& GetListenerDirection() const override { return Forward; }

    void SetListenerVelocity(const Vector3& x) override { Velocity = x; }
    const Vector3& GetListenerVelocity() const override { return Velocity; }
private:
    bool Muted = false;
    float Volume = 0.75f;
    Vector3 Position = VECTOR3_ZERO;
    Vector3 Forward = VECTOR3_FORWARD;
    Vector3 Velocity = VECTOR3_ZERO;
};


