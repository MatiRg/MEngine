#include "MainApp.hpp"

bool CMainApp::Init()
{
    Menu = std::make_unique<CMainMenu>(this);

    GetUserUpdater()->AddContainer(MENU_CONTAINER);
    GetUserUpdater()->AddUpdatable(MENU_CONTAINER, Menu.get());

    Play = std::make_unique<CPlayGround>(this);

    GetUserUpdater()->AddContainer(DODGER_CONTAINER);
    GetUserUpdater()->AddUpdatable(DODGER_CONTAINER, Play.get());

    // PHYSICS_CONTAINER
    PhysicsPlay = std::make_unique<CPhysicsPlay>(this);

    GetUserUpdater()->AddContainer(PHYSICS_CONTAINER);
    GetUserUpdater()->AddUpdatable(PHYSICS_CONTAINER, PhysicsPlay.get());

    if( !GetSystem()->DirectoryExist("Data") )
    {
        LOG(ESeverity::Fatal) << "No 'Data' Directory\n";
        return false;
    }
    GetResources()->AddPath("Data", true);

    return true;
}

void CMainApp::OnInit()
{
    GetWindow()->SetCaption("Dodger");
    GetGraphics()->SetClearColor( Color( 0.12f, 0.50f, 0.92f, 1.0f ) );
    
    GetUserUpdater()->SetContainer(MENU_CONTAINER);
}

void CMainApp::Exit()
{
    PhysicsPlay.reset();
    Play.reset();
    Menu.reset();
}