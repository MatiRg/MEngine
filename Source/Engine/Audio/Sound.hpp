#pragma once
#include "../Core/NonCopyable.hpp"
#include "../Math/Vector3.hpp"

class ISoundData;

enum class ESoundState
{
    Invalid = 0,
    Idle,
    Paused,
    Playing
};

class ISound: public NonCopyableMovable
{
public:
    ISound() {}
    virtual ~ISound() {}

    virtual bool IsValid() const = 0;

    virtual bool Play() = 0;
    virtual bool Pause(const bool) = 0;
    virtual bool Stop() = 0;

    virtual ESoundState GetState() = 0;

    // 0.0 To 1.0
    virtual bool SetVolume(const float) = 0;

    virtual void SetMuted(const bool) = 0;
    virtual bool IsMuted() const = 0;

    virtual void SetLooped(const bool) = 0;

    virtual void SetPosition(const Vector3&) = 0;

    // In Samples
    virtual void SetOffset(const int) = 0;
    // In Samples
    virtual int GetOffset() const = 0;

    virtual void SetSoundData(ISoundData*) = 0;
    virtual ISoundData* GetSoundData() const = 0;
};

