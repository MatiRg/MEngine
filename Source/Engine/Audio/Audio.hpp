#pragma once
#include "../Engine/EngineModule.hpp"
#include "../Math/Vector3.hpp"
#include "Sound.hpp"
#include <memory>

class ISoundData;

class IAudio: public IEngineModule
{
public:
    IAudio(): IEngineModule( "Audio" ) {}
    virtual ~IAudio() {}

    ENGINE_MODULE(IAudio)

    // Name - Resources Usage
    virtual std::unique_ptr<ISoundData> CreateSoundData(const std::string&) = 0;

    virtual std::unique_ptr<ISound> CreateSound() = 0;
    virtual std::unique_ptr<ISound> CreateSound(ISoundData*) = 0;

    virtual float GetVolume() const = 0;
    // 0.0 To 1.0
    virtual void SetVolume(const float) = 0;

    virtual void SetMuted(const bool) = 0;
    virtual bool IsMuted() const = 0;

    virtual void SetListenerPosition(const Vector3&) = 0;
    virtual const Vector3& GetListenerPosition() const = 0;

    virtual void SetListenerDirection(const Vector3&) = 0;
    virtual const Vector3& GetListenerDirection() const = 0;

    virtual void SetListenerVelocity(const Vector3&) = 0;
    virtual const Vector3& GetListenerVelocity() const = 0;
};


