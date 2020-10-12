#pragma once
#include "../Scene/Component.hpp"
#include "../Math/Vector2.hpp"
#include <box2d/box2d.h>

class CPhysicsWorld2D final: public IComponent
{
    class CContactListener: public b2ContactListener
    {
    public:
        void BeginContact(b2Contact*);
        void EndContact(b2Contact*);
    };
public:
    CPhysicsWorld2D(CEngine* aEngine);
    ~CPhysicsWorld2D();

    COMPONENT(CPhysicsWorld2D)

    bool OnLoad(CXMLElement*) override;
    bool OnSave(CXMLElement*) override;

    void OnRender() override;

    void SetGravity(const Vector2&);
    const Vector2& GetGravity() const { return Gravity; }

    void SetDebugDraw(const bool Value) { DebugDraw = Value; }

    b2World& GetWorld() { return WorldBox2D; }
private:
    Vector2 Gravity;
    CContactListener Listener;
    b2World WorldBox2D;
    bool DebugDraw = false;
};
