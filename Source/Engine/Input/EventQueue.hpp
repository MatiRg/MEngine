#pragma once
#include "../Engine/EngineModule.hpp"
#include "Event.hpp" // Easier use

class IEventQueue: public IEngineModule
{
public:
    IEventQueue(): IEngineModule( "Event Queue" ) {}
    virtual ~IEventQueue() {}

    ENGINE_MODULE(IEventQueue)

    virtual void PullEvents() = 0;
    virtual bool PopEvent(CEvent&) = 0;
    virtual bool PushEvent(CEvent&&) = 0;
};
