#pragma once
#include "Entity.hpp"
#include "Component.hpp"
#include "Event.hpp"
#include "EventManager.hpp"
#include "../Engine/EngineModule.hpp"
#include <vector>

class CEngine;

/**
  \class CScene
  \brief Class for registering and creating Components and Entities classes
*/
class CScene final: public IEngineModule
{
public:
    CScene(CEngine*);
    ~CScene();

    ENGINE_MODULE(CScene)

    CEventManager* GetEventManager() { return &EventManager; }

    /**
     * \brief Register Entity
     * \tparam T type of entity must inherints from CEntity
     */
    template<class T>
    void RegisterEntityFactory()
    {
        static_assert(std::is_base_of<CEntity, T>::value, "Must be base of CEntity");
        EntityFactory.push_back( std::make_unique<TEntityFactory<T>>(Engine) );
    }
    /**
     * \brief Create Entity
     * \param Type type of entity must inherints from CEntity
     * \return Returns Pointer for created Entity or nullptr if error.Pointer must be deleted by user.
     */
    CEntity* CreateEntity(const std::string& Type);
    /**
     * \brief Create Entity
     * \tparam Type of entity must inherints from CEntity
     * \return Returns Pointer for created Entity or nullptr if error.Pointer must be deleted by user.
     */
    template<class T>
    T* CreateEntity()
    {
        static_assert(std::is_base_of<CEntity, T>::value, "Must be base of CEntity");
        return dynamic_cast<T*>(CreateEntity(T::GetTypeStatic()));
    }
    //

    /**
     * \brief Register Component
     * \tparam T type of component must inherints from IComponent
     */
    template<class T>
    void RegisterComponentFactory()
    {
        static_assert(std::is_base_of<IComponent, T>::value, "Must be base of IComponent");
        ComponentFactory.push_back( std::make_unique<TComponentFactory<T>>(Engine) );
    }
    /**
     * \brief Create Component
     * \param Type type of component must inherints from IComponent
     * \return Returns Pointer for created component or nullptr if error.Pointer must be deleted by user.
     */
    IComponent* CreateComponent(const std::string&);
    /**
     * \brief Create Component
     * \tparam T type of component must inherints from IComponent
     * \return Returns Pointer for created component or nullptr if error.Pointer must be deleted by user.
     */
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
