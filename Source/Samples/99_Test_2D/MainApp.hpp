#pragma once
#include "Engine/Full.hpp"
#include "MainMenu.hpp"
#include "PlayGround.hpp"
#include "PhysicsPlay.hpp"
#include "Globals.hpp"

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
    std::unique_ptr<CMainMenu> Menu;
    std::unique_ptr<CPlayGround> Play;
    std::unique_ptr<CPhysicsPlay> PhysicsPlay;
};
