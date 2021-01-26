#pragma once
#include "EngineParams.hpp"
#include "Config.hpp"
#include "../Core/Types.hpp"
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
class CPerformanceCounters;
class CEngine;

/**
  \class CApp
  \brief Creates Engine, Runs Engine Main Loop, Derrive from this class and overwrite virtual methods to start you game creation.

  This class encapsulates and creates CEngine. It has virtual methods that can be used to do game init.
  It is managing CEngine creation and destruction steps.Derrive from this class and overwrite virtual methods to start you game creation.
  Use ENTRY_POINT macro to create EngineMain function somewhere in .cpp file.
*/
class CApp: public NonCopyableMovable
{
public:
    CApp();
    virtual ~CApp();

    /**
     * \brief Runs Engine Main Loop, creates Engine
     * \param Args Command Line Arguments
     * \return Will return 0 on success or any other number on error
     */
    int Run(const StringVec& Args);
    //! Quits Engine/Application
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
    CPerformanceCounters* GetPerformanceCounters() const;
protected:
    /**
     * Virtual method that is called before Engine, Context and Config is Created.
     * Can be used to change Config File Name or do not load config file at all.
     * \return Return true on success or false if not
     */
    virtual bool PreInit() { return true; } 
    /**
     * Virtual method that is called after Engine and Context is Created.
     * Can be used to creating Engine Modules in CEngineUpdater and creating Updatables and setting them to global or 
     * some named container in CUserUpdater.Can also be used to setup Resource Search directories in CResources class.
     * \return Return true on success or false if not
     */
    virtual bool Init() { return true; } 
    /**
     * Virtual method that is called just before Engine oriented OnInit.
     * Can be used to setup start container in CUserUpdater, setting window caption in IWindow, preloading game assets in CResources.
     */
    virtual void OnInit() {}
    /**
     * Virtual method that is called just after Engine oriented OnExit.
     * Can be used to perform cleanup of resources that are tightly tied to initialized engine modules.
     */
    virtual void OnExit() {} 
    /**
     * Virtual method that is called just after Engine and it`s modules are shutdown.
     * Can be used to destroy Updatables and user Engine modules.
     */
    virtual void Exit() {} 
    /**
     * Virtual method that is called just after Engine, it`s modules and config is destroyed.
     */
    virtual void PostExit() {}
protected:
    //! Structure holding Engine Init Variables that have default values
    SEngineParams Params;
    //! Loading config from file
    bool UseConfigFile = true;
    //! Config file Name
    std::string ConfigFile = "Engine.xml";
    //! Command Line Options
    StringVec CommandLine;
private:
    std::unique_ptr<IContext> Context;
    std::unique_ptr<CEngine> Engine;
    std::unique_ptr<CConfig> Config;
};
