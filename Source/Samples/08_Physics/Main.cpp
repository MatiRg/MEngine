#include "Engine/Full.hpp" 

class CLevel;

// Container Names must be unique
static constexpr char LVL_CONTAINER[] = "Level";

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
    std::unique_ptr<CLevel> Level;
};

// Create Custom Entity Class

class CGameObject: public CEntity
{
public:
    //! Proper Constructor
    CGameObject(CEngine* aEngine):
        CEntity(aEngine)
    {
    }

    virtual ~CGameObject()
    {
    }

    //! Use for Reflection Basic - Must be Used
    ENTITY(CGameObject)

    void OnGUI() override
    {
        CImGUI* UI = Engine->GetImGUI();
        UI->Begin("Collision Info");
        if (HasCollision)
        {
            UI->Text(CollisionInfo);
        }
        else
        {
            UI->Text("No Collision!");
        }
        UI->End();
    }

    void OnCollisionEnter(const SEntityCollision3D& Collision) override
    {
        HasCollision = true;
        CollisionInfo = "ID: " + std::to_string(Collision.Entity->GetID()) + " -> " + Collision.Entity->GetName();
    }

    void OnCollisionStay(const SEntityCollision3D& Collision) override
    {
        CollisionInfo = "ID: " + std::to_string(Collision.Entity->GetID()) + " -> " + Collision.Entity->GetName();
    }

    void OnCollisionLeave(const SEntityCollision3D& Collision) override
    {
        CollisionInfo = "ID: " + std::to_string(Collision.Entity->GetID()) + " -> " + Collision.Entity->GetName();
        HasCollision = false;
    }
private:
    bool HasCollision = false;
    std::string CollisionInfo;
};

//

class CLevel : public IUpdatable
{
public:
    CLevel(CMainApp* aApp): IUpdatable("Level"), App(aApp) {}

    UPDATABLE(CLevel)

    void OnInit() override
    {
        // Register Custom Entity Class
        App->GetScene()->RegisterEntityFactory<CGameObject>();
    }

    void OnEnter() override 
    {
        // Set Mouse Mode for Free Camera
        App->GetInput()->SetMouseMode(EMouseMode::Relative);

        // Create World
        World = std::make_unique<CWorld>( App->GetEngine() );
        // Create Physics World
        CPhysicsWorld3D* PhysicsWorld = World->CreateComponent<CPhysicsWorld3D>();
        PhysicsWorld->SetGravity( {0.0f, -10.0f, 0.0f} );

        // Create Sky Dome
        CEntity* SkyDome = World->CreateModel<CEntity>("sky2.dae");
        // Make it Big
        SkyDome->GetTransform().SetScale(Vector3(500.0f));
        //
        // Create Sun Entity
        CEntity* Sun = World->CreateChild<CEntity>();
        // Create Light Component
        CLightComponent* LightSun = Sun->CreateComponent<CLightComponent>();
        // Set Type
        LightSun->SetLightType(ELightType::Direction);
        // Set Color - From Kelvin Temperature
        LightSun->SetTemperature(5500.f);
        // Set Proper Rotation
        Sun->GetTransform().SetRotation(Quaternion(-30.0f, 0.0f, 0.0f));
       
        // Create Camera Object
        CameraObject = World->CreateChild<CEntity>();
        // Create Camera Component Required
        CameraComponent = CameraObject->CreateComponent<CCamera>();
        // Set Aspect Ratio
        CameraComponent->SetAspect(App->GetWindow()->GetAspectRatio());
        // Set FOV
        CameraComponent->SetFOV(Fov);
        // Set Near ...
        CameraComponent->SetNearClipPlane(0.3f);
        // ... and Far Plane
        CameraComponent->SetFarClipPlane(1000.0f);

        // Create Physics Box, Using our Custom Entity Class
        Box1 = World->CreateModel<CGameObject>("cube.dae");
        // Set Name
        Box1->SetName("FallingBox");
        // Change Position
        Box1->GetTransform().SetPosition( {0.0f, 0.0f, -10.0f} );
        // First Create Collision Shape
        Box1->CreateComponent<CBoxCollider3D>();
        // ... Then RigidBody
        CRigidBody3D* Body1 = Box1->CreateComponent<CRigidBody3D>();
        // Change Gravity for Body1
        Body1->SetGravity( Vector3::DOWN*3.33f );

        // Create Ground
        CEntity* Ground = World->CreateModel<CEntity>("cube.dae");
        // Set Name
        Ground->SetName("Ground");
        // Change Position
        Ground->GetTransform().SetPosition({ 0.0f, -5.0f, -10.0f });
        // Change Rotation
        Ground->GetTransform().SetRotation(Quaternion(33.0f, 33.0f, 0.0f));
        // First Create Collision Shape
        Ground->CreateComponent<CBoxCollider3D>();
        // ... Then RigidBody
        CRigidBody3D* GroundBody = Ground->CreateComponent<CRigidBody3D>();
        // Make it Static Body
        GroundBody->SetBodyType(ERigidBodyType3D::Static);
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

        // Add Force
        if (Input->IsMouseKeyDown(EMouseKey::LButton))
        {
            Box1->GetComponent<CRigidBody3D>()->AddForce(Box1->GetTransform().GetForward()*30.0f);
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
    // Physics Box 1
    CGameObject* Box1 = nullptr;
};

bool CMainApp::Init()
{
    Level = std::make_unique<CLevel>(this);

    GetUserUpdater()->AddContainer(LVL_CONTAINER);
    GetUserUpdater()->AddUpdatable(LVL_CONTAINER, Level.get());

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
    GetWindow()->SetCaption("Physics!"); 
    GetUserUpdater()->SetContainer(LVL_CONTAINER);
}

void CMainApp::Exit()
{
    Level.reset();
}

ENTRY_POINT( CMainApp, "log_helloworld.txt" )