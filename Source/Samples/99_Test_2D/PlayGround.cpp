#include "PlayGround.hpp"
#include "MainApp.hpp"
#include "Globals.hpp"

static constexpr int FONT_SIZE = 24;
static constexpr char ENEMY_TAG[] = "Enemy";
static constexpr char PLAYER_NAME[] = "Player";
//
static constexpr char WORLD2D_NAME[] = "MyWorld2D.world2d";

class CFallComponent: public IComponent
{
public:
    CFallComponent(CEngine* aEngine): IComponent(aEngine) {}

    COMPONENT(CFallComponent)

    bool OnLoad(CXMLElement* Root) override
    {
		Speed = XML::LoadFloat( Root, "Speed", 0.0f );
        return true;
    }

    bool OnSave(CXMLElement* Root) override
    {
        XML::SaveFloat( Root, "Speed", Speed);
        return true;
    }

    void OnUpdate(const float DT) override
    {
        auto& Transform = GetOwner()->GetTransform();

        Vector2 Position = Transform.GetPosition2D();
        Position -= Vector2::UP*Speed*DT;

        Transform.SetPosition2D( Position );
    }

    void SetSpeed(const float aSpeed) { Speed = aSpeed; }
    float GetSpeed() const { return Speed; }
private:
    float Speed = 0.0f;
};

//

class CEnemy: public CEntity
{
public:
    CEnemy(CEngine* aEngine): CEntity(aEngine) {}

    ENTITY(CEnemy)

    void OnStart() override
    {
        Transform.SetPosition2D( Vector2( Math::Random(0, Engine->GetWindow()->GetSize().x), Engine->GetWindow()->GetSize().y+48.0f) );

        auto Rect = CreateComponent<CRectRenderer2D>();
        Rect->SetSize( Vector2(Math::Random(8.0f, 48.0f)) );
        Rect->SetColor( Color::GREEN );

        auto Fall = CreateComponent<CFallComponent>();
        Fall->SetSpeed( Math::Random(75.0f, 200.0f) );

        SetTag( ENEMY_TAG );
    }

    void OnLateUpdate(const float) override
    {
        Vector2 Position = Transform.GetWorldPosition2D();
        if( Position.y < 0.0f )
        {
            Parent->DestroyChild(this);
        }
    }
};

//

class CPlayer: public CEntity
{
public:
    CPlayer(CEngine* aEngine): CEntity(aEngine) {}

    ENTITY(CPlayer)

    bool OnLoad(CXMLElement* Root) override
    {
		GameOver = XML::LoadBool(Root, "GameOver", false);
        return true;
    }

    bool OnSave(CXMLElement* Root) override
    {
        XML::SaveBool( Root, "GameOver", GameOver);
        return true;
    }

    void OnCreate() override
    {
        Transform.SetPosition2D( Vector2( 400.0f, 64.0f ) );

        auto Rect = CreateComponent<CStaticSprite2D>();
        Rect->SetTexture( Engine->GetResources()->CreateResource<ITexture2D>("Sprite1.png") );
        Rect->SetSize( Vector2(32.0f) );
        Rect->SetFlip(ETextureFlip::Both);

        CreateComponent<CAudioListener>();

        AudioSource = CreateComponent<CAudioSource>();
        AudioSource->SetLooped(true);
        AudioSource->SetVolume(0.75f);

        ResourceCreateMap ResVar;
        ResVar[RESOURCES_VAR_IS_STREAM] = true;
        AudioSource->SetData(Engine->GetResources()->CreateResource<ISoundData>("Debussy - Arabesque No. 1 (Ciccolini).ogg", ResVar));

        AudioSource->Play();

        CameraObject = CreateChild<CEntity>();
        auto Camera = CameraObject->CreateComponent<CCamera2D>();
        Camera->SetSize( Vector2(800.0f, 600.0f) );

        SetName( PLAYER_NAME );

        auto Right = CreateChild<CEntity>();
        Right->GetTransform().SetPosition2D(Vector2(16.0f, -16.0f) );
        auto RCRight = Right->CreateComponent<CRectRenderer2D>();
        RCRight->SetColor( Color::YELLOW );
        RCRight->SetSize( Vector2(16.0f) );

        auto Left = CreateChild<CEntity>();
        Left->GetTransform().SetPosition2D(Vector2(-16.0f, -16.0f) );
        auto RCLeft = Left->CreateComponent<CRectRenderer2D>();
        RCLeft->SetColor( Color::BLUE );
        RCLeft->SetSize( Vector2(16.0f) );
    }

