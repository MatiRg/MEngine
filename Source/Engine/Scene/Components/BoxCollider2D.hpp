#pragma once
#include "../Component.hpp"
#include "../../Physics/CollisionShape2D.hpp"

class CPhysicsWorld2D;

class CBoxCollider2D final: public IComponent
{
public:
    CBoxCollider2D(CEngine* aEngine);
    ~CBoxCollider2D();

    COMPONENT(CBoxCollider2D)

    void OnCreate() override;

    bool OnLoad(CXMLElement*) override;
    bool OnSave(CXMLElement*) override;

    void SetFriction(const float);
    float GetFriction() const;

    void SetRestitution(const float);
    float GetRestitution() const;

    void SetDensity(const float);
    float GetDensity() const;

    void SetSize(const Vector2&);
    Vector2 GetSize() const;

    void SetOffset(const Vector2&);
    Vector2 GetOffset() const;

    void SetSensor(const bool);
    bool GetSensor() const;

    ICollisionShape2D* GetShape() const { return Shape; }
private:
    CPhysicsWorld2D* World = nullptr;
    ICollisionShape2D* Shape = nullptr;
};