#pragma once
#include "../../Audio/SoundData.hpp"

class CNullSoundData: public ISoundData
{
public:
    CNullSoundData(const std::string&, CResources*);
    ~CNullSoundData();

    bool Load() override;
    bool IsValid() const override { return true; }

    int GetSamples() const override { return 0; }
};

