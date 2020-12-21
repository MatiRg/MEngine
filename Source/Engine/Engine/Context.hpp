#pragma once
#include "EngineParams.hpp"
#include "../Core/Log.hpp"
#include "../Core/NonCopyable.hpp"
#include "../System//MemoryManager.hpp"

class ISystem;
class IEventQueue;
class IInput;
class IWindow;
class IGraphics;
class IAudio;
class IPhysics3D;
class IPhysics2D;
class CConfig;

class IContext: public NonCopyableMovable
{
public:
    IContext()
    {
        LOG( ESeverity::Debug ) << "Context - Created\n";
        ADD_MEMORY_RECORD(this);
    }

    virtual ~IContext()
    {
        LOG( ESeverity::Debug ) << "Context - Destroyed\n";
        ERASE_MEMORY_RECORD(this);
    }

    virtual bool Init(const SEngineParams&) = 0;

    virtual ISystem* GetSystem() const = 0;
    virtual IEventQueue* GetEventQueue() const = 0;
    virtual IInput* GetInput() const = 0;
    virtual IWindow* GetWindow() const = 0;
    virtual IGraphics* GetGraphics() const = 0;
    virtual IAudio* GetAudio() const = 0;
    virtual IPhysics3D* GetPhysics3D() const = 0;
    virtual IPhysics2D* GetPhysics2D() const = 0;
};
