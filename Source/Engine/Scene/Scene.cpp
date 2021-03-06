#include "Scene.hpp"
#include "Entity.hpp"
#include "Components/AudioListener.hpp"
#include "Components/AudioSource.hpp"
#include "Components/Camera2D.hpp"
#include "Components/RectRenderer2D.hpp"
#include "Components/StaticSprite2D.hpp"
#include "Components/RigidBody2D.hpp"
#include "Components/PhysicsWorld2D.hpp"
#include "Components/BoxCollider2D.hpp"
#include "Components/Camera.hpp"
#include "Components/MeshRenderer.hpp"
#include "Components/LightComponent.hpp"
#include "Components/RigidBody3D.hpp"
#include "Components/PhysicsWorld3D.hpp"
#include "Components/BoxCollider3D.hpp"
#include "../Engine/Engine.hpp"

CScene::CScene(CEngine* aEngine):
    IEngineModule( "Scene" ),
    Engine(aEngine)
{
}

CScene::~CScene()
{
}

bool CScene::Init(const SEngineParams&)
{
    RegisterComponent<CAudioListener>();
    RegisterComponent<CAudioSource>();
    RegisterComponent<CCamera2D>();
    RegisterComponent<CRectRenderer2D>();
    RegisterComponent<CStaticSprite2D>();
    RegisterComponent<CBoxCollider2D>();
    RegisterComponent<CRigidBody2D>();
    RegisterComponent<CPhysicsWorld2D>();
    RegisterComponent<CCamera>();
    RegisterComponent<CMeshRenderer>();
    RegisterComponent<CLightComponent>();
    RegisterComponent<CBoxCollider3D>();
    RegisterComponent<CRigidBody3D>();
    RegisterComponent<CPhysicsWorld3D>();
    //
    RegisterEntity<CEntity>();

    LOG( ESeverity::Info ) << "Scene Module - Init\n";
    return true;
}

CEntity* CScene::CreateEntity(const std::string& Type)
{
    IEntityFactory* Factory = nullptr;
    for(const auto& i: EntityFactory)
    {
        if( i->GetType() == Type )
        {
            Factory = i.get();
            break;
        }
    }
    if( !Factory )
    {
        LOG(ESeverity::Warning) << "No Factory for Entity - " << Type << "\n";
        return nullptr;
    }
    return Factory->CreateEntity();
}

IComponent* CScene::CreateComponent(const std::string& Type)
{
    IComponentFactory* Factory = nullptr;
    for(const auto& i: ComponentFactory)
    {
        if( i->GetType() == Type )
        {
            Factory = i.get();
            break;
        }
    }
    if( !Factory )
    {
        LOG(ESeverity::Warning) << "No Factory for Component - " << Type << "\n";
        return nullptr;
    }
    return Factory->CreateComponent();
}

void CScene::OnBeginFrame()
{
    EventManager.Send<EBeginFrame>();
}

void CScene::OnUpdate(const float DT)
{
    EventManager.Send<EUpdate>( DT );
}

void CScene::OnLateUpdate(const float DT)
{
    EventManager.Send<ELateUpdate>( DT );
}

void CScene::OnGUI()
{
    EventManager.Send<EGUI>();
}

void CScene::OnRender()
{
    EventManager.Send<ERender>();
}

void CScene::OnEndFrame()
{
    EventManager.Send<EEndFrame>();
}

void CScene::OnExit()
{
}
