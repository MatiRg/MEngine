#pragma once
#include "EngineModule.hpp"
#include "../Core/NonCopyable.hpp"
#include <memory>
#include <type_traits>

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
class CDebugDrawer;
class IPhysics3D;
class IPhysics2D;
class CImGUI;
class CScene;
class CScriptModule;
class CPerformanceCounters;
struct SEngineParams;

/**
  \class CEngine
  \brief Encapsulated Main Loop, Creates Engine Modules
*/
class CEngine final: public NonCopyableMovable
{
public:
    /**
     * \brief Construct CEngine
     * \param aContext Pointer to class that implements IContext interface. Ownership is not taken.
     */
    CEngine(IContext* aContext);
    ~CEngine();

    /**
     * \brief Create Engine, called by CApp
     * \return Return true on success or false if not
     */
    bool Create();
    /**
     * \brief Init Engine, called by CApp
     * \param aParams Engine Creation Parameters
     * \return Return true on success or false if not
     */
    bool Init(const SEngineParams& aParams);
    //! OnInit Engine, called by CApp
    void OnInit();
    //! OnExit Engine, called by CApp
    void OnExit();
    //! Exit Engine, called by CApp
    void Exit();

    /**
     * \brief Setup Main Loop Timer to number of ticks
     * \param aTicks Number of ticks, example: 60 or 30
     */
    void SetupTicks(const uint32_t aTicks);

    //! Engine Main Loop - Fixed Delta Time for Simulation, Rendering at Different Framerates
    void Run();

    //! Exit Main Loop, called by CApp, Use CApp::Quit()
    void Quit() { Loop = false; }

    ISystem* GetSystem() const { return System; }
    IEventQueue* GetEventQueue() const { return EventQueue; }
    IInput* GetInput() const { return Input; }
    IWindow* GetWindow() const { return Window; }
    IGraphics* GetGraphics() const { return Graphics; }
    IAudio* GetAudio() const { return Audio; }
    IPhysics3D* GetPhysics3D() const { return Physics3D; }
    IPhysics2D* GetPhysics2D() const { return Physics2D; }
    CEngineUpdater* GetEngineUpdater() const { return EngineUpdater.get(); }
    CUserUpdater* GetUserUpdater() const { return UserUpdater.get(); }
    CResources* GetResources() const { return Resources.get(); }
    CAudioHandler* GetAudioHandler() const { return AudioHandler.get(); }
    CRenderer3D* GetRenderer3D() const { return Renderer3D.get(); }
    CDrawer2D* GetDrawer2D() const { return Drawer2D.get(); }
    CRenderer2D* GetRenderer2D() const { return Renderer2D.get(); }
    CDebugDrawer* GetDebugDrawer() const { return DebugDrawer.get(); }
    CImGUI* GetImGUI() const { return ImGUI.get(); }
    CScene* GetScene() const { return Scene.get(); }
    CScriptModule* GetScriptModule() const { return ScriptModule.get(); }
    CPerformanceCounters* GetPerformanceCounters() const { return Counter.get(); }

    /**
     * \brief Get Engine Module by Type Name
     * \param aType Name of the Class
     * \return Return Pointer if found given module or nullptr if not
     */
    IEngineModule* GetModule(const std::string& aType) const;
    /**
     * \brief Get Engine Module by Template, class must be derrivied from IEngineModule
     * \return Return Pointer if found given module or nullptr if not
     */
    template<class T>
    T* GetModule() const
    {
        static_assert(std::is_base_of<IEngineModule, T>::value, "Must be base of IEngineModule");
        return dynamic_cast<T*>( GetModule(T::GetTypeStatic()) );
    }
private:
    uint32_t Ticks = 0u;
    //! Delta Time Value
    float DeltaTime = 0.0f;
    float CurrentTime = 0.0f;
    float Accumulator = 0.0f; 
    bool Loop = true;
    IContext* Context = nullptr;
    ISystem* System = nullptr;
    IEventQueue* EventQueue = nullptr;
    IInput* Input = nullptr;
    IWindow* Window = nullptr;
    IGraphics* Graphics = nullptr;
    IAudio* Audio = nullptr;
    IPhysics3D* Physics3D = nullptr;
    IPhysics2D* Physics2D = nullptr;
    std::unique_ptr<CEngineUpdater> EngineUpdater;
    std::unique_ptr<CUserUpdater> UserUpdater;
    std::unique_ptr<CResources> Resources;
    std::unique_ptr<CAudioHandler> AudioHandler;
    std::unique_ptr<CRenderer3D> Renderer3D;
    std::unique_ptr<CDrawer2D> Drawer2D;
    std::unique_ptr<CRenderer2D> Renderer2D;
    std::unique_ptr<CDebugDrawer> DebugDrawer;
    std::unique_ptr<CImGUI> ImGUI;
    std::unique_ptr<CScene> Scene;
    std::unique_ptr<CScriptModule> ScriptModule;
    std::unique_ptr<CPerformanceCounters> Counter;
};

