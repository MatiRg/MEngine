#include "Engine/Full.hpp" 

class CPlayerManager;
class CLevel1;
class CLevel2;

// Container Names must be unique
static constexpr char LVL1_CONTAINER[] = "Level1";
static constexpr char LVL2_CONTAINER[] = "Level2";

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
    std::unique_ptr<CPlayerManager> PlayerManager;
    std::unique_ptr<CLevel1> Level1;
    std::unique_ptr<CLevel2> Level2;
};

class CPlayerManager: public IUpdatable
{
public:
    // Setup Name for Updatable - should be unique but is not needed if there is no need for it
    CPlayerManager(CMainApp* aApp): IUpdatable("PlayerManager"), App(aApp) {}

    // Setup reflection - required
    UPDATABLE(CPlayerManager)

    // Called Only Once
    void OnInit() override 
    {
        Msg = "Global Updatable";
    }

    // Can be called many times, one or zero
    void OnUpdate(const float) override
    {
        // Get Input Engine Module
        IInput* Input = App->GetInput();
        // Check if key is down
        if (Input->IsKeyDown(EKey::A))
        {
            // Change container
            App->GetUserUpdater()->SetContainer(LVL1_CONTAINER);
        }
        // Check if key is down
        if (Input->IsKeyDown(EKey::D))
        {
            // Change container
            App->GetUserUpdater()->SetContainer(LVL2_CONTAINER);
        }
    }

    // Called Once after update part
    void OnLateUpdate(const float) override
    {
        // Debug information send
        LOG(ESeverity::Debug) << Msg << "\n";
    }
private:
    CMainApp* App;
    std::string Msg;
};

class CLevel1: public IUpdatable
{
public:
    CLevel1(): IUpdatable("Level 1") {}

    UPDATABLE(CLevel1)

    // Called everytime when container it holds is set active. For global ther are called only once just before main loop
    void OnEnter() override 
    {
        Msg = "Level 1 - " + std::to_string(Counter++);
    }

    void OnLateUpdate(const float) override
    {
        LOG(ESeverity::Debug) << Msg << "\n";
    }
private:
    int Counter = 0;
    std::string Msg;
};

class CLevel2: public IUpdatable
{
public:
    CLevel2(): IUpdatable("Level 2") {}

    UPDATABLE(CLevel2)

    void OnLateUpdate(const float) override
    {
        LOG(ESeverity::Debug) << GetName() << "\n";
    }
};

bool CMainApp::Init()
{
    // Create Updatables
    PlayerManager = std::make_unique<CPlayerManager>(this);
    Level1 = std::make_unique<CLevel1>();
    Level2 = std::make_unique<CLevel2>();

    // Add global Updtable
    GetUserUpdater()->AddUpdatable(PlayerManager.get());

    // Create Container
    GetUserUpdater()->AddContainer(LVL1_CONTAINER);
    // Add Updatble to Container
    GetUserUpdater()->AddUpdatable(LVL1_CONTAINER, Level1.get());

    // Create Container
    GetUserUpdater()->AddContainer(LVL2_CONTAINER);
    // Add Updatble to Container
    GetUserUpdater()->AddUpdatable(LVL2_CONTAINER, Level2.get());

    // Example Add resource Path
    // Check if Directory exist
    if (!GetSystem()->DirectoryExist("Data"))
    {
        // Log information
        LOG(ESeverity::Fatal) << "No 'Data' Directory\n";
        return false;
    }
    // Path and if you want include subdirectories
    GetResources()->AddPath("Data", true);
    return true;
}

void CMainApp::OnInit()
{
    GetWindow()->SetCaption("Updatables!"); // Set Window Caption

    // Set starting container
    GetUserUpdater()->SetContainer(LVL1_CONTAINER);
}

void CMainApp::Exit()
{
    // Destroy Updatables
    Level2.reset();
    Level1.reset();
    PlayerManager.reset();
}

ENTRY_POINT( CMainApp, "log_helloworld.txt" )