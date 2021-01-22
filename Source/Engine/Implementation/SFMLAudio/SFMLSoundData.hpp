#pragma once
#include "../../Audio/SoundData.hpp"
#include <SFML/Audio.hpp>

class CSFMLSoundData: public ISoundData
{
public:
    CSFMLSoundData(const std::string&);
    ~CSFMLSoundData();

    bool Load(CResources*, const ResourceCreateMap&) override;

    int GetDuration() const override;

    const sf::SoundBuffer& GetBuffer() const { return Buffer; }
private:
    sf::SoundBuffer Buffer;
};

