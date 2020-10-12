#pragma once
#include "../Component.hpp"
#include "../../Audio/Sound.hpp"

class CAudioSource2D: public IComponent
{
public:
    CAudioSource2D(CEngine*);

    COMPONENT(CAudioSource2D)

    bool OnLoad(CXMLElement*) override;
    bool OnSave(CXMLElement*) override;

    void OnEnabled() override;
    void OnDisabled() override;

    void OnCreate() override;
    void OnDestroy() override;

    void OnBeginFrame() override;

    void SetMuted(const bool);
    bool IsMuted() const { return Muted; }

    void SetVolume(const float);
    float GetVolume() const { return Volume; }

    void SetLooped(const bool);
    bool IsLooped() const { return Looped; }

    void Play();
    void Pause(const bool);
    void Stop();

    void SetData(ISoundData*);
    ISoundData* GetData() const { return Data; }
private:
    ISound* Sound = nullptr;
    bool Muted = false;
    float Volume = 1.0f;
    bool Looped = false;
    ISoundData* Data = nullptr;
    ESoundState OldState = ESoundState::Invalid;
};
