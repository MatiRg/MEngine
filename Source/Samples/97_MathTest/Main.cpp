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

/////////////////////////////////////////////
/////////////////////////////////////////////

class CTransform2;

using Transform2Changed = std::function<void(CTransform2*)>;

class CTransform2 final : public NonCopyableMovable
{
public:
    CTransform2()
    {
        ADD_MEMORY_RECORD(this);
    }

    ~CTransform2()
    {
        if (Parent)
        {
            Parent->RemoveChild(this);
        }
        //
        ERASE_MEMORY_RECORD(this);
    }

    //
    //Save/Load Here
    //

    void SetParent(CTransform2* aParent)
    {
        // Old Parent
        if (Parent)
        {
            Parent->RemoveChild(this);
        }
        //
        Parent = aParent;
        if (Parent)
        {
            Parent->AddChild(this);
        }
    }
    //
    bool HasParent() const { return Parent; }
    //
    CTransform2* GetParent() const { return Parent; }

    void AddChild(CTransform2* Other)
    {
        if (Other)
        {
            Children.push_back(Other);
        }
    }
    //
    void RemoveChild(CTransform2* Other)
    {
        if (Other)
        {
            auto Iterator = std::find_if(Children.begin(), Children.end(), [&](CTransform2* Transform)
            {
                return Transform == Other;
            });
            if (Iterator != Children.end())
            {
                Children.erase(Iterator);
            }
        }
    }
    //
    const std::vector<CTransform2*>& GetChildren() const { return Children; }
    //
    bool HasChildren() const { return !Children.empty(); }
    //
    std::size_t GetChildrenCount() const { return Children.size(); }

    void SetPosition(const Vector3& aPosition, const bool Silent = false)
    {
        Position = aPosition;
        MarkDirty();
        if (!Silent)
        {
            for (const auto& i : PositionCallback)
            {
                i.second(Position);
            }
        }
    }
    const Vector3& GetPosition() const { return Position; }

    void SetRotation(const Quaternion& aRotation, const bool Silent = false)
    {
        Rotation = aRotation;
        MarkDirty();
        if (!Silent)
        {
            for (const auto& i : RotationCallback)
            {
                i.second(Rotation);
            }
        }
    }
    const Quaternion& GetRotation() const { return Rotation; }

    void SetScale(const Vector3& aScale, const bool Silent = false)
    {
        Scale = aScale;
        MarkDirty();
        if (!Silent)
        {
            for (const auto& i : ScaleCallback)
            {
                i.second(Scale);
            }
        }
    }
    const Vector3& GetScale() const { return Scale; }

    void Translate(const Vector3& Delta)
    {
        SetPosition(Position + Delta);
    }

    Matrix4 GetMatrix() const
    {
        Matrix4 Mtx = glm::mat4(1.0f);
        Mtx = glm::translate(Mtx, Position);
        Mtx = Mtx * glm::mat4_cast(Rotation);
        Mtx = glm::scale(Mtx, Scale);
        return Mtx;
    }

    Matrix4 GetWorldMatrix() const
    {
        Matrix4 Mtx = GetMatrix();
        if (HasParent())
        {
            Mtx = Parent->GetWorldMatrix() * Mtx;
        }
        return Mtx;
    }

    Quaternion GetWorldRotation() const
    {
        Quaternion Tmp = Rotation;
        if (HasParent())
        {
            Tmp = Parent->GetWorldRotation() * Tmp;
        }
        return Tmp;
    }
private:
    void MarkDirty()
    {
        Dirty = true;
        for (const auto& i : Children)
        {
            i->MarkDirty();
        }
        //
        for (const auto& i : TransformChangedCallback)
        {
            i.second(this);
        }
    }
private:
    glm::vec3 Position = glm::vec3(0.0f);
    glm::vec3 Scale = glm::vec3(1.0f);
    glm::quat Rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
    CTransform2* Parent = nullptr;
    std::vector<CTransform2*> Children;

    //

    // Cache, mutable is used to be modified from const methods. These variables work as cache only
    mutable bool Dirty = true;
    mutable glm::mat4 Matrix = glm::mat4(1.0f);
    mutable glm::mat4 InvMatrix = glm::mat4(1.0f);
    mutable glm::mat4 WorldMatrix = glm::mat4(1.0f);
    mutable glm::mat4 InvWorldMatrix = glm::mat4(1.0f);
    std::vector<std::pair<void*, PositionChanged>> PositionCallback;
    std::vector<std::pair<void*, ScaleChanged>> ScaleCallback;
    std::vector<std::pair<void*, RotationChanged>> RotationCallback;
    std::vector<std::pair<void*, Transform2Changed>> TransformChangedCallback;
};

/////////////////////////////////////////////
/////////////////////////////////////////////