    void OnUpdate(const float DT) override
    {
        Vector2 Position = Transform.GetPosition2D();
        if( Engine->GetInput()->IsKeyPressed(EKey::A) )
        {
            Position += Vector2::LEFT*Speed*DT;
        }
        if( Engine->GetInput()->IsKeyPressed(EKey::D) )
        {
            Position += Vector2::RIGHT*Speed*DT;
        }
        if( Engine->GetInput()->IsKeyPressed(EKey::W) )
        {
            Position -= Vector2::DOWN*Speed*DT;
        }
        if( Engine->GetInput()->IsKeyPressed(EKey::S) )
        {
            Position -= Vector2::UP*Speed*DT;
        }
        Transform.SetPosition2D( Position );
        //
        float Angle = Transform.GetRotation2D();
        if( Engine->GetInput()->IsKeyPressed(EKey::Q) )
        {
            Angle -= 1.0f;
        }
        if( Engine->GetInput()->IsKeyPressed(EKey::E) )
        {
            Angle += 1.0f;
        }
        Transform.SetRotation2D(Angle);
        //
        Vector2 Scale = Transform.GetScale2D();
        if( Engine->GetInput()->IsKeyDown(EKey::O) )
        {
            Scale -= 0.25f;
        }
        if( Engine->GetInput()->IsKeyDown(EKey::P) )
        {
            Scale += 0.25f;
        }
        Transform.SetScale2D(Scale);
        //

        Rect2 Box1 = GetComponent<CStaticSprite2D>()->GetBox();
        auto Enemies = World->GetChildren<CEnemy>(ENEMY_TAG);
        for(const auto& Enemy: Enemies)
        {
            Rect2 Box2 = Enemy->GetComponent<CRectRenderer2D>()->GetBox();
            if( Box1.Intersect(Box2) )
            {
                GameOver = true;
                World->SetUpdated(false);
            }
        }
    }

    void OnGUI() override
    {
        auto ImUI = Engine->GetImGUI();
        ImUI->Begin("Player");
        ImUI->Text( "Angle: "+std::to_string(Transform.GetRotation2D()) );
        ImUI->Text( "Scale: " + std::to_string( Transform.GetScale2D().x )+","+std::to_string( Transform.GetScale2D().y) );
        ImUI->End();
    }

    void OnDestroy() override
    {
        AudioSource->Stop();
    }

    bool IsGameOver() const { return GameOver; }
private:
    const float Speed = 100.0f;
    bool GameOver = false;
    //
    CEntity* CameraObject = nullptr;
    CAudioSource* AudioSource = nullptr;
};

//

class CGameManager: public CEntity
{
public:
    CGameManager(CEngine* aEngine): CEntity(aEngine) {}

    ENTITY(CGameManager)

    bool OnLoad(CXMLElement* Root) override
    {
		Score = XML::LoadFloat( Root, "Score", 0.0f);
		TimeSpawn = XML::LoadFloat( Root, "TimeSpawn", 0.0f);
		TopScore = XML::LoadInt( Root, "TopScore", 0);
        return true;
    }

    bool OnSave(CXMLElement* Root) override
    {
        XML::SaveFloat( Root, "Score", Score);
        XML::SaveFloat( Root, "TimeSpawn", TimeSpawn);
        XML::SaveInt( Root, "TopScore", TopScore);
        return true;
    }

    void OnCreate() override
    {
        Font = Engine->GetResources()->CreateResource<IFont>("Default.ttf");
    }

