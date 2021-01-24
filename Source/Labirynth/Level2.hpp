#pragma once
#include "../Engine/Full.hpp"

class CMainApp;

class CLevel2: public IUpdatable
{
public:
    CLevel2(CMainApp*);
    ~CLevel2();

    UPDATABLE(CLevel2)

    void OnInit() override;

    void OnEnter() override;
    void OnUpdate(const float) override;
    void OnLateUpdate(const float) override;
    void OnGUI() override;
    void OnRender() override;
    void OnLeave() override;
    void OnExit() override;
private:
    CMainApp* App = nullptr;
    //
    float Time = 0.0f;
    float CameraSpeed = 10.0f;
    float Yaw = -90.0f;
    float Pitch = 0.0f;
    float Fov = 45.0f;
    float ScrollSpeed = 0.0f;
    ////////////////////////////
    std::unique_ptr<CWorld> World;
    std::unique_ptr<CMaterial> Material;
    CEntity* CameraObject = nullptr;
    CCamera* CameraComponent = nullptr;
    ///////////////////////////
    CPostEffect* BlurEffect = nullptr;
};
