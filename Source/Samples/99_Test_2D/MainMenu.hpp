#pragma once
#include "Engine/Full.hpp"

class CMainApp;

class CMainMenu: public IUpdatable
{
public:
    CMainMenu(CMainApp*);
    ~CMainMenu();

    UPDATABLE(CMainMenu)

    void OnInit() override;

    void OnEnter() override;
    void OnUpdate(const float) override;
    void OnLateUpdate(const float) override;
    void OnGUI() override;
    void OnRender() override;
private:
    CMainApp* App = nullptr;
    Rect2 PhysicsPosition;
    Rect2 DodgerPosition;
    Rect2 QuitPosition;
    IFont* Font = nullptr;
    Color PhysicsColor = COLOR_GREY;
    Color DodgerColor = COLOR_GREY;
    Color QuitColor = COLOR_GREY;
};
