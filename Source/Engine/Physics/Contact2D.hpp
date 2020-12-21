#pragma once

class IRigidBody2D;
class ICollisionShape2D;

struct SContact2D
{
    SContact2D(IRigidBody2D* aRigidBody, ICollisionShape2D* aCollider):
        RigidBody(aRigidBody),
        Collider(aCollider)
    {
    }

    IRigidBody2D* RigidBody = nullptr;
    ICollisionShape2D* Collider = nullptr;
};