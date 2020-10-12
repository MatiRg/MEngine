#include "MainApp.hpp"

bool CMainApp::Init(const SEngineParams&)
{
    GetUserUpdater()->AddContainer(MENU_CONTAINER);
    Menu = std::unique_ptr<CMainMenu>( GetUserUpdater()->CreateUpdatable<CMainMenu>(MENU_CONTAINER, this) );

    GetUserUpdater()->AddContainer(DODGER_CONTAINER);
    Play = std::unique_ptr<CPlayGround>( GetUserUpdater()->CreateUpdatable<CPlayGround>(DODGER_CONTAINER, this) );

    // PHYSICS_CONTAINER
    GetUserUpdater()->AddContainer(PHYSICS_CONTAINER);
    PhysicsPlay = std::unique_ptr<CPhysicsPlay>( GetUserUpdater()->CreateUpdatable<CPhysicsPlay>(PHYSICS_CONTAINER, this) );

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