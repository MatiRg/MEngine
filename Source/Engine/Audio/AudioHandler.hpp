#pragma once
#include "../Engine/EngineModule.hpp"
#include "Audio.hpp"
#include <memory>
#include <string>
#include <map>

class CResources;

class CAudioHandler final: public IEngineModule
{
public:
    CAudioHandler(IAudio*, CResources*);
    ~CAudioHandler();

    ENGINE_MODULE(CAudioHandler)

	void OnExit() override;

    void PlayMusic(const std::string&);
    void StopMusic();

    void PlaySound(const std::string&);
private:
    IAudio* Audio = nullptr;
    CResources* Resources = nullptr;
    std::unique_ptr<ISound> Music;
    std::map<std::string, std::unique_ptr<ISound>> Sounds;
};
