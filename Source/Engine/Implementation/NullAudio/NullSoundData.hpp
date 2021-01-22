#pragma once
#include "../../Audio/SoundData.hpp"

class CNullSoundData: public ISoundData
{
public:
    CNullSoundData(const std::string&);
    ~CNullSoundData();

    bool Load(CResources*, const ResourceCreateMap&) override;

    bool IsStream() const override { return Stream; }

    int GetDuration() const override { return 0; }
private:
    bool Stream = false;
};

