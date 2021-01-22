#pragma once
#include "../Component.hpp"

class IAudio;

class CAudioListener: public IComponent
{
public:
    CAudioListener(CEngine*);

    COMPONENT(CAudioListener)

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
private:
    IAudio* Audio = nullptr;
    bool OrginalMute = false;
    bool Muted = false;
    float OrginalVolume = 1.0f;
    float Volume = 1.0f;
};
