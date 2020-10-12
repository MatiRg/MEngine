#pragma once
#include "../Engine/EngineModule.hpp"
#include <memory>
#include <string>
#include <map>

class IAudio;
class CResources;
class ISound;

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
    ISound* Music = nullptr;
    std::map<std::string, ISound*> Sounds;
};
