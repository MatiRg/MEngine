#pragma once
#include "../Engine/Full.hpp"

class CMainApp;

class CPhysicsPlay: public IUpdatable
{
public:
    CPhysicsPlay(CMainApp*);
    ~CPhysicsPlay();

    UPDATABLE(CPhysicsPlay)

    void OnInit() override;

    void OnEnter() override;
    void OnUpdate(const float) override;
    void OnLateUpdate(const float) override;
    void OnGUI() override;
    void OnRender() override;
    void OnLeave() override;
private:
    CMainApp* App = nullptr;
    std::unique_ptr<CWorld2D> World;
    CEntity2D* Object = nullptr;
};
