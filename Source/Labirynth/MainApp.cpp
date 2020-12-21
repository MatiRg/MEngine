#include "MainApp.hpp"
#include "Globals.hpp"

bool CMainApp::Init()
{
    Level2 = std::make_unique<CLevel2>(this);

    GetUserUpdater()->AddContainer(LEVEL2_CONTAINER);
    GetUserUpdater()->AddUpdatable(LEVEL2_CONTAINER, Level2.get());

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
    GetWindow()->SetCaption("Labirynth");
    GetGraphics()->SetClearColor( Color( 0.12f, 0.50f, 0.92f, 1.0f ) );
    
    GetUserUpdater()->SetContainer(LEVEL2_CONTAINER);
}

void CMainApp::Exit()
{
    Level2.reset();
}