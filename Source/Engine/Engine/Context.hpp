#pragma once
#include "../Core/Log.hpp"
#include "../Core/NonCopyable.hpp"
#include "EngineParams.hpp"

class ISystem;
class IEventQueue;
class IInput;
class IWindow;
class IGraphics;
class IAudio;

class IContext: public NonCopyableMovable
{
public:
    IContext()
    {
        LOG( ESeverity::Debug ) << "Context - Created\n";
    }

    virtual ~IContext()
    {
        LOG( ESeverity::Debug ) << "Context - Destroyed\n";
    }

    virtual bool Init(const SEngineParams&) = 0;

    virtual ISystem* GetSystem() const = 0;
    virtual IEventQueue* GetEventQueue() const = 0;
    virtual IInput* GetInput() const = 0;
    virtual IWindow* GetWindow() const = 0;
    virtual IGraphics* GetGraphics() const = 0;
    virtual IAudio* GetAudio() const = 0;
};
