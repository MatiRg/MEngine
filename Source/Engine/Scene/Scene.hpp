#pragma once
#include "Entity.hpp"
#include "Component.hpp"
#include "Event.hpp"
#include "EventManager.hpp"
#include "../Engine/EngineModule.hpp"
#include <vector>

class CEngine;

class CScene final: public IEngineModule
{
public:
    CScene(CEngine*);
    ~CScene();

    ENGINE_MODULE(CScene)

    CEventManager* GetEventManager() { return &EventManager; }

    template<class T>
    void RegisterEntityFactory()
    {
        EntityFactory.push_back( std::make_unique<TEntityFactory<T>>(Engine) );
    }
    //
    CEntity* CreateEntity(const std::string&);
    //
    template<class T>
    T* CreateEntity()
    {
        static_assert(std::is_base_of<CEntity, T>::value, "Must be base of CEntity");
        return dynamic_cast<T*>(CreateEntity(T::GetTypeStatic()));
    }
    //

    template<class T>
    void RegisterComponentFactory()
    {
        ComponentFactory.push_back( std::make_unique<TComponentFactory<T>>(Engine) );
    }
    //
    IComponent* CreateComponent(const std::string&);
    //
    template<class T>
    T* CreateComponent()
    {
        static_assert(std::is_base_of<IComponent, T>::value, "Must be base of IComponent");
        return dynamic_cast<T*>(CreateComponent(T::GetTypeStatic()));
    }

    bool Init(const SEngineParams&) override;
    //
    void OnBeginFrame() override;
    void OnUpdate(const float) override;
    void OnLateUpdate(const float) override;
    void OnGUI() override;
    void OnRender() override;
    void OnEndFrame() override;
    //
    void OnExit() override;
private:
    CEngine* Engine = nullptr;
    CEventManager EventManager;
    std::vector<std::unique_ptr<IEntityFactory>> EntityFactory;
    std::vector<std::unique_ptr<IComponentFactory>> ComponentFactory;
};
