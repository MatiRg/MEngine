#include "MainMenu.hpp"
#include "MainApp.hpp"
#include "Globals.hpp"

static constexpr int FONT_SIZE = 36;
static constexpr char PHYSICS_TEXT[] = "PHYSICS";
static constexpr char DODGER_TEXT[] = "DODGER";
static constexpr char QUIT_TEXT[] = "QUIT";

CMainMenu::CMainMenu(CMainApp* aApp):
    IUpdatable("Main Menu"),
    App(aApp)
{
}

CMainMenu::~CMainMenu()
{
}

void CMainMenu::OnInit()
{
    Font = App->GetResources()->CreateResource<IFont>("Default.ttf");

    IWindow* Window = App->GetWindow();
    IntVector2 ContainerSize = IntVector2( Window->GetSize().x * 0.25,  Window->GetSize().y * 0.5);
    IntVector2 ContainerPosition = Window->GetSize() / 2;

    PhysicsPosition = Rect2( Vector2( ContainerPosition.x-Font->GetTextLength(PHYSICS_TEXT, FONT_SIZE)/2, ContainerPosition.y-Font->GetTextHeight(PHYSICS_TEXT, FONT_SIZE)*1.5f ),
        Vector2(Font->GetTextLength(PHYSICS_TEXT, FONT_SIZE), Font->GetTextHeight(PHYSICS_TEXT, FONT_SIZE) ) );

    DodgerPosition = Rect2( Vector2( ContainerPosition.x-Font->GetTextLength(DODGER_TEXT, FONT_SIZE)/2, ContainerPosition.y-Font->GetTextHeight(DODGER_TEXT, FONT_SIZE)/2 ),
        Vector2(Font->GetTextLength(DODGER_TEXT, FONT_SIZE), Font->GetTextHeight(DODGER_TEXT, FONT_SIZE) ) );

    QuitPosition = Rect2( Vector2( ContainerPosition.x-Font->GetTextLength(QUIT_TEXT, FONT_SIZE)/2, ContainerPosition.y+Font->GetTextHeight(QUIT_TEXT, FONT_SIZE)/2 ),
        Vector2(Font->GetTextLength(QUIT_TEXT, FONT_SIZE), Font->GetTextHeight(QUIT_TEXT, FONT_SIZE) ) );
}

void CMainMenu::OnEnter()
{
}

void CMainMenu::OnUpdate(const float)
{
}

void CMainMenu::OnLateUpdate(const float)
{
    Vector2 Mouse = App->GetInput()->GetMousePosition();
    if( PhysicsPosition.Intersect(Mouse) )
    {
        PhysicsColor = Color::WHITE;
        if( App->GetInput()->IsMouseKeyDown(EMouseKey::LButton) )
        {
            App->GetUserUpdater()->SetContainer(PHYSICS_CONTAINER);
        }
    }
    else
    {
        PhysicsColor = Color::GREY;
    }

    if( DodgerPosition.Intersect(Mouse) )
    {
        DodgerColor = Color::WHITE;
        if( App->GetInput()->IsMouseKeyDown(EMouseKey::LButton) )
        {
            App->GetUserUpdater()->SetContainer(DODGER_CONTAINER);
        }
    }
    else
    {
        DodgerColor = Color::GREY;
    }

    if( QuitPosition.Intersect(Mouse) )
    {
        QuitColor = Color::WHITE;
        if( App->GetInput()->IsMouseKeyDown(EMouseKey::LButton) )
        {
            App->Quit();
        }
    }
    else
    {
        QuitColor = Color::GREY;
    }
}

void CMainMenu::OnGUI()
{
}

void CMainMenu::OnRender()
{
    CDrawer2D* Drawer = App->GetDrawer2D();
    Drawer->DrawText( Font, PHYSICS_TEXT, PhysicsPosition.GetPosition(), FONT_SIZE, PhysicsColor, 0.0f );
    Drawer->DrawText( Font, DODGER_TEXT, DodgerPosition.GetPosition(), FONT_SIZE, DodgerColor, 0.0f );
    Drawer->DrawText( Font, QUIT_TEXT, QuitPosition.GetPosition(), FONT_SIZE, QuitColor, 0.0f );
}
