#pragma once
#include "../Component.hpp"
#include "../../Audio/Sound.hpp"

class CAudioSource: public IComponent
{
public:
    CAudioSource(CEngine*);

    COMPONENT(CAudioSource)

    bool OnLoad(CXMLElement*) override;
    bool OnSave(CXMLElement*) override;

    void OnEnabled() override;
    void OnDisabled() override;

    void OnCreate() override;
    void OnDestroy() override;

    void OnLateUpdate(const float) override;

    void SetMuted(const bool);
    bool IsMuted() const { return Muted; }

    void SetVolume(const float);
    float GetVolume() const { return Volume; }

    void SetLooped(const bool);
    bool IsLooped() const { return Looped; }

    void SetRelative(const bool);
    bool IsRelative() const { return Relative; }

    void SetMinDistance(const float);
    float GetMinDistance() const { return MinDistance; }

    void SetAttenuation(const float);
    float GetAttenuation() const { return Attenuation; }

    void Play();
    void Pause(const bool);
    void Stop();

    void SetData(ISoundData*);
    ISoundData* GetData() const { return Data; }
private:
    std::unique_ptr<ISound> Sound;
    bool Muted = false;
    float Volume = 1.0f;
    bool Looped = false;
    ISoundData* Data = nullptr;
    ESoundState OldState = ESoundState::Invalid;
    bool Relative = false;
    float MinDistance = 1.0f;
    float Attenuation = 1.0f;
};
