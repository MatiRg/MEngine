# Documentation
Documentation is Work in Progress.
## File Formats
See [FORMATS.md](FORMATS.md)
## Creating App
Creating Basic Application is simple:
* Create Simple App class
```C++
#pragma once
#include "Engine/Full.hpp"

class CMainApp: public CApp
{
public:
    CMainApp() = default;
    ~CMainApp() = default;
protected:
    bool Init(const SEngineParams&) override;
    void OnInit() override;
    void Exit() override;
};
```
After that you define main function:
```C++
ENTRY_POINT( CMainApp, "log_helloworld.txt" )
```
Where `CMainApp` is App class name and `log_helloworld.txt` is file that message will be saved.

TO DO
