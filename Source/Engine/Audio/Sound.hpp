#pragma once
#include "../Core/NonCopyable.hpp"
#include "../Math/Vector3.hpp"
#include "../System/MemoryManager.hpp"

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
    ISound() 
    {
        ADD_MEMORY_RECORD(this);
    }

    virtual ~ISound() 
    {
        ERASE_MEMORY_RECORD(this);
    }

    virtual bool IsValid() const = 0;

    virtual void Play() = 0;
    virtual void Pause(const bool) = 0;
    virtual void Stop() = 0;

    virtual ESoundState GetState() = 0;

    virtual void SetRelative(const bool) = 0;
    virtual bool IsRelative() const = 0;

    virtual void SetMinDistance(const float) = 0;
    virtual float GetMinDistance() const = 0;

    virtual void SetAttenuation(const float) = 0;
    virtual float GetAttenuation() const = 0;

    // 0.0 To 1.0
    virtual void SetVolume(const float) = 0;
    virtual float GetVolume() const = 0;

    virtual void SetMuted(const bool) = 0;
    virtual bool IsMuted() const = 0;

    virtual void SetLooped(const bool) = 0;
    virtual bool IsLooped() const = 0;

    virtual void SetPosition(const Vector3&) = 0;
    virtual const Vector3& GetPosition() const = 0;

    // In Samples
    virtual void SetOffset(const int) = 0;
    // In Samples
    virtual int GetOffset() const = 0;

    virtual void SetSoundData(ISoundData*) = 0;
    virtual ISoundData* GetSoundData() const = 0;
};

