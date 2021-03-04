# Documentation
Documentation is Work in Progress.
## File Formats
See [FORMATS.md](FORMATS.md)
## Creating Empty App
Creating Empty Application is simple:
```C++
#include "Engine/Full.hpp" // Includes all Engine Headers for easier use

class CMainApp: public CApp
{
public:
    CMainApp() = default;
    ~CMainApp() = default;
protected:
    bool Init() override
    {
        // Adding Resources Search Path, Creating Updatable Containers with CUserUpdater
        // Create Engine Modules
        // Create Game Updatables add add them to given Container or to global container
        return true;
    }

    void OnInit() override
    {
        // Game Init, after Engine Modules & Game Updatables are created
        // You can set here stating container with CUserUpdater
        GetWindow()->SetCaption("Hello!"); // Set Window Caption
    }

    void Exit() override
    {
        // Destroy here Game Updatables
    }
};
// Define Entry Point for Applciation. 
// Where `CMainApp` is App class name and `log_helloworld.txt` is file that messages will be saved by logger.
ENTRY_POINT( CMainApp, "log_helloworld.txt" )
```
## Creating and Managing Updatables
Updatables are something akin to State.
Updatables can be grouped into Global Container and Individual Containers.
This example also showcases hot to add Resource Search Path, use IInput Engine Module and use loging.
You can also see how to use ISystem to check if directory exist.
```C++
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
```
## Creating and Managing User Engine Modules
Engine Module is extension of IUpdatable.It has extended Init() and Exit() phases and works more tightly with Engine.
Engine Modules are added to CEngineUpdater.Also showcases how to use debug UI system: CImGUI.
```C++
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
```
## Creating Camera and World
This sample showcases how to use World-Scene system.We are creating Free Fly Camera, Simple Mesh and Directional Light.
```C++
// CMainApp code ...
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
        // Load Model As Entity Graph
        // Material is loaded for Model based on it is name, in this case it will be: "backpack.mat"
        CEntity* Entity = World->CreateModel<CEntity>("backpack.obj");
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
};
// Rest of the Code ...
```
## Working with Scene Audio
In this sample we will add Sound Playback - 3D Music and simple 2D sound.
Example how to stream audio file and How to use CResources.
```C++
// Rest of the Code ...
class CLevel1: public IUpdatable
{
public:
    // ...
    void OnEnter() override 
    {
        // ...
        // Create Audio Listener so we will hear the audio output
        // Create it at camera object so it will move with camera
        CameraObject->CreateComponent<CAudioListener>();

        // Create 2D Sound Source at Camera Object
        Sound2D = CameraObject->CreateComponent<CAudioSource>();
        // Play only Once
        Sound2D->SetLooped(false);
        // Set Volume from 0 to 1
        Sound2D->SetVolume(0.75f);
        // Get Resource Module
        CResources* Resources = App->GetResources();
        // Example how to load resource
        // Load Sound Data, resource is loaded only once
        // If resource is loaded it will return pointer to it without loading it from disk
        ISoundData* SampleWav = Resources->CreateResource<ISoundData>("mouseclick.wav");
        // Set new sound data
        Sound2D->SetData(SampleWav);

        // Create 3D audio source at Mesh Entity
        Sound3D = Entity->CreateComponent<CAudioSource>();
        // Loop Sound Effect
        Sound3D->SetLooped(true);
        // Set Volume from 0 to 1
        Sound3D->SetVolume(0.75f);
        // The attenuation is a multiplicative factor. The greater the attenuation, the less it 
        // will be heard when the sound moves away from the listener
        Sound3D->SetAttenuation( 4.0f );
        // Example how to load resource with additional resource variables
        // Create Resource Variable Map
        ResourceCreateMap ResVar;
        // Set IsStream varaible indicating that this sound will be streamed from Disk, use it for music
        ResVar[RESOURCES_VAR_IS_STREAM] = true;
        // Load Data
        ISoundData* SampleOgg = Resources->CreateResource<ISoundData>("Debussy - Arabesque No. 1 (Ciccolini).ogg", ResVar);
        // Set new sound data
        Sound3D->SetData(SampleOgg);
        // Play Music
        Sound3D->Play();
    }

    void OnUpdate(const float TimeStep) override
    {
        // ...
        // Play 2D sound
        if (Input->IsMouseKeyDown(EMouseKey::LButton))
        {
            Sound2D->Play();
        }
    }
private:
    // ...
    // 2D Sound Source
    CAudioSource* Sound2D = nullptr;
    // 3D Sound Source
    CAudioSource* Sound3D = nullptr;
};
// Rest of the Code ...
```
## Creating custom Entities
In this example I will show you how to create custom Entity and register it.
It also showcase how to use Basic 2D Rendering.
```C++
class CGameManager: public CEntity
{
public:
    // Constructor must always have this parameters
    CGameManager(CEngine* aEngine): CEntity(aEngine) {}

    // Must be used to implement reflection
    ENTITY(CGameManager)

    // Use it load variables associated with this class 
    bool OnLoad(CXMLElement* Root) override
    {
        // Load Variable
		Score = XML::LoadFloat( Root, "Score", 0.0f);
        // Load Variable
		TimeSpawn = XML::LoadFloat( Root, "TimeSpawn", 0.0f);
        // Load Variable
		TopScore = XML::LoadInt( Root, "TopScore", 0);
        // Return true on success
        return true;
    }

    // Use it save variables associated with this class 
    bool OnSave(CXMLElement* Root) override
    {
        // Save Variable
        XML::SaveFloat( Root, "Score", Score);
        // Save Variable
        XML::SaveFloat( Root, "TimeSpawn", TimeSpawn);
        // Save Variable
        XML::SaveInt( Root, "TopScore", TopScore);
        // Return true on success
        return true;
    }

    // Called when Entity is created and added to World structure
    void OnCreate() override
    {
        // Load font resource
        Font = Engine->GetResources()->CreateResource<IFont>("Default.ttf");
    }

    // When Entity is Updated
    void OnUpdate(const float DT) override
    {
        // Get Entity from World with given Name and cast it to given class
        auto PlayerObject = World->GetChild<CPlayer>( PLAYER_NAME );
        // Update Logic
        if( PlayerObject && PlayerObject->IsGameOver() )
        {
            if( static_cast<float>(Score) > TopScore )
            {
                TopScore = static_cast<int>(Math::Floor(Score));
            }
            // Return if Game is already over
            return;
        }
        // Update rest of variables
        Score += DT;
        TimeSpawn += DT;
        if( TimeSpawn > TimeToSpawn )
        {
            // Create another custom Entity
            World->CreateChild<CEnemy>();
            TimeSpawn -= TimeToSpawn;
        }
    }

    // Use it to send render requests
    void OnRender() override
    {
        // Get 2D Drawer
        CDrawer2D* Drawer = Engine->GetDrawer2D();
        // Draw Text ...
        Drawer->DrawText( Font, "Press 'Escape' to Return to Menu", Vector2(32.0f, 16.0f), FONT_SIZE, Color::WHITE, 1000.0f );
        // Draw Text ...
        Drawer->DrawText( Font, "Score: "+std::to_string(static_cast<int>(Math::Floor(Score))),
            Vector2(32.0f, 40.0f), FONT_SIZE, Color::WHITE, 1000.0f );
        // Draw Text ...
        Drawer->DrawText( Font, "Top Score: "+std::to_string(TopScore), Vector2(32.0f, 64.0f), FONT_SIZE, Color::WHITE, 1000.0f );
    }
private:
    IFont* Font = nullptr;
    float Score = 0.0f;
    const float TimeToSpawn = 0.1f;
    float TimeSpawn = 0.0f;
    int TopScore = 0;
};
// Other Code ...
void CLevel1::OnInit()
{
    // ...
    // Register Entity before first usage
    App->GetScene()->RegisterEntity<CGameManager>();
    // ...
}
// Other Code ...
```
## Creating custom Components
In this example I will show you how to create custom Components and register it.
```C++
class CFallComponent: public IComponent
{
public:
    // Constructor must always have this parameters
    CFallComponent(CEngine* aEngine): IComponent(aEngine) {}

    // Must be used to implement reflection
    COMPONENT(CFallComponent)

    // Load Component Data similar to CEntity Loading
    bool OnLoad(CXMLElement* Root) override
    {
		Speed = XML::LoadFloat( Root, "Speed", 0.0f );
        return true;
    }

    // Save Component Data similar to CEntity Saving
    bool OnSave(CXMLElement* Root) override
    {
        XML::SaveFloat( Root, "Speed", Speed);
        return true;
    }

    // Update Logic
    void OnUpdate(const float DT) override
    {
        // Get Transform
        auto& Transform = GetOwner()->GetTransform();

        Vector2 Position = Transform.GetPosition2D();
        Position -= Vector2::UP*Speed*DT;

        Transform.SetPosition2D( Position );
    }

    // Data
    void SetSpeed(const float aSpeed) { Speed = aSpeed; }
    float GetSpeed() const { return Speed; }
private:
    float Speed = 0.0f;
};
// Other Code ...
void CLevel1::OnInit()
{
    // ...
    // Register Component before first usage
    App->GetScene()->RegisterComponent<CFallComponent>();
    // ...
}
// Other Code ...
```
## Working with Post Effects
Post Effects are applied after Object are rendered in Frame Buffer.
They Are applied in given order.As with shaders they are written in GLSL.
```C++
// ...
class CLevel1: public IUpdatable
{
public:
    // ...
    void OnInit() override
    {
        // Create Effect only once
        // Effect is created and added to stack with order given in second argument
        // CRenderer3D is ownership of the effect
        // Best option is to create Effects in one place
        // Effects are applied in ascending order
        MyEffect = App->GetRenderer3D()->CreatePostEffect("ChromaticAberration.shader", 10);
        // Set Effect Parameter
        MyEffect->SetFloat("Strength", 0.75f);
    }
    // ...
    void OnUpdate(const float TimeStep) override
    {
        // ...
        // Enable/Disable Post Effect
        if (Input->IsMouseKeyDown(EMouseKey::LButton))
        {
            // Toggle It
            MyEffect->SetEnabled( !MyEffect->IsEnabled() );
        }
    }
    // ...
private:
    // ...
    // For easier use
    CPostEffect* MyEffect = nullptr;
};
// ...
```
And Shader Code.
```glsl
// Based on ChromaticAberration.fx from ReShade
// First Includes 
#include "Locations.inc" // Vertex Input
#include "Uniforms.inc" // Typical Uniforms
#include "PostEffect.inc" // Post Process Uniforms

// Uniforms
// -10 to 10
uniform vec2 Shift = {2.5, -0.5};
// 0 to 1
uniform float Strength = 0.5;

// Vertex Shader
#ifdef VS

out vec2 TexCoords;

void main()
{
	TexCoords = TexCoords1;
	gl_Position = vec4(Position, 1.0);
}

#endif

// Fragment Shader
#ifdef FS

in vec2 TexCoords;

out vec4 Color;

void main()
{	
    vec3 InputColor = texture(BackBufferColor, TexCoords).rgb;
    vec3 Tmp = texture(BackBufferColor, TexCoords).rgb;

	// Sample the color components
	Tmp.r = texture(BackBufferColor, TexCoords + (PixelSize * Shift)).r;
	Tmp.g = InputColor.g;
	Tmp.b = texture(BackBufferColor, TexCoords - (PixelSize * Shift)).b;
	// Adjust the strength of the effect
	Tmp = mix(InputColor, Tmp, Strength);

    Color = vec4(Tmp, 1.0);
}

#endif
```
TO DO
