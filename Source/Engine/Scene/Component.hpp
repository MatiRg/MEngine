#pragma once
#include "../Core/Log.hpp"
#include "../Core/NonCopyable.hpp"
#include <string>

class CEngine;
class CEntity;
class CRenderer2D;
class CXMLElement;

/**
 * \brief Define in IComponent children class for reflection
 * \param x class name of IComponent type, must be base of IComponent
 */
#define COMPONENT(x) virtual std::string GetType() const override { return #x; } \
                     static std::string GetTypeStatic() { return #x; }

/**
  \class IComponent
  \brief Interface for Components - Contains Logic and Data, in derrivied class COMPONENT(CLASS_NAME) must be used.
*/
class IComponent: public NonCopyableMovable
{
public:
    //! All classes should have the same constructor
    IComponent(CEngine* aEngine);
    virtual ~IComponent();

    virtual std::string GetType() const = 0;

    CEngine* GetEngine() const { return Engine; }

    void SetOwner(CEntity* aOwner) { Owner = aOwner; }
    //! Gets owning Entity
    CEntity* GetOwner() const { return Owner; }

    //! Use to load component data from XMLElement
    virtual bool OnLoad(CXMLElement*) { return true; }
    //! Use to save component data to XMLElement
    virtual bool OnSave(CXMLElement*) { return true; }

    //! Called when component is enabled
    virtual void OnEnabled() {}
    //! Called when component is disabled
    virtual void OnDisabled() {}

    //! Called when component is created and added to Entity - called only once
    virtual void OnCreate() {}
    //
    virtual void OnBeginFrame() {}
    virtual void OnUpdate(const float) {}
    virtual void OnLateUpdate(const float) {}
    //! Should contain only ImGUI commands 
    virtual void OnGUI() {}
    //! Should contain only render commands 
    virtual void OnRender() {}
    virtual void OnEndFrame() {}
    //
    //! Called just before component destruction
    virtual void OnDestroy() {}
protected:
    CEngine* Engine = nullptr;
    CEntity* Owner = nullptr;
};

using ComponentArray = std::vector<IComponent*>;

class IComponentFactory: public NonCopyableMovable
{
public:
    IComponentFactory() = default;
    virtual ~IComponentFactory() = default;

    virtual std::string GetType() const = 0; 
    virtual IComponent* CreateComponent() const = 0;
};

template<class T>
class TComponentFactory final: public IComponentFactory
{
    static_assert(std::is_base_of<IComponent, T>::value, "Must be base of IComponent");
public:
    TComponentFactory(CEngine* aEngine):
        Engine(aEngine)
    {
        LOG( ESeverity::Debug ) << "Created Component Factory - " << GetType() << "\n";
    }

    ~TComponentFactory()
    {
        LOG( ESeverity::Debug ) << "Destroyed Component Factory - " << GetType() << "\n";
    }

    std::string GetType() const override { return T::GetTypeStatic(); }
    IComponent* CreateComponent() const override { return new T(Engine); }
private:
    CEngine* Engine = nullptr;
};