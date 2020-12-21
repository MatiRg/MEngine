#pragma once
#include "../Component.hpp"
#include "../../Physics/PhysicsWorld2D.hpp"
#include <memory>

// Should be always attached to World(Root) Object
class CPhysicsWorld2D final: public IComponent
{
public:
    CPhysicsWorld2D(CEngine* aEngine);
    ~CPhysicsWorld2D();

    COMPONENT(CPhysicsWorld2D)

    void OnCreate() override;

    bool OnLoad(CXMLElement*) override;
    bool OnSave(CXMLElement*) override;

    void OnUpdate(const float) override;
    void OnRender() override;

    void SetGravity(const Vector2&);
    const Vector2& GetGravity() const;

    void SetDebugDraw(const bool Value) { DebugDraw = Value; }

    IPhysicsWorld2D* GetWorld() { return World.get(); }
private:
    std::unique_ptr<IPhysicsWorld2D> World;
    bool DebugDraw = false;
};
