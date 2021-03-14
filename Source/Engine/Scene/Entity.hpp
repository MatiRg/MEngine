#pragma once
#include "../Core/NonCopyable.hpp"
#include "../Physics/Contact2D.hpp"
#include "../Physics/Contact3D.hpp"
#include "../Math/Transform.hpp"
#include "EventManager.hpp"
#include "Event.hpp"
#include "Component.hpp"
#include <string>
#include <memory>
#include <algorithm>

class CEngine;
class CImGUI;
class CXMLElement;
class CWorld;
class CEntity;

using EntityArray = std::vector<CEntity*>;

static constexpr int WORLD_ENTITY = -1;
static constexpr int INVALID_ENTITY = -1000;

/**
 * \brief Define in CEntity children class for reflection
 * \param x class name of CEntity type, must be base of CEntity
 */
#define ENTITY(x) virtual std::string GetType() const override { return #x; } \
                  static std::string GetTypeStatic() { return #x; }

// 2D

class CRigidBody2D;
class CBoxCollider2D;

struct SEntityCollision2D
{
    SEntityCollision2D(CRigidBody2D* aRigidBody, CBoxCollider2D* aCollider):
        RigidBody(aRigidBody),
        Collider(aCollider)
    {
    }

    CRigidBody2D* RigidBody = nullptr;
    CBoxCollider2D* Collider = nullptr;
};

// 3D

class CMeshNode;
class CRigidBody3D;
class CMaterial;

struct SEntityCollision3D
{
    SEntityCollision3D(CRigidBody3D* aRigidBody, CEntity* aEntity, const std::vector<SContactPoint3D>& aContacts):
        RigidBody(aRigidBody),
        Entity(aEntity),
        Contacts(aContacts)
    {
    }

    CRigidBody3D* RigidBody = nullptr;
    CEntity* Entity = nullptr;
    std::vector<SContactPoint3D> Contacts;
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

/**
 * \class CEntity
 * \brief Base Class for Entities - Contains Components and optionaly user data and logic, 
 * in derrivied class ENTITY(CLASS_NAME) must be used.
 *
 */
class CEntity : public IEntity
{
    friend class CWorld;
public:
    //! All classes should have the same constructor
    CEntity(CEngine*);
    virtual ~CEntity();

    ENTITY(CEntity)
    
    //! Can be used to access Engine Modules
    CEngine* GetEngine() const { return Engine; }

    //! Get Entity ID - Unique to Entire World
    int GetID() const { return ID; }

    //! Set Entity Name - Uniqueness not required
    void SetName(const std::string& aName) { Name = aName; }
    //! Get Entity Name - Uniqueness not required
    const std::string& GetName() const { return Name; }
    //! Get Pure Display Name: Returns Name or <Annoymous> if no Name
    std::string GetDisplayName() const;
    //! Unique Namne ID for ImGUI
    std::string GetUniqueDisplayName() const;

    //! Set String Tag - can be used to group Entities
    void SetTag(const std::string& aTag) { Tag = aTag; }
    //! Get String Tag - can be used to group Entities
    const std::string& GetTag() const { return Tag; }

    //! Set World - Used Internally
    void SetWorld(CWorld* aWorld) { World = aWorld; }
    //! Gets World Pointer
    CWorld* GetWorld() const { return World; }
    //! Check if Entity is attached to CWorld
    bool IsAttachedToWorld() const { return World; }

    //! Reparent Entity to new Parent or to None at All. Also reparents CTransform to new Parent
    void SetParent(CEntity*);
    //! Gets parent Entity
    CEntity* GetParent() const { return Parent; }
    //! Check if Entity has Parent
    bool HasParent() const { return Parent; }

    //! Check if Entity is Updated and Rendered
    bool IsActive() const { return IsUpdated() && IsRendered(); }
    //! Sets whether Entity should be Updated and Rendered
    void SetActive(const bool Value)
    {
        SetUpdated(Value);
        SetRendered(Value);
    }

    //! Check if Entity is Updated, Controls OnGUI(), OnLateUpdate(), OnUpdate(), OnBeginFrame(), OnEndFrame()
    bool IsUpdated() const { return Updated; }
    //! Sets whether Entity should be Updated
    void SetUpdated(const bool);

    //! Called when Entity is Enabled
    virtual void OnEnabled() {}
    //! Called when Entity is Disabled
    virtual void OnDisabled() {}

    //! Check if Entity is Rendered, Controls OnRender()
    bool IsRendered() const { return Rendered; }
    //! Sets whether Entity should be Rendered
    void SetRendered(const bool);

    //! Get Transform
    CTransform& GetTransform() { return Transform; }
    //! Get Transform
    const CTransform& GetTransform() const { return Transform; }

    /**
     * \brief Create Component
     * \param Type type of Component must inherints from IComponent
     * \return Returns Pointer for created Component or nullptr if error
     */
    IComponent* CreateComponent(const std::string& Type);
    //
    /**
     * \brief Create Component
     * \tparam T type of Component must inherints from IComponent
     * \return Returns Pointer for created Component or nullptr if error
     */
    template<class T>
    T* CreateComponent()
    {
        static_assert(std::is_base_of<IComponent, T>::value, "Must be base of IComponent");
        return dynamic_cast<T*>(CreateComponent(T::GetTypeStatic()));
    }
    /**
     * \brief Check if given component exist
     * \param Type type of Component must inherints from IComponent
     * \return Returns true if component exist else returns false
     */
    bool HasComponent(const std::string& Type) const;
    /**
     * \brief Check if given component exist
     * \tparam T type of Component must inherints from IComponent
     * \return Returns true if component exist else returns false
     */
    template<class T>
    bool HasComponent() const
    {
        static_assert(std::is_base_of<IComponent, T>::value, "Must be base of IComponent");
        return HasComponent(T::GetTypeStatic());
    }
    /**
     * \brief Get component of given type
     * \param Type type of Component must inherints from IComponent
     * \return Returns Pointer for Component if it exist or nullptr if not
     */
    IComponent* GetComponent(const std::string& Type) const;
    /**
     * \brief Get component of given type
     * \tparam T type of Component must inherints from IComponent
     * \return Returns Pointer for Component if it exist or nullptr if not
     */
    template<class T>
    T* GetComponent() const
    {
        static_assert(std::is_base_of<IComponent, T>::value, "Must be base of IComponent");
        return dynamic_cast<T*>(GetComponent(T::GetTypeStatic()));
    }
    /**
     * \brief Get all components of given type
     * \param Type type of Component must inherints from IComponent
     * \return Returns collection of Component pointers
     */
    ComponentArray GetComponents(const std::string& Type) const;
    /**
     * \brief Get all components of given type
     * \tparam T type of Component must inherints from IComponent
     * \return Returns collection of Component pointers
     */
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
    //! Returns all components
    const ComponentArray& GetComponents() const { return Components; }
    //! Check if has any components
    bool HasComponents() const { return Components.size() > 0u; }
    //! Get Component Count
    std::size_t GetComponentCount() const { return Components.size(); }
    //! Static Function to Destroy Component
    static void Destroy(IComponent*);
    //! Destroy Given Component if it is hold in this Entity
    void DestroyComponent(IComponent*);
    /**
     * \brief Destroy Component of type
     * \param Type type of Component must inherints from IComponent
     */
    void DestroyComponent(const std::string& Type);
    /**
     * \brief Destroy Component of type
     * \tparam T type of Component must inherints from IComponent
     */
    template<class T>
    void DestroyComponent()
    {
        static_assert(std::is_base_of<IComponent, T>::value, "Must be base of IComponent");
        DestroyComponent(T::GetTypeStatic());
    }
    /**
     * \brief Destroy all Components of type
     * \param Type type of Component must inherints from IComponent
     */
    void DestroyComponents(const std::string& Type);
    /**
     * \brief Destroy all Components of type
     * \tparam T type of Component must inherints from IComponent
     */
    template<class T>
    void DestroyComponents()
    {
        static_assert(std::is_base_of<IComponent, T>::value, "Must be base of IComponent");
        DestroyComponents(T::GetTypeStatic());
    }
    //! Destroy all components
    void DestroyComponents();

    /**
     * \brief Create Model Child Entity
     * \param Type type of Entity must inherints from CEntity
     * \param ModelName Model Name
     * \param Overwrite Material, Ownerships is not taken
     * \return Returns Loaded Model Root Entity for given Model Name or nullptr if error
     */
    CEntity* CreateModel(const std::string& ModelName, const std::string& Type, CMaterial* = nullptr);
    /**
     * \brief Create Model Child Entity
     * \tparam T type of Entity must inherints from CEntity
     * \param ModelName Model Name
     * \param Overwrite Material, Ownerships is not taken
     * \return Returns Loaded Model Root Entity for given Model Name or nullptr if error
     */
    template<class T>
    T* CreateModel(const std::string& ModelName, CMaterial* Material = nullptr)
    {
        static_assert(std::is_base_of<CEntity, T>::value, "Must be base of CEntity");
        return dynamic_cast<T*>(CreateModel(ModelName, T::GetTypeStatic(), Material));
    }

    /**
     * \brief Create Child Entity
     * \param Type type of Entity must inherints from CEntity
     * \return Returns created Entity or nullptr if error
     */
    CEntity* CreateChild(const std::string& Type);
    /**
     * \brief Create Child Entity
     * \tparam T type of Entity must inherints from CEntity
     * \return Returns created Entity or nullptr if error
     */
    template<class T>
    T* CreateChild()
    {
        static_assert(std::is_base_of<CEntity, T>::value, "Must be base of CEntity");
        return dynamic_cast<T*>(CreateChild(T::GetTypeStatic()));
    }
    //! Get Child By ID
    CEntity* GetChild(const int) const;
    //! Get Child By ID, Casts to given type - type must inherints from CEntity
    template<class T>
    T* GetChild(const int ID) const
    {
        static_assert(std::is_base_of<CEntity, T>::value, "Must be base of CEntity");
        return dynamic_cast<T*>(GetChild(ID));
    }
    //! Get Child By Name
    CEntity* GetChild(const std::string&) const;
    //! Get Child By Name, Casts to given type - type must inherints from CEntity
    template<class T>
    T* GetChild(const std::string& Name) const
    {
        static_assert(std::is_base_of<CEntity, T>::value, "Must be base of CEntity");
        return dynamic_cast<T*>(GetChild(Name));
    }
    //! Get All children by Tag
    EntityArray GetChildren(const std::string&) const;
    //! Get All children by Tag, Casts to given type - type must inherints from CEntity
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
    //! Get All children as given type - type must inherints from CEntity
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
    //! Returns all children
    const EntityArray& GetChildren() const { return Children; }
    //! Check if has any children
    bool HasChildren() const { return Children.size() > 0u; }
    //! Get number of children
    std::size_t GetChildrenCount() const { return Children.size(); }
    //
    // TO DO: Make Recurse Destruction to find child in parents
    //! Destroy self - Unsafe - Do not use
    void Destroy();
    //! Static Function to Destroy Entity
    static void Destroy(CEntity*);
    //! Destroy Given Child
    void DestroyChild(CEntity*);
    //! Destroy Given Child by ID
    void DestroyChild(const int);
    //! Destroy Given Child by Name
    void DestroyChild(const std::string&);
    //! Destroy All Children with Tag
    void DestroyChildren(const std::string&);
    //! Destroy All Children
    void DestroyChildren();
    //
    // Remove Child From This Node: TO DO

    //! Use to load Entity data from XMLElement
    virtual bool OnLoad(CXMLElement*) { return true; }
    //! Use to save Entity data to XMLElement
    virtual bool OnSave(CXMLElement*) { return true; }

    //! Called when Entity is created and added as Child - called only once but always when entity is created(after loaded from world file)
    virtual void OnCreate() {}
    //! Called Once before first Update Loop - called once is never called again even after world is loaded from file
    virtual void OnStart() {}
    //
    virtual void OnBeginFrame() {}
    virtual void OnUpdate(const float) {}
    virtual void OnLateUpdate(const float) {}
    //! Should contain only ImGUI commands 
    virtual void OnGUI() {}
    //! Should contain only render commands 
    virtual void OnRender() {}
    virtual void OnEndFrame() {}
    //! Called just before Entity destruction
    virtual void OnDestroy() {}
    // 2D
    virtual void OnCollisionEnter(const SEntityCollision2D&) {}
    virtual void OnCollisionLeave(const SEntityCollision2D&) {}
    // 3D
    virtual void OnCollisionEnter(const SEntityCollision3D&) {}
    virtual void OnCollisionStay(const SEntityCollision3D&) {}
    virtual void OnCollisionLeave(const SEntityCollision3D&) {}

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
    // Used Internally
    void SetID(const int aID) { ID = aID; }
    //
    static void ProcessMeshNode(CEntity*, CMeshNode*, CMaterial*);
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
    //
    void Start();
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