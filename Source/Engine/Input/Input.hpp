#pragma once
#include "../Engine/EngineModule.hpp"
#include "../Math/Vector2.hpp"
#include "EventTypes.hpp"

//! Mouse mode type
enum class EMouseMode
{
    Normal,
    Relative
};

/**
  \class IInput
  \brief Interface for Input Checking like Mouse and Keyboard
*/
class IInput: public IEngineModule
{
public:
    IInput(): IEngineModule( "Input" ) {}
    virtual ~IInput() {}

    ENGINE_MODULE(IInput)

    void OnBeginFrame() override { ClearInput(); }

    virtual void ClearInput() = 0;

    //! Returns true if given keyboard key is down
    virtual bool IsKeyDown(const EKey) const = 0;
    //! Returns true if given keyboard key is pressed
    virtual bool IsKeyPressed(const EKey) const = 0;

    //! Returns true if given mouse key is down
    virtual bool IsMouseKeyDown(const EMouseKey) const = 0;
    //! Returns true if given mouse key is pressed
    virtual bool IsMouseKeyPressed(const EMouseKey) const = 0;
    //! Sets new mouse position
    virtual void SetMousePosition(const Vector2&) = 0;
    //! Returns mouse position
    virtual const Vector2& GetMousePosition() const = 0;
    //! Returns relative moues motion
    virtual const Vector2& GetRelativeMouseMotion() const = 0;
    //! Returns Mouse wheel motion
    virtual const Vector2& GetMouseWheel() const = 0;

    //! Sets new mouse mode
    virtual bool SetMouseMode(const EMouseMode) = 0;
    //! Returns mouse mode
    virtual EMouseMode GetMouseMode() const = 0;
    //! Set if mouse cursor should be drawn
    virtual void ShowCursor(const bool) = 0;
    //! Returns true if mouse cursor is drawn
    virtual bool IsCursorVisible() const = 0;

    //! Returns actual clipboard text
    virtual const std::string& GetClipBoardText() const = 0;
    //! Sets actual clipboard text
    virtual void SetClipBoardText(const std::string&) = 0;
};

