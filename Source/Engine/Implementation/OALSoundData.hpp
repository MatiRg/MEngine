#pragma once
#include "../Audio/SoundData.hpp"
#include <AL/al.h>

class COALSoundData: public ISoundData
{
public:
    COALSoundData(const std::string&);
    ~COALSoundData();

    bool Load(CResources*, const ResourceCreateMap&) override;

    int GetSamples() const override;

    ALuint GetBufferID() const { return Buffer; }
private:
    ALuint Buffer = 0;
};

