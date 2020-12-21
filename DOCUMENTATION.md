# Documentation
Documentation is Work in Progress.
## File Formats
See [FORMATS.md](FORMATS.md)
## Creating App
Creating Empty Application is simple:
* Create Empty App class
```C++
#pragma once
#include "Engine/Full.hpp"

class CMainApp: public CApp
{
public:
    CMainApp() = default;
    ~CMainApp() = default;
protected:
    bool Init() override
    {
        // Adding Resources Search Path, Creating Updatable Containers with CUserUpdater
        // Create Game Updatables add add them to given Container or to global container, 
    }

    void OnInit() override
    {
        // Game Init, after Enigne Modules & Game Updatables are created
        // You can set here stating container with CUserUpdater
    }

    void Exit() override
    {
        // Destroy here Game Updatables
    }
};
```
After that you have to define main function in somewhere in .cpp file using this macro:
```C++
ENTRY_POINT( CMainApp, "log_helloworld.txt" )
```
Where `CMainApp` is App class name and `log_helloworld.txt` is file that messages will be saved by logger.

TO DO
