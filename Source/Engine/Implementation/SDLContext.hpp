#pragma once
#include "../Engine/Context.hpp"
#include <memory>

class CSDLSystem;
class CSDLEventQueue;
class CSDLInput;
class CSDLWindow;

class CSDLContext: public IContext
{
public:
    CSDLContext();
    ~CSDLContext();

    bool Init(const SEngineParams&) override;

    ISystem* GetSystem() const override;
    IEventQueue* GetEventQueue() const override;
    IInput* GetInput() const override;
    IWindow* GetWindow() const override;
    IGraphics* GetGraphics() const override;
    IAudio* GetAudio() const override;
private:
    std::unique_ptr<CSDLSystem> System;
    std::unique_ptr<CSDLEventQueue> EventQueue;
    std::unique_ptr<CSDLInput> Input;
    std::unique_ptr<CSDLWindow> Window;
    std::unique_ptr<IGraphics> Graphics;
    std::unique_ptr<IAudio> Audio;
};
