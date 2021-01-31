#pragma once
#include "../Component.hpp"
#include "../../Physics/PhysicsWorld3D.hpp"
#include <memory>

// Should be always attached to World(Root) Object
class CPhysicsWorld3D final: public IComponent
{
public:
    CPhysicsWorld3D(CEngine* aEngine);
    ~CPhysicsWorld3D();

    COMPONENT(CPhysicsWorld3D)

    void OnCreate() override;

    bool OnLoad(CXMLElement*) override;
    bool OnSave(CXMLElement*) override;

    void OnUpdate(const float) override;
    void OnRender() override;

    void SetGravity(const Vector3&);
    Vector3 GetGravity() const;

    void SetDebugDraw(const bool Value) { DebugDraw = Value; }

    IPhysicsWorld3D* GetWorld() { return World.get(); }
private:
    std::unique_ptr<IPhysicsWorld3D> World;
    bool DebugDraw = false;
};
