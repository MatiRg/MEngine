#pragma once
#include "../Engine/EngineModule.hpp"
#include "Event.hpp" // Easier use

/**
  \class IEventQueue
  \brief Interface for Translating System Event to Engine Event, can also send custom events
*/
class IEventQueue: public IEngineModule
{
public:
    IEventQueue(): IEngineModule( "Event Queue" ) {}
    virtual ~IEventQueue() {}

    ENGINE_MODULE(IEventQueue)

    virtual void PullEvents() = 0;
    virtual bool PopEvent(CEvent&) = 0;
    //! Send New Event, Event is moved to internal Queue
    virtual bool PushEvent(CEvent&&) = 0;
};
