#include "Level2.hpp"
#include "MainApp.hpp"
#include "Globals.hpp"

CLevel2::CLevel2(CMainApp* aApp) :
    IUpdatable("Level2"),
    App(aApp)
{
}

CLevel2::~CLevel2()
{
}

void CLevel2::OnInit()
{
    CResources* Resources = App->GetResources();
    //
    Material = std::make_unique<CMaterial>();
    Material->SetShader( Resources->CreateResource<IShader>("TextureLSDEffects.shader") );
    Material->SetTexture("Diffuse", Resources->CreateResource<ITexture2D>("wall.jpg") );
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
    CEntity* Entity = World->CreateChild<CEntity>();
    CameraObject = World->CreateChild<CEntity>();

    CMeshRenderer* MeshRenderer = Entity->CreateComponent<CMeshRenderer>();
    CResources* Resources = App->GetResources();

    //CModel* ModelCube = Resources->CreateResource<CModel>("backpack.obj");
    CModel* ModelCube = Resources->CreateResource<CModel>("cube.dae");
   
    MeshRenderer->SetMesh(ModelCube->GetMeshes()[0]);
    MeshRenderer->SetMaterial( Resources->CreateResource<CMaterial>("Default.mat") );

    PrintModel(ModelCube->GetRoot(), 0);

    CameraComponent = CameraObject->CreateComponent<CCamera>();
    CameraComponent->SetAspect(App->GetWindow()->GetAspectRatio());
    CameraComponent->SetFOV(Fov);
    CameraComponent->SetNearClipPlane(0.3f);
    CameraComponent->SetFarClipPlane(1000.0f);
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
    Material->SetFloat("Time", Time);
}

void CLevel2::OnLeave()
{
    World.reset();
    Material.reset();
    //
    App->GetInput()->SetMouseMode(EMouseMode::Normal);
}

void CLevel2::OnExit()
{
}