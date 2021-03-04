#include "Engine/Full.hpp" 

class CLevel1;

// Container Names must be unique
static constexpr char LVL1_CONTAINER[] = "Level1";

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
    std::unique_ptr<CLevel1> Level1;
};

class CForwardDrawer: public IComponent
{
public:
    CForwardDrawer(CEngine* aEngine) :
        IComponent(aEngine)
    {
    }

    COMPONENT(CForwardDrawer)

    void OnCreate() override
    {
        Drawer = Engine->GetDebugDrawer();
        Transform = &GetOwner()->GetTransform();
    }

    void OnRender() override
    {
        Drawer->AddLine(Transform->GetWorldPosition(), Transform->GetWorldPosition() + Transform->GetForward() * Length, Color::BLUE);
    }
public:
    CDebugDrawer* Drawer = nullptr;
    CTransform* Transform = nullptr;
    float Length = 1.0f;
};


class CLevel1: public IUpdatable
{
public:
    CLevel1(CMainApp* aApp): IUpdatable("Level 1"), App(aApp) {}

    UPDATABLE(CLevel1)

    void OnEnter() override 
    {
        // Set Mouse Mode for Free Camera
        App->GetInput()->SetMouseMode(EMouseMode::Relative);
        // Create World
        World = std::make_unique<CWorld>( App->GetEngine() );
        //
        // Create Sun Entity
        CEntity* Sun = World->CreateChild<CEntity>();
        // Create Light Component
        CLightComponent* LightSun = Sun->CreateComponent<CLightComponent>();
        // Set Type
        LightSun->SetLightType(ELightType::Direction);
        // Set Color - From Kelvin Temperature
        LightSun->SetTemperature(2500.f);
        // Set Proper Rotation
        Sun->GetTransform().SetRotation(Quaternion(-30.0f, 0.0f, 0.0f));
        //
        Entity1 = World->CreateModel<CEntity>("cube.dae");
        Entity1->CreateComponent<CForwardDrawer>()->Length = 2.5f;
        Entity1->GetTransform().SetWorldPosition({4.0f, 3.0f, -5.0f});
        //
        Entity2 = World->CreateModel<CEntity>("cube.dae");
        Entity2->CreateComponent<CForwardDrawer>()->Length = 2.5f;
        Entity2->GetTransform().SetWorldPosition({ -2.0f, -1.0f, -2.0f });
        //
        // Create Camera Object
        CameraObject = World->CreateChild<CEntity>();
        // Create Camera COmponent Required
        CameraComponent = CameraObject->CreateComponent<CCamera>();
        // Set Aspect Ratio
        CameraComponent->SetAspect(App->GetWindow()->GetAspectRatio());
        // Set FOV
        CameraComponent->SetFOV(Fov);
        // Set Near ...
        CameraComponent->SetNearClipPlane(0.3f);
        // ... and Far Plane
        CameraComponent->SetFarClipPlane(1000.0f);
    }

    void OnUpdate(const float TimeStep) override
    {
        IInput* Input = App->GetInput();
        // Quit App after "Esc" key is down
        if (Input->IsKeyDown(EKey::Escape))
        {
            App->Quit();
        }
        // Move Camera
        if (Input->IsKeyPressed(EKey::W))
        {
            // Translate object by ... with respect to Time Step
            CameraObject->GetTransform().Translate(Vector3::FORWARD * CameraSpeed * TimeStep);
        }
        if (Input->IsKeyPressed(EKey::S))
        {
            // Translate object by ... with respect to Time Step
            CameraObject->GetTransform().Translate(Vector3::BACK * CameraSpeed * TimeStep);
        }
        if (Input->IsKeyPressed(EKey::A))
        {
            // Translate object by ... with respect to Time Step
            CameraObject->GetTransform().Translate(Vector3::LEFT * CameraSpeed * TimeStep);
        }
        if (Input->IsKeyPressed(EKey::D))
        {
            // Translate object by ... with respect to Time Step
            CameraObject->GetTransform().Translate(Vector3::RIGHT * CameraSpeed * TimeStep);
        }
        if (Input->IsKeyPressed(EKey::Q))
        {
            // Translate object by ... with respect to Time Step
            CameraObject->GetTransform().Translate(Vector3::UP * CameraSpeed * TimeStep);
        }
        if (Input->IsKeyPressed(EKey::E))
        {
            // Translate object by ... with respect to Time Step
            CameraObject->GetTransform().Translate(Vector3::DOWN * CameraSpeed * TimeStep);
        }

        // Update Camera Rotation
        constexpr float SensitivityRel = 0.05f;
        Yaw += Input->GetRelativeMouseMotion().x * SensitivityRel;
        Pitch += Input->GetRelativeMouseMotion().y * SensitivityRel;

        // Set New Rotation in Euler Angles
        CameraObject->GetTransform().SetRotation(Quaternion(Pitch, Yaw, 0.0f));

        // Update FOV
        ScrollSpeed = Input->GetMouseWheel().y;

        Fov -= ScrollSpeed;
        if (Fov <= 1.0f)
            Fov = 1.0f;
        if (Fov >= 45.0f)
            Fov = 45.0f;
        ScrollSpeed = 0.0f;

        // Set Updated FOV
        CameraComponent->SetFOV(Fov);

        // LookAt Test
        if (Input->IsKeyDown(EKey::P))
        {
            Entity1->GetTransform().LookAt(Entity2->GetTransform());
        }
        // RotateAround Test
        if (Input->IsKeyDown(EKey::R))
        {
            RotateAround = !RotateAround;
        }
        //
        if (RotateAround)
        {
            Entity1->GetTransform().RotateAround(Entity2->GetTransform(), Vector3::UP, 33.0f * TimeStep);
        }
    }

    void OnLeave() override
    {
        // Destroy World - just befor exiting Level 1 Container
        World.reset();
        // Reset Mouse Mode
        App->GetInput()->SetMouseMode(EMouseMode::Normal);
    }
private:
    CMainApp* App;
    // Camera Control Variables
    float Time = 0.0f;
    float CameraSpeed = 10.0f;
    float Yaw = -90.0f;
    float Pitch = 0.0f;
    float Fov = 45.0f;
    float ScrollSpeed = 0.0f;
    // World Smart Pointer
    std::unique_ptr<CWorld> World;
    // For easier use
    CEntity* CameraObject = nullptr;
    // For easier use
    CCamera* CameraComponent = nullptr;
    //
    CEntity* Entity1 = nullptr;
    CEntity* Entity2 = nullptr;
    bool RotateAround = false;
};

bool CMainApp::Init()
{
    Level1 = std::make_unique<CLevel1>(this);

    GetUserUpdater()->AddContainer(LVL1_CONTAINER);
    GetUserUpdater()->AddUpdatable(LVL1_CONTAINER, Level1.get());

    if (!GetSystem()->DirectoryExist("Data"))
    {
        // Log information
        LOG(ESeverity::Fatal) << "No 'Data' Directory\n";
        return false;
    }
    GetResources()->AddPath("Data", true);

    return true;
}

void CMainApp::OnInit()
{
    GetScene()->RegisterComponent<CForwardDrawer>();
    //
    GetWindow()->SetCaption("Math Sandbox!"); 
    GetUserUpdater()->SetContainer(LVL1_CONTAINER);
}

void CMainApp::Exit()
{
    Level1.reset();
}

ENTRY_POINT( CMainApp, "log_helloworld.txt" )