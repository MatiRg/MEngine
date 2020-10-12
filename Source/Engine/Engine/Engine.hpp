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
class CDrawer2D;
class CRenderer2D;
class CPhysics2D;
class CImGUI;
class CScene;
class CScriptModule;
class CPerformanceCounter;
struct SEngineParams;

class CEngine final: public NonCopyableMovable
{
public:
    CEngine(IContext*);
    ~CEngine();

    bool Create();
    bool Init(const SEngineParams&);
    void OnInit();
    void OnExit();
    void Exit();

    void SetupTicks(const uint32_t);

    void Run();

    void Quit() { Loop = false; }

    ISystem* GetSystem() const { return System; }
    IEventQueue* GetEventQueue() const { return EventQueue; }
    IInput* GetInput() const { return Input; }
    IWindow* GetWindow() const { return Window; }
    IGraphics* GetGraphics() const { return Graphics; }
    IAudio* GetAudio() const { return Audio; }
    CEngineUpdater* GetEngineUpdater() const { return EngineUpdater.get(); }
    CUserUpdater* GetUserUpdater() const { return UserUpdater.get(); }
    CResources* GetResources() const { return Resources.get(); }
    CAudioHandler* GetAudioHandler() const { return AudioHandler.get(); }
    CDrawer2D* GetDrawer2D() const { return Drawer2D.get(); }
    CRenderer2D* GetRenderer2D() const { return Renderer2D.get(); }
    CPhysics2D* GetPhysics2D() const { return Physics2D.get(); }
    CImGUI* GetImGUI() const { return ImGUI.get(); }
    CScene* GetScene() const { return Scene.get(); }
    CScriptModule* GetScriptModule() const { return ScriptModule.get(); }

    // Type
    IEngineModule* GetModule(const std::string&) const;
    // Type
    template<class T>
    T* GetModule() const
    {
        static_assert(std::is_base_of<IEngineModule, T>::value, "Must be base of IEngineModule");
        return dynamic_cast<T*>( GetModule(T::GetTypeStatic()) );
    }
private:
    uint32_t Ticks = 0u;
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
    std::unique_ptr<CEngineUpdater> EngineUpdater;
    std::unique_ptr<CUserUpdater> UserUpdater;
    std::unique_ptr<CResources> Resources;
    std::unique_ptr<CAudioHandler> AudioHandler;
    std::unique_ptr<CDrawer2D> Drawer2D;
    std::unique_ptr<CRenderer2D> Renderer2D;
    std::unique_ptr<CPhysics2D> Physics2D;
    std::unique_ptr<CImGUI> ImGUI;
    std::unique_ptr<CScene> Scene;
    std::unique_ptr<CScriptModule> ScriptModule;
    std::unique_ptr<CPerformanceCounter> Counter;
};

