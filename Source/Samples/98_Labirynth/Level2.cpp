#include "Level2.hpp"
#include "MainApp.hpp"
#include "Globals.hpp"

//

CGameObject::CGameObject(CEngine* aEngine) :
    CEntity(aEngine)
{
}

CGameObject::~CGameObject()
{
}

void CGameObject::OnGUI()
{
}

void CGameObject::OnCollisionEnter(const SEntityCollision3D& Collision)
{    
}

void CGameObject::OnCollisionStay(const SEntityCollision3D& Collision)
{
}

void CGameObject::OnCollisionLeave(const SEntityCollision3D& Collision)
{
}

//

CLevel2::CLevel2(CMainApp* aApp) :
    IUpdatable("Level2"),
    App(aApp)
{
}

CLevel2::~CLevel2()
{
}

void CLevel2::LoadMap()
{
    std::string Path;
    if( !App->GetResources()->FindPath("Labirynt.xml", Path) )
    {
        return;
    }

    CXMLDocument Doc;
    if (!Doc.Load(Path))
    {
        return;
    }

    CXMLElement* Root = Doc.GetElement("map");
    if (!Root)
    {
        return;
    }
    Width = Root->GetIntAttribute("W", 0);
    Height = Root->GetIntAttribute("H", 0);

    XMLElementArray Rows = Root->GetElements("row");
    for (const auto& i : Rows)
    {
        int Row = i->GetIntAttribute("id", 0);
        XMLElementArray Cols = i->GetElements("col");
        for (const auto& j : Cols)
        {
            int Col = j->GetIntAttribute("id", 0);
            int Value = j->GetInt(0);
            //CGameObject* Object = nullptr;
            //if (Value)
            //{
            CGameObject* Object = World->CreateModel<CGameObject>( "cube.dae", Materials[Math::Random(0, 4)].get() );
            //}
            //else
            //{
            //    Object = World->CreateModel<CGameObject>("cube.dae");
            //}
            Object->SetValue(Value);
            Object->GetTransform().SetPosition({ static_cast<float>(Row), static_cast<float>(Value), static_cast<float>(Col)});
        }
    }

    CXMLElement* StartXRoot = Doc.GetElement("start_x");
    if (!StartXRoot)
    {
        return;
    }
    StartX = StartXRoot->GetInt(0);
    //
    CXMLElement* StartYRoot = Doc.GetElement("start_y");
    if (!StartYRoot)
    {
        return;
    }
    StartY = StartYRoot->GetInt(0);
}

void CLevel2::OnInit()
{
    App->GetScene()->RegisterEntityFactory<CGameObject>();
    //
    CResources* Resources = App->GetResources();
    //
    IShader* ShaderCube = Resources->CreateResource<IShader>("TextureLSDEffects.shader");
    ITexture2D* TextureCube = Resources->CreateResource<ITexture2D>("wall.jpg");
    //
    Materials[0] = std::make_unique<CMaterial>();
    Materials[0]->SetShader(ShaderCube );
    Materials[0]->SetTexture("Diffuse", TextureCube);
    Materials[0]->SetInt("SwitchVar", 0);
    //
    Materials[1] = std::make_unique<CMaterial>();
    Materials[1]->SetShader(ShaderCube);
    Materials[1]->SetTexture("Diffuse", TextureCube);
    Materials[1]->SetInt("SwitchVar", 1);
    //
    Materials[2] = std::make_unique<CMaterial>();
    Materials[2]->SetShader(ShaderCube);
    Materials[2]->SetTexture("Diffuse", TextureCube);
    Materials[2]->SetInt("SwitchVar", 2);
    //
    Materials[3] = std::make_unique<CMaterial>();
    Materials[3]->SetShader(ShaderCube);
    Materials[3]->SetTexture("Diffuse", TextureCube);
    Materials[3]->SetInt("SwitchVar", 3);
    //
    Materials[4] = std::make_unique<CMaterial>();
    Materials[4]->SetShader(ShaderCube);
    Materials[4]->SetTexture("Diffuse", TextureCube);
    Materials[4]->SetInt("SwitchVar", 4);
    //
    BlurEffect = App->GetRenderer3D()->CreatePostEffect( "ScreenBlurBox.shader", 10 );
    if( BlurEffect )
    {
        BlurEffect->SetInt("Samples", 1);
    }
}

namespace
{
    void PrintModel(CMeshNode* Node, int Level)
    {
        if (Node)
        {
            std::string Margin(Level, ' ');
            LOG(ESeverity::Info) << Margin << "Node: " << Node->GetName() << "\n";

            for (const auto& i : Node->GetMeshes())
            {
                LOG(ESeverity::Info) << Margin << "+Mesh: " << i->GetName() << "\n";
            }

            for (const auto& i : Node->GetChildren())
            {
                PrintModel(i, Level+1);
            }
        }
    }
}

