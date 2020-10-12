#pragma once

class CRigidBody2D;
class CBoxCollider2D;

struct SContact2D
{
    SContact2D(CRigidBody2D* aRigidBody, CBoxCollider2D* aCollider):
        RigidBody(aRigidBody),
        Collider(aCollider)
    {
    }

    CRigidBody2D* RigidBody = nullptr;
    CBoxCollider2D* Collider = nullptr;
};