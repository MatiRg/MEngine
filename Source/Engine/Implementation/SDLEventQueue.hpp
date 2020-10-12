#pragma once
#include "../Input/EventQueue.hpp"
#include <queue>

class CSDLEventQueue: public IEventQueue
{
public:
    CSDLEventQueue();
    ~CSDLEventQueue();
    bool Init(const SEngineParams&) override;
    void PullEvents() override;
    bool PopEvent(CEvent&) override;
    bool PushEvent(CEvent&&) override;
private:
    EKey ToKey(const int) const;
    EKeyMod ToKeyMod(const int) const;
    EMouseKey ToMouseKey(const int) const;
private:
    std::queue<CEvent> EventQueue;
};
