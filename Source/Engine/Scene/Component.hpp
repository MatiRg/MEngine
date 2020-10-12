#pragma once
#include "../Core/Log.hpp"
#include "../Core/NonCopyable.hpp"
#include <string>

class CEngine;
class CEntity2D;
class CRenderer2D;
class CXMLElement;

// Define in Component children class
#define COMPONENT(x) virtual std::string GetType() const override { return #x; } \
                     static std::string GetTypeStatic() { return #x; }

class IComponent: public NonCopyableMovable
{
public:
    IComponent(CEngine* aEngine);
    virtual ~IComponent();

    virtual std::string GetType() const = 0;

    void SetOwner(CEntity2D* aOwner) { Owner = aOwner; }
    CEntity2D* GetOwner() const { return Owner; }

    virtual bool OnLoad(CXMLElement*) { return true; }
    virtual bool OnSave(CXMLElement*) { return true; }

    virtual void OnEnabled() {}
    virtual void OnDisabled() {}

    virtual void OnCreate() {} // After Component is Created and added to Object
    //
    virtual void OnBeginFrame() {}
    virtual void OnUpdate(const float) {}
    virtual void OnLateUpdate(const float) {}
    virtual void OnGUI() {}
    virtual void OnRender() {}
    virtual void OnEndFrame() {}
    //
    virtual void OnDestroy() {}
protected:
    CEngine* Engine = nullptr;
    CEntity2D* Owner = nullptr;
};

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