class CLevel1: public IUpdatable
{
public:
    CLevel1(CMainApp* aApp): IUpdatable("Level 1"), App(aApp) {}

    UPDATABLE(CLevel1)

    void OnEnter() override;

    void OnUpdate(const float TimeStep) override
    {
        if (Input->IsKeyDown(EKey::Escape))
        {
            App->Quit();
        }
        // Camera
        constexpr float SensitivityRel = 0.05f;
        Yaw += Input->GetRelativeMouseMotion().x * SensitivityRel;
        Pitch += Input->GetRelativeMouseMotion().y * SensitivityRel;

        Pitch = Math::Clamp(Pitch, -30.0f, 20.0f);

        // Set New Rotation in Euler Angles
        Camera.SetRotation(Math::FromEulerAngles(Pitch, Yaw, 0.0f));

        constexpr float PlayerSpeed = 1.0f;
        Vector3 Keys = VECTOR3_ZERO;
        if (Input->IsKeyPressed(EKey::W))
        {
            Keys.z = PlayerSpeed;
        }
        if (Input->IsKeyPressed(EKey::S))
        {
            Keys.z = -PlayerSpeed;
        }
        if (Input->IsKeyPressed(EKey::A))
        {
            Keys.x = -PlayerSpeed;
        }
        if (Input->IsKeyPressed(EKey::D))
        {
            Keys.x = PlayerSpeed;
        }
        Vector3 Velocity = VECTOR3_FORWARD * Keys.z + VECTOR3_RIGHT * Keys.x;
        Player.Translate(Camera.GetRotation() * Velocity * TimeStep);


    }

    void OnRender() override
    {
        glm::mat4 WorldCam = Camera.GetWorldMatrix();
        glm::vec3 CameraPos = glm::vec3(WorldCam[3][0], WorldCam[3][1], WorldCam[3][2]);

        glm::mat4 VM = glm::mat4(1.0f);
        VM = glm::translate(VM, CameraPos);
        VM = VM * glm::mat4_cast(Camera.GetWorldRotation());
        VM = glm::inverse(VM);

        Renderer->SetViewMatrix(VM);
        Renderer->SetCameraPosition(CameraPos);

        Renderer->SetProjectionMatrix( glm::perspective(glm::radians(60.0f), Window->GetAspectRatio(), 0.3f, 1000.0f) );
        //
        SkyDome.SetMatrix( Sky.GetWorldMatrix() );
        Renderer->AddRenderable(&SkyDome);
        //
        Box1.SetMatrix(BoxNode.GetWorldMatrix());
        Renderer->AddRenderable(&Box1);
    }

    void OnLeave() override
    {
        App->GetInput()->SetMouseMode(EMouseMode::Normal);
    }
private:
    CMainApp* App;
    IInput* Input = nullptr;
    IGraphics* Graphics = nullptr;
    IWindow* Window = nullptr;
    CRenderer3D* Renderer = nullptr;
    CResources* Resources = nullptr;
    //
    CTransform2 Root;
    CTransform2 Sky;
    CTransform2 Player;
    CTransform2 Camera;
    CTransform2 BoxNode;
    //
    CRenderable3D SkyDome{ERenderableType::Solid};
    CRenderable3D Box1{ ERenderableType::Solid };
    //
    float Yaw = 0.0f;
    float Pitch = 0.0f;
};

void CLevel1::OnEnter()
{
    Input = App->GetInput();
    Graphics = App->GetGraphics();
    Window = App->GetWindow();
    Renderer = App->GetRenderer3D();
    Resources = App->GetResources();
    ////
    Sky.SetParent(&Root);
    Sky.SetScale(glm::vec3(500.0f));
    Sky.SetRotation(Math::FromEulerAngles(90.0f, 0.0f, 0.0f));
    //
    Player.SetParent(&Root);
    Camera.SetParent(&Player);
    //
    BoxNode.SetParent(&Root);
    ////
    CModel* Sky2 = Resources->CreateResource<CModel>("sky2.dae");
    SkyDome.SetVertexBuffer(Sky2->GetMeshes()[0]->GetVertexBuffer());
    SkyDome.SetMaterial(Sky2->GetMeshes()[0]->GetMaterial());
    //
    CModel* Cube1 = Resources->CreateResource<CModel>("cube.dae");
    Box1.SetVertexBuffer(Cube1->GetMeshes()[0]->GetVertexBuffer());
    Box1.SetMaterial(Cube1->GetMeshes()[0]->GetMaterial());
    //
    App->GetInput()->SetMouseMode(EMouseMode::Relative);
}

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
    GetWindow()->SetCaption("Math Test!"); 
    GetUserUpdater()->SetContainer(LVL1_CONTAINER);
}

void CMainApp::Exit()
{
    Level1.reset();
}

ENTRY_POINT( CMainApp, "log_helloworld.txt" )