#pragma once
#include "../../Audio/SoundData.hpp"

class CNullSoundData: public ISoundData
{
public:
    CNullSoundData(const std::string&);
    ~CNullSoundData();

    bool Load(CResources*, const ResourceCreateMap&) override;

    int GetDuration() const override { return 0; }
};