void CLevel2::OnEnter()
{
    App->GetInput()->SetMouseMode(EMouseMode::Relative);
    //
    World = std::make_unique<CWorld>(App->GetEngine());

    // Create Sky Dome
    CEntity* SkyDome = World->CreateModel<CEntity>("sky2.dae");
    // Make it Big
    SkyDome->GetTransform().SetScale(Vector3(500.0f));

    CEntity* Sun = World->CreateChild<CEntity>();
    //
    CLightComponent* LightSun = Sun->CreateComponent<CLightComponent>();
    LightSun->SetLightType( ELightType::Direction );
    LightSun->SetTemperature( 5500.f );
    //
    Sun->GetTransform().SetRotation( Quaternion(-30.0f, 0.0f, 0.0f) );

    //CEntity* Entity = World->CreateModel<CEntity>("cube.dae", Material.get());

    CameraObject = World->CreateChild<CEntity>();
    //
    CameraComponent = CameraObject->CreateComponent<CCamera>();
    CameraComponent->SetAspect(App->GetWindow()->GetAspectRatio());
    CameraComponent->SetFOV(Fov);
    CameraComponent->SetNearClipPlane(0.3f);
    CameraComponent->SetFarClipPlane(1000.0f);

    LoadMap();
}

void CLevel2::OnUpdate(const float TimeStep)
{
    Time += TimeStep;
    //
    IInput* Input = App->GetInput();
    //
    if (Input->IsKeyDown(EKey::Escape))
    {
        App->Quit();
    }
    //
    if (Input->IsKeyPressed(EKey::W))
    {
        CameraObject->GetTransform().Translate(Vector3::FORWARD * CameraSpeed * TimeStep);
    }
    if (Input->IsKeyPressed(EKey::S))
    {
        CameraObject->GetTransform().Translate(Vector3::BACK * CameraSpeed * TimeStep);
    }
    if (Input->IsKeyPressed(EKey::A))
    {
        CameraObject->GetTransform().Translate(Vector3::LEFT * CameraSpeed * TimeStep);
    }
    if (Input->IsKeyPressed(EKey::D))
    {
        CameraObject->GetTransform().Translate(Vector3::RIGHT * CameraSpeed * TimeStep);
    }
    if (Input->IsKeyPressed(EKey::Q))
    {
        CameraObject->GetTransform().Translate(Vector3::UP * CameraSpeed * TimeStep);
    }
    if (Input->IsKeyPressed(EKey::E))
    {
        CameraObject->GetTransform().Translate(Vector3::DOWN * CameraSpeed * TimeStep);
    }

    constexpr float SensitivityRel = 0.05f;
    Yaw += Input->GetRelativeMouseMotion().x * SensitivityRel;
    Pitch += Input->GetRelativeMouseMotion().y * SensitivityRel;

    CameraObject->GetTransform().SetRotation( Quaternion(Pitch, Yaw, 0.0f) );

    ScrollSpeed = Input->GetMouseWheel().y;

    Fov -= ScrollSpeed;
    if (Fov <= 1.0f)
        Fov = 1.0f;
    if (Fov >= 45.0f)
        Fov = 45.0f;
    ScrollSpeed = 0.0f;

    CameraComponent->SetFOV(Fov);

    // Blur Toggle
    if (Input->IsKeyDown(EKey::V))
    {
        BlurEffect->SetEnabled(!BlurEffect->IsEnabled());
    }
    // Wireframe Mode Toggle
    if (Input->IsKeyDown(EKey::B))
    {
        App->GetRenderer3D()->SetWireframe(!App->GetRenderer3D()->GetWireframe());
    }
}

void CLevel2::OnLateUpdate(const float)
{
}

void CLevel2::OnGUI()
{
    /*CImGUI* Im = App->GetImGUI();
    Im->Begin("Level Demo");
    Im->Text("Position: [" + std::to_string(Camera.GetPosition().x) + ", " + std::to_string(Camera.GetPosition().y) + ", " + std::to_string(Camera.GetPosition().z) + "]");
    Im->Text("Aspect: " + std::to_string(Camera.GetAspect()));
    Im->Text("Yaw: " + std::to_string(Yaw));
    Im->Text("Pitch: " + std::to_string(Pitch));
    Im->Text("FOV: " + std::to_string(Fov));
    Im->End();*/
}

void CLevel2::OnRender()
{
}

void CLevel2::OnLeave()
{
    World.reset();
    MaterialArray{}.swap(Materials);
    //
    App->GetInput()->SetMouseMode(EMouseMode::Normal);
}

void CLevel2::OnExit()
{
}