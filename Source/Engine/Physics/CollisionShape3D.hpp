#pragma once
#include "../Core/NonCopyable.hpp"

class ICollisionShape3D: public NonCopyableMovable
{
public:
    ICollisionShape3D();
    virtual ~ICollisionShape3D();

    void SetUserData(void* Data) { UserData = Data; }
    void* GetUserData() const { return UserData; }
    template<class T>
    T* GetUserDataAs() const { return static_cast<T*>(UserData); }
protected:
    void* UserData = nullptr;
};
