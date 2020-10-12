#pragma once
#include "../Audio/SoundData.hpp"
#include <AL/al.h>

class COALSoundData: public ISoundData
{
public:
    COALSoundData(const std::string&, CResources*);
    ~COALSoundData();

    bool Load() override;
    bool IsValid() const override { return Valid; }

    int GetSamples() const override;

    ALuint GetBufferID() const { return Buffer; }
private:
    bool Valid = false;
    ALuint Buffer = 0;
};

