#pragma once
#include "../Engine/EngineModule.hpp"
#include "../Math/Vector2.hpp"
#include "EventTypes.hpp"

enum class EMouseMode
{
    Normal,
    Relative
};

class IInput: public IEngineModule
{
public:
    IInput(): IEngineModule( "Input" ) {}
    virtual ~IInput() {}

    ENGINE_MODULE(IInput)

    void OnBeginFrame() override { ClearInput(); }

    virtual void ClearInput() = 0;

    virtual bool IsKeyDown(const EKey) const = 0;
    virtual bool IsKeyPressed(const EKey) const = 0;

    virtual bool IsMouseKeyDown(const EMouseKey) const = 0;
    virtual bool IsMouseKeyPressed(const EMouseKey) const = 0;
    virtual void SetMousePosition(const Vector2&) = 0;
    virtual const Vector2& GetMousePosition() const = 0;
    virtual const Vector2& GetRelativeMouseMotion() const = 0;
    virtual const Vector2& GetMouseWheel() const = 0;

    virtual bool SetMouseMode(const EMouseMode) = 0;
    virtual EMouseMode GetMouseMode() const = 0;
    virtual void ShowCursor(const bool) = 0;
    virtual bool IsCursorVisible() const = 0;

    virtual const std::string& GetClipBoardText() const = 0;
    virtual void SetClipBoardText(const std::string&) = 0;
};

