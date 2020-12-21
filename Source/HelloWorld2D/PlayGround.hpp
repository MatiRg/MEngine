#pragma once
#include "../Engine/Full.hpp"

class CMainApp;

class CPlayGround: public IUpdatable
{
public:
    CPlayGround(CMainApp*);
    ~CPlayGround();

    UPDATABLE(CPlayGround)

    void OnInit() override;

    void OnEnter() override;
    void OnUpdate(const float) override;
    void OnLateUpdate(const float) override;
    void OnGUI() override;
    void OnRender() override;
    void OnLeave() override;
private:
    CMainApp* App = nullptr;
    std::unique_ptr<CWorld> World;
    CEntity* Player = nullptr;
};
