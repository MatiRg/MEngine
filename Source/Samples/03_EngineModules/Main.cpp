#include "Engine/Full.hpp" 

class CMyEngineModule;
class CPlayerManager;

class CMainApp: public CApp
{
public:
    CMainApp() = default;
    ~CMainApp() = default;
protected:
    bool Init() override;
    void OnInit() override;
    void Exit() override;
private:
    std::unique_ptr<CMyEngineModule> MyEngineModule;
    //
    std::unique_ptr<CPlayerManager> PlayerManager;
};

class CMyEngineModule: public IEngineModule
{
public:
    // Setup Name for Engine Module - should be unique but is not needed if there is no need for it
    CMyEngineModule(CMainApp* aApp): IEngineModule("My Engine Module"), App(aApp) {}

    // Setup reflection - required
    ENGINE_MODULE(CMyEngineModule)


    // Initialize Engine Module - SEngineParams is struct holding Engien Creation Params
    bool Init(const SEngineParams&) override
    { 
        OurValue = 3.0f;

        LOG(ESeverity::Debug) << GetType() << " Initialized\n";
        return true; 
    }

    // Shutdown Engine Module
    void Exit() override
    {
        LOG(ESeverity::Debug) << GetType() << " Shutdown\n";
    }

    // Used for ImGUI debug system
    void OnGUI() override
    {
        CImGUI* UI = App->GetImGUI();
        // Start Debug Window
        UI->Begin("Me Enigne Module");
        // Draw Text
        UI->Text("Modify Module Params");
        // Horizontal Line
        UI->Separator();
        // Float Edit Box
        UI->InputFloat("Value", OurValue);
        // End Debug Window
        UI->End();
    }

    // Test Method
    float GetSomething() const
    {
        return OurValue;
    }
private:
    CMainApp* App;
    float OurValue = 0.0f;
};

class CPlayerManager : public IUpdatable
{
public:
    // Setup Name for Updatable - should be unique but is not needed if there is no need for it
    CPlayerManager(CMainApp* aApp) : IUpdatable("PlayerManager"), App(aApp) {}

    // Setup reflection - required
    UPDATABLE(CPlayerManager)

    // Can be called many times, one or zero
    void OnUpdate(const float) override
    {
        // Get Input Engine Module
        IInput* Input = App->GetInput();
        // Check if key is down
        if (Input->IsKeyDown(EKey::D))
        {
            // Get Our Engine Module by Type
            CMyEngineModule* Module = App->GetEngineUpdater()->GetEngineModule<CMyEngineModule>();
            LOG(ESeverity::Debug) << "Value from our Engine Module: " << Module->GetSomething() << "\n";
        }
    }
private:
    CMainApp* App;
    std::string Msg;
};

bool CMainApp::Init()
{
    // Create Updatables
    MyEngineModule = std::make_unique<CMyEngineModule>(this);

    // Add Engine Modules
    GetEngineUpdater()->AddEngineModule(MyEngineModule.get());

    // Here Create and Add Updatables
    // ...
    PlayerManager = std::make_unique<CPlayerManager>(this);
    GetUserUpdater()->AddUpdatable(PlayerManager.get());

    return true;
}

void CMainApp::OnInit()
{
    GetWindow()->SetCaption("Engine Modules!"); // Set Window Caption
}

void CMainApp::Exit()
{
    // Destroy Updatables
    // ...
    PlayerManager.reset();
    // Destroy Engine Modules after updatables
    MyEngineModule.reset();
}

ENTRY_POINT( CMainApp, "log_helloworld.txt" )