#pragma once
#include "../Core/Log.hpp"
#include "../Core/NonCopyable.hpp"
#include "../System//MemoryManager.hpp"
#include <string>

class CEvent;

#define UPDATABLE(X) virtual std::string GetType() const override { return #X; } \
                     static std::string GetTypeStatic() { return #X; }

class IUpdatable: public NonCopyableMovable
{
public:
    IUpdatable(const std::string& aName):
        Name( aName )
    {
        LOG( ESeverity::Debug ) << GetName() << " - Created\n";
        ADD_MEMORY_RECORD(this);
    }

    virtual ~IUpdatable()
    {
        LOG( ESeverity::Debug ) << GetName() << " - Destroyed\n";
        ERASE_MEMORY_RECORD(this);
    }

    virtual std::string GetType() const = 0;

    void SetName(const std::string& aName) { Name = aName; }
    const std::string& GetName() const { return Name; }

    void SetActive(const bool x) { Active = x; }
    bool IsActive() const { return Active; }

    virtual void OnInit() {}
    //
    virtual void OnEnter() {}
    //
    virtual void OnBeginFrame() {}
    virtual void OnEvent(const CEvent&) {}
    virtual void OnUpdate(const float) {}
    virtual void OnLateUpdate(const float) {}
    virtual void OnGUI() {}
    virtual void OnRender() {}
    virtual void OnEndFrame() {}
    //
    virtual void OnLeave() {}
    //
    virtual void OnExit() {}
private:
    std::string Name;
    bool Active = true;
};
