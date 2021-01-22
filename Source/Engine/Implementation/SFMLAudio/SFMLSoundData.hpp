#pragma once
#include "../../Audio/SoundData.hpp"
#include <SFML/Audio.hpp>
#include <memory>

class CSFMLSoundData: public ISoundData
{
public:
    CSFMLSoundData(const std::string&);
    ~CSFMLSoundData();

    bool Load(CResources*, const ResourceCreateMap&) override;

    bool IsStream() const override { return Stream; }

    int GetDuration() const override;

    const sf::SoundBuffer& GetBuffer() const { return *Buffer; }
private:
    std::unique_ptr<sf::SoundBuffer> Buffer;
    bool Stream = false;
};

