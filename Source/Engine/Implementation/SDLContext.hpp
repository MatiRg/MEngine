#pragma once
#include "../Engine/Context.hpp"
#include <memory>

class CSDLSystem;
class CSDLEventQueue;
class CSDLInput;
class CSDLWindow;
class CBulletPhysics3D;
class CBox2DPhysics2D;

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
    IPhysics3D* GetPhysics3D() const override;
    IPhysics2D* GetPhysics2D() const override;
private:
    std::unique_ptr<CSDLSystem> System;
    std::unique_ptr<CSDLEventQueue> EventQueue;
    std::unique_ptr<CSDLInput> Input;
    std::unique_ptr<CSDLWindow> Window;
    std::unique_ptr<IGraphics> Graphics;
    std::unique_ptr<IAudio> Audio;
    std::unique_ptr<CBulletPhysics3D> Physics3D;
    std::unique_ptr<CBox2DPhysics2D> Physics2D;
};
