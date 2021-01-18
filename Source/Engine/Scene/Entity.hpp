#pragma once
#include "../Core/NonCopyable.hpp"
#include "../Physics/Contact2D.hpp"
#include "EventManager.hpp"
#include "Event.hpp"
#include "Component.hpp"
#include "Transform.hpp"
#include <string>
#include <memory>
#include <algorithm>

class CEngine;
class CXMLElement;
class CWorld;
class CEntity;

using EntityArray = std::vector<CEntity*>;

static constexpr int WORLD_ENTITY = -1;
static constexpr int INVALID_ENTITY = -1000;

#define ENTITY(x) virtual std::string GetType() const override { return #x; } \
                  static std::string GetTypeStatic() { return #x; }

// 2D

class CRigidBody2D;
class CBoxCollider2D;

// 3D
class CMeshNode;

struct SEntityCollision2D
{
    SEntityCollision2D(CRigidBody2D* aRigidBody, CBoxCollider2D* aCollider) :
        RigidBody(aRigidBody),
        Collider(aCollider)
    {
    }

    CRigidBody2D* RigidBody = nullptr;
    CBoxCollider2D* Collider = nullptr;
};

// 
class IEntity : public NonCopyableMovable
{
public:
    IEntity() = default;
    virtual ~IEntity() = default;

    // Type Support
    virtual std::string GetType() const = 0;
};

// Base Class
class CEntity : public IEntity
{
public:
    CEntity(CEngine*);
    virtual ~CEntity();

    ENTITY(CEntity)
    
    CEngine* GetEngine() const { return Engine; }

    void SetID(const int aID) { ID = aID; }
    int GetID() const { return ID; }

    void SetName(const std::string& aName) { Name = aName; }
    const std::string& GetName() const { return Name; }

    void SetTag(const std::string& aTag) { Tag = aTag; }
    const std::string& GetTag() const { return Tag; }

    void SetWorld(CWorld* aWorld) { World = aWorld; }
    CWorld* GetWorld() const { return World; }
    bool IsAttachedToWorld() const { return World; }

    void SetParent(CEntity*);
    CEntity* GetParent() const { return Parent; }
    bool HasParent() const { return Parent; }

    bool IsActive() const { return IsUpdated() && IsRendered(); }
    void SetActive(const bool Value)
    {
        SetUpdated(Value);
        SetRendered(Value);
    }

    // Controls OnGUI(), OnLateUpdate(), OnUpdate(), OnBeginFrame(), OnEndFrame()
    bool IsUpdated() const { return Updated; }
    void SetUpdated(const bool);

    virtual void OnEnabled() {}
    virtual void OnDisabled() {}

    // Controls OnRender()
    bool IsRendered() const { return Rendered; }
    void SetRendered(const bool);

    void SetTransform(const CTransform& aTransform) { Transform = aTransform; }
    CTransform& GetTransform() { return Transform; }
    const CTransform& GetTransform() const { return Transform; }

    IComponent* CreateComponent(const std::string&);
    //
    template<class T>
    T* CreateComponent()
    {
        static_assert(std::is_base_of<IComponent, T>::value, "Must be base of IComponent");
        return dynamic_cast<T*>(CreateComponent(T::GetTypeStatic()));
    }
    //
    bool HasComponent(const std::string&) const;
    //
    template<class T>
    bool HasComponent() const
    {
        static_assert(std::is_base_of<IComponent, T>::value, "Must be base of IComponent");
        return HasComponent(T::GetTypeStatic());
    }
    //
    IComponent* GetComponent(const std::string&) const;
    //
    template<class T>
    T* GetComponent() const
    {
        static_assert(std::is_base_of<IComponent, T>::value, "Must be base of IComponent");
        return dynamic_cast<T*>(GetComponent(T::GetTypeStatic()));
    }
    //
    ComponentArray GetComponents(const std::string&) const;
    //
    template<class T>
    std::vector<T*> GetComponents() const
    {
        static_assert(std::is_base_of<IComponent, T>::value, "Must be base of IComponent");
        std::vector<T*> Array;
        for (const auto& i : Components)
        {
            if (i->GetType() == T::GetTypeStatic())
            {
                Array.push_back(dynamic_cast<T*>(i));
            }
        }
        return Array;
    }
    //
    const ComponentArray& GetComponents() const { return Components; }
    //
    bool HasComponents() const { return Components.size() > 0u; }
    //
    std::size_t GetComponentCount() const { return Components.size(); }
    //
    static void Destroy(IComponent*);
    //
    void DestroyComponent(IComponent*);
    //
    void DestroyComponent(const std::string&);
    //
    template<class T>
    void DestroyComponent()
    {
        static_assert(std::is_base_of<IComponent, T>::value, "Must be base of IComponent");
        DestroyComponent(T::GetTypeStatic());
    }
    //
    void DestroyComponents(const std::string&);
    //
    template<class T>
    void DestroyComponents()
    {
        static_assert(std::is_base_of<IComponent, T>::value, "Must be base of IComponent");
        DestroyComponents(T::GetTypeStatic());
    }
    //
    void DestroyComponents();

    // Model Name; Returns Loaded Model Root, Type
    CEntity* CreateModel(const std::string&, const std::string&);
    //
    template<class T>
    T* CreateModel(const std::string& ModelName)
    {
        static_assert(std::is_base_of<CEntity, T>::value, "Must be base of CEntity");
        return dynamic_cast<T*>(CreateModel(ModelName, T::GetTypeStatic()));
    }

    CEntity* CreateChild(const std::string&);
    //
    template<class T>
    T* CreateChild()
    {
        static_assert(std::is_base_of<CEntity, T>::value, "Must be base of CEntity");
        return dynamic_cast<T*>(CreateChild(T::GetTypeStatic()));
    }
    // Get Child By ID
    CEntity* GetChild(const int) const;
    // Get Child By ID
    template<class T>
    T* GetChild(const int ID) const
    {
        static_assert(std::is_base_of<CEntity, T>::value, "Must be base of CEntity");
        return dynamic_cast<T*>(GetChild(ID));
    }
    // Get Child By Name
    CEntity* GetChild(const std::string&) const;
    //
    template<class T>
    T* GetChild(const std::string& Name) const
    {
        static_assert(std::is_base_of<CEntity, T>::value, "Must be base of CEntity");
        return dynamic_cast<T*>(GetChild(Name));
    }
    // By Tag
    EntityArray GetChildren(const std::string&) const;
    // By Tag
    template<class T>
    std::vector<T*> GetChildren(const std::string& Tag) const
    {
        static_assert(std::is_base_of<CEntity, T>::value, "Must be base of CEntity");
        std::vector<T*> Array;
        for (const auto& i : Children)
        {
            if (i->GetTag() == Tag)
            {
                auto Cast = dynamic_cast<T*>(i);
                if (Cast)
                {
                    Array.push_back(Cast);
                }
            }
        }
        return Array;
    }
    //
    template<class T>
    std::vector<T*> GetChildrenAs() const
    {
        static_assert(std::is_base_of<CEntity, T>::value, "Must be base of CEntity");
        std::vector<T*> Array;
        for (const auto& i : Children)
        {
            auto Cast = dynamic_cast<T*>(i);
            if (Cast)
            {
                Array.push_back(Cast);
            }
        }
        return Array;
    }
    //
    const EntityArray& GetChildren() const { return Children; }
    //
    bool HasChildren() const { return Children.size() > 0u; }
    //
    std::size_t GetChildrenCount() const { return Children.size(); }
    //
    // TO DO: Make Recurse Destruction to find child in parents
    //
    void Destroy();
    // Entity
    static void Destroy(CEntity*);
    // Child
    void DestroyChild(CEntity*);
    // By ID
    void DestroyChild(const int);
    // By Name
    void DestroyChild(const std::string&);
    // Destroy Children By Tag
    void DestroyChildren(const std::string&);
    //
    void DestroyChildren();
    //
    // Remove Child From This Node: TO DO

    virtual bool OnLoad(CXMLElement*) { return true; }
    virtual bool OnSave(CXMLElement*) { return true; }

    virtual void OnCreate() {} // Entity Init Code
    virtual void OnStart() {} // Called Once before first Update
    //
    virtual void OnBeginFrame() {}
    virtual void OnUpdate(const float) {}
    virtual void OnLateUpdate(const float) {}
    virtual void OnGUI() {}
    virtual void OnRender() {}
    virtual void OnEndFrame() {}
    //
    virtual void OnDestroy() {}
    // 2D
    virtual void OnCollisionEnter(const SEntityCollision2D&) {}
    virtual void OnCollisionLeave(const SEntityCollision2D&) {}
    //

    template<class E, class F>
    void Subscribe(const F& aFunction)
    {
        EventManager->Subscribe<E>(this, aFunction);
    }
    // UnSubscribe From all Events
    void UnSubscribe();
protected:
    bool Load(CXMLElement*);
    bool Save(CXMLElement*);
private:
    static void ProcessMeshNode(CEntity*, CMeshNode*);
    //
    void BeginFrame(const EBeginFrame&);
    void Update(const EUpdate&);
    void LateUpdate(const ELateUpdate&);
    void GUI(const EGUI&);
    void Render(const ERender&);
    void EndFrame(const EEndFrame&);
    //
    CEntity* RemoveChild(CEntity*);
    void InsertChild(CEntity*);
    //
    static void DestroyEntityRaw(CEntity*);
    void DestroyChildrenRaw();
    //
    static void DestroyComponentRaw(IComponent*);
    void DestroyComponentsRaw();
protected:
    CEngine* Engine = nullptr;
    CEventManager* EventManager = nullptr;
    int ID = -1; // -1 Means World Object
    std::string Name; // Uniqueness not required
    std::string Tag;
    CWorld* World = nullptr; // Default nullptr
    CEntity* Parent = nullptr; // Default nullptr
    CTransform Transform;
private:
    bool Updated = true;
    bool Rendered = true;
    EntityArray Children;
    ComponentArray Components;
    bool Started = false;
    EntityArray ToEntitiesRemove;
    ComponentArray ToComponentsRemove;
};

class IEntityFactory : public NonCopyableMovable
{
public:
    IEntityFactory() = default;
    virtual ~IEntityFactory() = default;

    virtual std::string GetType() const = 0;
    virtual CEntity* CreateEntity() const = 0;
};

template<class T>
class TEntityFactory final : public IEntityFactory
{
    static_assert(std::is_base_of<CEntity, T>::value, "Must be base of CEntity");
public:
    TEntityFactory(CEngine* aEngine) :
        Engine(aEngine)
    {
        LOG(ESeverity::Debug) << "Created Entity Factory - " << GetType() << "\n";
    }

    ~TEntityFactory()
    {
        LOG(ESeverity::Debug) << "Destroyed Entity Factory - " << GetType() << "\n";
    }

    std::string GetType() const override { return T::GetTypeStatic(); }
    CEntity* CreateEntity() const override { return new T(Engine); }
private:
    CEngine* Engine = nullptr;
};