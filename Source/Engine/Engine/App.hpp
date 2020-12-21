#pragma once
#include "EngineParams.hpp"
#include "Config.hpp"
#include "../Core/NonCopyable.hpp"
#include <memory>
#include <vector>
#include <string>

class IContext;
class ISystem;
class IEventQueue;
class IInput;
class IWindow;
class IGraphics;
class IAudio;
class CEngineUpdater;
class CUserUpdater;
class CResources;
class CAudioHandler;
class CRenderer3D;
class CDrawer2D;
class CRenderer2D;
class IPhysics3D;
class IPhysics2D;
class CImGUI;
class CScene;
class CScriptModule;
class CEngine;

class CApp: public NonCopyableMovable
{
public:
    CApp();
    virtual ~CApp();

    int Run(const std::vector<std::string>&);
    void Quit();

    CEngine* GetEngine() const;
    ISystem* GetSystem() const;
    IEventQueue* GetEventQueue() const;
    IInput* GetInput() const;
    IWindow* GetWindow() const;
    IGraphics* GetGraphics() const;
    IAudio* GetAudio() const;
    IPhysics3D* GetPhysics3D() const;
    IPhysics2D* GetPhysics2D() const;
    CEngineUpdater* GetEngineUpdater() const;
    CUserUpdater* GetUserUpdater() const;
    CResources* GetResources() const;
    CAudioHandler* GetAudioHandler() const;
    CRenderer3D* GetRenderer3D() const;
    CDrawer2D* GetDrawer2D() const;
    CRenderer2D* GetRenderer2D() const;
    CImGUI* GetImGUI() const;
    CScene* GetScene() const;
    CScriptModule* GetScriptModule() const;
protected:
    virtual bool PreInit() { return true; } // Before Engine & Context Creation, Before Config Loading
    virtual bool Init() { return true; } // After Context & Engine Creation, Before Engine Init
    virtual void OnInit() {} // Before Engine OnInit - Game oriented OnInit
    virtual void OnExit() {} // After Main Loop, Before Engine OnExit
    virtual void Exit() {} // Before Engine Exit
    virtual void PostExit() {} // Before Engine & Context Destruction, After Engine Exit
protected:
    SEngineParams Params;
    bool UseConfigFile = true;
    std::string ConfigFile = "Engine.xml";
    std::vector<std::string> CommandLine;
private:
    std::unique_ptr<IContext> Context;
    std::unique_ptr<CEngine> Engine;
    std::unique_ptr<CConfig> Config;
};
