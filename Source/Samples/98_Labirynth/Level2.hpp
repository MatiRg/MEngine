#pragma once
#include "Engine/Full.hpp"
#include <array>

class CMainApp;

//

class CGameObject : public CEntity
{
public:
    CGameObject(CEngine*);
    virtual ~CGameObject();

    ENTITY(CGameObject)

    void SetValue(const int x) { Value = x; }
    int GetValue() const { return Value; }
private:
    int Value = 0;
};

//

class CPlayerObject : public CGameObject
{
public:
    CPlayerObject(CEngine*);
    ~CPlayerObject();

    ENTITY(CPlayerObject)

    void OnStart() override;
    void OnUpdate(const float) override;
    void OnRender() override;
    void OnGUI() override;

    void OnCollisionEnter(const SEntityCollision3D& Collision) override;
    void OnCollisionStay(const SEntityCollision3D& Collision) override;
    void OnCollisionLeave(const SEntityCollision3D& Collision) override;

    void SetGoal(const Vector3& aGoal) { Goal = aGoal; }
private:
    float TotalTime = 0.0f;
    int Score = 0;
    Vector3 Goal = Vector3::ZERO;
};


//

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
    using MaterialArray = std::array<std::unique_ptr<CMaterial>, 6>;
private:
    void LoadMap();
private:
    CMainApp* App = nullptr;
    //
    float CameraSpeed = 10.0f;
    float Fov = 45.0f;
    float ScrollSpeed = 0.0f;
    ////////////////////////////
    std::unique_ptr<CWorld> World;
    CEntity* CameraObject = nullptr;
    CTransform* CameraTransform = nullptr;
    CCamera* CameraComponent = nullptr;
    ///////////////////////////
    CPostEffect* BlurEffect = nullptr;
    //
    std::vector<CGameObject*> Objects;
    int Width = 0; 
    int Height = 0;
    int StartX = 0;
    int StartY = 0;
    MaterialArray Materials;
    CPlayerObject* PlayerObject = nullptr;
    CRigidBody3D* PlayerBody = nullptr;
    float PlayerSpeed = 0.75f;
};