    void OnUpdate(const float DT) override
    {
        auto PlayerObject = World->GetChild<CPlayer>( PLAYER_NAME );
        if( PlayerObject && PlayerObject->IsGameOver() )
        {
            if( static_cast<float>(Score) > TopScore )
            {
                TopScore = static_cast<int>(Math::Floor(Score));
            }
            return;
        }
        //
        Score += DT;
        TimeSpawn += DT;
        if( TimeSpawn > TimeToSpawn )
        {
            World->CreateChild<CEnemy>();
            TimeSpawn -= TimeToSpawn;
        }
    }

    void OnRender() override
    {
        CDrawer2D* Drawer = Engine->GetDrawer2D();
        Drawer->DrawText( Font, "Press 'Escape' to Return to Menu", Vector2(32.0f, 16.0f), FONT_SIZE, Color::WHITE, 1000.0f );
        Drawer->DrawText( Font, "Score: "+std::to_string(static_cast<int>(Math::Floor(Score))),
            Vector2(32.0f, 40.0f), FONT_SIZE, Color::WHITE, 1000.0f );
        Drawer->DrawText( Font, "Top Score: "+std::to_string(TopScore), Vector2(32.0f, 64.0f), FONT_SIZE, Color::WHITE, 1000.0f );
    }
private:
    IFont* Font = nullptr;
    float Score = 0.0f;
    const float TimeToSpawn = 0.1f;
    float TimeSpawn = 0.0f;
    int TopScore = 0;
};

CPlayGround::CPlayGround(CMainApp* aApp):
    IUpdatable("Play Ground"),
    App(aApp)
{
}

CPlayGround::~CPlayGround()
{
}

void CPlayGround::OnInit()
{
    App->GetScene()->RegisterComponent<CFallComponent>();
    //
    App->GetScene()->RegisterEntity<CGameManager>();
    App->GetScene()->RegisterEntity<CEnemy>();
    App->GetScene()->RegisterEntity<CPlayer>();
}

void CPlayGround::OnEnter()
{
    World = std::make_unique<CWorld>( App->GetEngine() );
    World->CreateChild<CGameManager>();
    Player = World->CreateChild<CPlayer>();
}

void CPlayGround::OnUpdate(const float)
{
}

void CPlayGround::OnLateUpdate(const float)
{
    IInput* Input = App->GetEngineUpdater()->GetEngineModule<IInput>();

    if( Input->IsKeyDown(EKey::Escape) )
    {
        App->GetUserUpdater()->SetContainer(MENU_CONTAINER);
    }
    //
    if( Input->IsKeyDown(EKey::F9) )
    {
        World = std::make_unique<CWorld>(App->GetEngine());
        if( !World->Load(WORLD2D_NAME) )
        {
            World.reset();
            LOG(ESeverity::Error) << "Error During World 2D Loading\n";
        }
        Player = World->GetChild<CPlayer>( PLAYER_NAME );
    }
    if( Input->IsKeyDown(EKey::F5) )
    {
        if( World )
        {
            World->Save(WORLD2D_NAME);
        }
    }
    //
    if( Input->IsKeyDown(EKey::T) )
    {   
        Player->SetActive( !Player->IsActive() );
    }
}

namespace
{
    void PrintWorldHierarchy(CImGUI* ImUI, CEntity* Root, const int Level)
    {
        if( Root )
        {
            std::string Text(Level, ' ');
            Text.append(std::to_string(Root->GetID())+".").append(Root->GetType()).append(" - ").append(Root->GetName()).append(" - ").append(Root->GetTag());
            ImUI->Text( Text );
            auto Children = Root->GetChildren();
            for(const auto& i: Children)
            {
                PrintWorldHierarchy(ImUI, i, Level+1);
            }
        }
    }
}

void CPlayGround::OnGUI()
{
    #ifdef DEV_BUILD
    auto ImUI = App->GetImGUI();
    ImUI->Begin("World Information");
    PrintWorldHierarchy(ImUI, World.get(), 0);
    ImUI->End();
    #endif
}

void CPlayGround::OnRender()
{
}

void CPlayGround::OnLeave()
{
    World.reset();
    Player = nullptr;
}
