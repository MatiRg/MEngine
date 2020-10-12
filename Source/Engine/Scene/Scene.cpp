#include "Scene.hpp"
#include "Components/AudioListener2D.hpp"
#include "Components/AudioSource2D.hpp"
#include "Components/Camera2D.hpp"
#include "Components/RectRenderer2D.hpp"
#include "Components/StaticSprite2D.hpp"
#include "../Physics/PhysicsWorld2D.hpp"
#include "../Physics/RigidBody2D.hpp"
#include "../Physics/BoxCollider2D.hpp"
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
    RegisterComponentFactory<CAudioListener2D>();
    RegisterComponentFactory<CAudioSource2D>();
    RegisterComponentFactory<CBoxCollider2D>();
    RegisterComponentFactory<CRigidBody2D>();
    RegisterComponentFactory<CPhysicsWorld2D>();
    RegisterComponentFactory<CCamera2D>();
    RegisterComponentFactory<CRectRenderer2D>();
    RegisterComponentFactory<CStaticSprite2D>();
    //
    RegisterEntityFactory<CEntity2D>();

    LOG( ESeverity::Info ) << "Scene Module - Init\n";
    return true;
}

CEntity2D* CScene::CreateEntity(const std::string& Type)
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
        LOG(ESeverity::Warning) << "No Factory for Entity2D - " << Type << "\n";
        return nullptr;
    }
    return Factory->CreateEntity();
}

CEntity2D* CScene::CreateEntityGlobal(const std::string& Type)
{
    auto Entity = CreateEntity(Type);
    if( Entity )
    {
        Entity->SetID(GLOBAL_ENTITY);
        Entity->SetWorld(nullptr);
        Entity->SetParent(nullptr);
        //
        Entity->OnCreate();
    }
    return Entity;
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
