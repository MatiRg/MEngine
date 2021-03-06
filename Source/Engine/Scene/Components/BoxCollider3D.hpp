#pragma once
#include "../Component.hpp"
#include "../../Physics/CollisionShape3D.hpp"
#include "../../Math/Vector3.hpp"

class CPhysicsWorld3D;

class CBoxCollider3D final: public IComponent
{
public:
    CBoxCollider3D(CEngine* aEngine);
    ~CBoxCollider3D();

    COMPONENT(CBoxCollider3D)

    void OnCreate() override;

    bool OnLoad(CXMLElement*) override;
    bool OnSave(CXMLElement*) override;

    //! Half Extents
    void SetSize(const Vector3&);
    //! Half Extents
    const Vector3& GetSize() const { return Size; }

    ICollisionShape3D* GetShape() const { return Shape; }
private:
    CPhysicsWorld3D* PhysicsWorld3D = nullptr;
    ICollisionShape3D* Shape = nullptr;
    Vector3 Size;
};