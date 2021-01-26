#include "PhysicsPlay.hpp"
#include "MainApp.hpp"
#include "Globals.hpp"

static constexpr int FONT_SIZE = 24;
static constexpr char PLAYER_NAME[] = "Player";
static constexpr char WORLD2D_NAME[] = "MyPhysicsWorld2D.world2d";

//

class CCameraController: public CEntity
{
public:
    CCameraController(CEngine* aEngine): CEntity(aEngine) {}

    ENTITY(CCameraController)

    void OnCreate() override
    {
        Font = Engine->GetResources()->CreateResource<IFont>("Default.ttf");
        //
        Camera = CreateComponent<CCamera2D>();
        Camera->SetSize( Vector2(800.0f, 600.0f) );
    }

    void OnUpdate(const float DT) override
    {
        Vector2 CameraTranslation;
        if( Engine->GetInput()->IsKeyPressed(EKey::A) )
        {
            CameraTranslation += Vector2::LEFT*Speed*DT;
        }
        if( Engine->GetInput()->IsKeyPressed(EKey::D) )
        {
            CameraTranslation += Vector2::RIGHT*Speed*DT;
        }
        if( Engine->GetInput()->IsKeyPressed(EKey::W) )
        {
            CameraTranslation -= Vector2::DOWN*Speed*DT;
        }
        if( Engine->GetInput()->IsKeyPressed(EKey::S) )
        {
            CameraTranslation -= Vector2::UP*Speed*DT;
        }
        Camera->Translate( CameraTranslation );
    }

    void OnGUI() override
    {
        auto ImUI = Engine->GetImGUI();
        ImUI->Begin("Camera");
        ImUI->Text( std::to_string( Camera->GetTranslation().x) + "," +
std::to_string( Camera->GetTranslation().y) );
        ImUI->End();
    }

    void OnRender() override
    {
        CDrawer2D* Drawer = Engine->GetDrawer2D();
        Drawer->DrawText( Font, "Press 'Escape' to Return to Menu", Vector2(32.0f, 16.0f), FONT_SIZE, Color::WHITE, 1000.0f );
    }
private:
    const float Speed = 100.0f;
    IFont* Font = nullptr;
    CCamera2D* Camera = nullptr;
};

//

class CTestObject: public CEntity
{
public:
    CTestObject(CEngine* aEngine): CEntity(aEngine) {}

    ENTITY(CTestObject)

    void OnGUI() override
    {
        auto ImUI = Engine->GetImGUI();
        ImUI->Begin("Test Object");
        if( Collision )
        {
            ImUI->Text("Collision with: "+std::to_string(ContactInfo.RigidBody->GetOwner()->GetID()) );
        }
        ImUI->End();
    }

    void OnCollisionEnter(const SEntityCollision2D& Info) override
    {
        Collision = true;
        ContactInfo = Info;
    }

    void OnCollisionLeave(const SEntityCollision2D&) override
    {
        Collision = false;
    }
private:
    bool Collision = false;
    SEntityCollision2D ContactInfo{nullptr, nullptr};
};

//

CPhysicsPlay::CPhysicsPlay(CMainApp* aApp):
    IUpdatable("Physics Play"),
    App(aApp)
{
}

CPhysicsPlay::~CPhysicsPlay()
{
}

void CPhysicsPlay::OnInit()
{
    App->GetScene()->RegisterEntityFactory<CCameraController>();
    App->GetScene()->RegisterEntityFactory<CTestObject>();
}

void CPhysicsPlay::OnEnter()
{
    World = std::make_unique<CWorld>( App->GetEngine() );
    // Must Be First
    World->CreateComponent<CPhysicsWorld2D>()->SetDebugDraw( true );

    World->CreateChild<CCameraController>();

    Object = World->CreateChild<CTestObject>();
    Object->SetName(PLAYER_NAME);
    Object->GetTransform().SetPosition2D( {400.0f, 500.0f} );
    auto CollisionBox = Object->CreateComponent<CBoxCollider2D>();
    CollisionBox->SetSize(Vector2(32.0f));
    CollisionBox->SetDensity(1.0f);
    Object->CreateComponent<CRigidBody2D>()->SetFixedRotation(false);
    auto BoxObjectComponent = Object->CreateComponent<CRectRenderer2D>();
    BoxObjectComponent->SetSize( Vector2(32.0f) );
    BoxObjectComponent->SetColor( Color::BLUE );

    auto Right = Object->CreateChild<CEntity>();
    Right->GetTransform().SetPosition2D(Vector2(16.0f, -16.0f) );
    //Right->CreateComponent<CRigidBody2D>(); Not Recomended use joint for this
    auto RCRight = Right->CreateComponent<CRectRenderer2D>();
    RCRight->SetColor( Color::YELLOW );
    RCRight->SetSize( Vector2(16.0f) );

    auto Left = Object->CreateChild<CEntity>();
    Left->GetTransform().SetPosition2D(Vector2(-16.0f, -16.0f) );
    //Left->CreateComponent<CRigidBody2D>(); Not Recomended use joint for this
    auto RCLeft = Left->CreateComponent<CRectRenderer2D>();
    RCLeft->SetColor( Color::BLUE );
    RCLeft->SetSize( Vector2(16.0f) );

    auto Ground = World->CreateChild<CEntity>();
    Ground->GetTransform().SetPosition2D( {400.0f, 100.0f} );
    auto CollisionBox2 = Ground->CreateComponent<CBoxCollider2D>();
    CollisionBox2->SetSize(Vector2(800.0f, 16.0f));
    Ground->CreateComponent<CRigidBody2D>()->SetBodyType(ERigidBodyType2D::Static);
}

void CPhysicsPlay::OnUpdate(const float)
{
}

void CPhysicsPlay::OnLateUpdate(const float)
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
        Object = World->GetChild<CTestObject>( PLAYER_NAME );
    }
    if( Input->IsKeyDown(EKey::F5) )
    {
        if( World )
        {
            World->Save(WORLD2D_NAME);
        }
    }
    //
    if( Input->IsKeyDown(EKey::R) )
    {
        if( World )
        {
            Object->GetTransform().SetPosition2D( {400.0f, 500.0f} );
        }
    }
    if( Input->IsKeyDown(EKey::Q) )
    {
        if( World )
        {
            float a = Object->GetTransform().GetRotation2D();
            a -= 1.0f;
            Object->GetTransform().SetRotation2D(a);
        }
    }
    if( Input->IsKeyDown(EKey::E) )
    {
        if( World )
        {
            float a = Object->GetTransform().GetRotation2D();
            a += 1.0f;
            Object->GetTransform().SetRotation2D(a);
        }
    }
    //
    if( Input->IsKeyDown(EKey::T) )
    {   
        Object->SetActive( !Object->IsActive() );
    }
}

void CPhysicsPlay::OnGUI()
{
}

void CPhysicsPlay::OnRender()
{
}

void CPhysicsPlay::OnLeave()
{
    World.reset();
}
