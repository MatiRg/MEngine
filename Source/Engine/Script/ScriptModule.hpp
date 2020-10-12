#pragma once
#include "../Engine/EngineModule.hpp"
#include <string>
#include <memory>

class CEngine;

class CScriptModule final: public IEngineModule
{
public:
    CScriptModule(CEngine*);
    ~CScriptModule();

    ENGINE_MODULE(CScriptModule)

    bool Init(const SEngineParams&) override;
    void Exit() override;
private:
    bool InitBindings();
private:
    CEngine* Engine = nullptr;
};
