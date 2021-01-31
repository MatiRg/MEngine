#pragma once
#include "../Component.hpp"
#include"../../Physics/RigidBody3D.hpp"

class CPhysicsWorld3D;

class CRigidBody3D final: public IComponent
{
public:
    CRigidBody3D(CEngine* aEngine);
    ~CRigidBody3D();

    COMPONENT(CRigidBody3D)

    void OnCreate() override;

    bool OnLoad(CXMLElement*) override;
    bool OnSave(CXMLElement*) override;

    void OnEnabled() override;
    void OnDisabled() override;

    void OnBeginFrame() override;

    IRigidBody3D* GetBody() const { return Body; }
private:
    CPhysicsWorld3D* PhysicsWorld3D = nullptr;
    IRigidBody3D* Body = nullptr;
};