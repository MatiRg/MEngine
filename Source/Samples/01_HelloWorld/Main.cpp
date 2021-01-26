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