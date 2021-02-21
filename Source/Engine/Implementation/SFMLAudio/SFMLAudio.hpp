#pragma once
#include "../../Audio/Audio.hpp"
#include <SFML/Audio.hpp>

class CSFMLAudio: public IAudio
{
public:
    CSFMLAudio();
    ~CSFMLAudio();

    bool Init(const SEngineParams&) override;
    void Exit() override;

    std::unique_ptr<ISoundData> CreateSoundData(const std::string&) override;

    std::unique_ptr<ISound> CreateSound() override { return CreateSound(nullptr); }
    std::unique_ptr<ISound> CreateSound(ISoundData*) override;

    float GetVolume() const override { return Volume; }
    void SetVolume(const float) override;

    void SetMuted(const bool Value) override;
    bool IsMuted() const override { return Muted; }

    void SetListenerPosition(const Vector3&) override;
    const Vector3& GetListenerPosition() const override { return Position; }

    void SetListenerDirection(const Vector3&) override;
    const Vector3& GetListenerDirection() const override { return Forward; }

    // Not Supported By SFML
    void SetListenerVelocity(const Vector3&) override;
    // Not Supported By SFML
    const Vector3& GetListenerVelocity() const override { return Velocity; }
private:
    float Volume = 0.75f;
    bool Muted = false;
    Vector3 Position = VECTOR3_ZERO;
    Vector3 Forward = VECTOR3_FORWARD;
    Vector3 Velocity = VECTOR3_ZERO;
};


