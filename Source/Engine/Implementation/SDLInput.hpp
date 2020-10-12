#pragma once
#include "../Input/Input.hpp"
#include <array>

class CSDLWindow;

class CSDLInput: public IInput
{
public:
    CSDLInput(CSDLWindow*);
    ~CSDLInput();

    bool Init(const SEngineParams&) override;
    void OnEvent(const CEvent&) override;

    void ClearInput() override;

    bool IsKeyDown(const EKey Key) const override { return KeyDown[static_cast<std::size_t>(Key)]; }
    bool IsKeyPressed(const EKey Key) const override { return KeyPressed[static_cast<std::size_t>(Key)]; }

    bool IsMouseKeyDown(const EMouseKey Key) const override { return MouseKeyDown[static_cast<std::size_t>(Key)]; }
    bool IsMouseKeyPressed(const EMouseKey Key) const override { return MouseKeyPressed[static_cast<std::size_t>(Key)]; }
    void SetMousePosition(const Vector2&) override;
    const Vector2& GetMousePosition() const override { return MousePosition; }
    const Vector2& GetRelativeMouseMotion() const override { return RelativeMouseMotion; }
    const Vector2& GetMouseWheel() const override { return MouseWheel; }

    bool SetMouseMode(const EMouseMode) override;
    EMouseMode GetMouseMode() const override { return MouseMode; }
    void ShowCursor(const bool) override;
    bool IsCursorVisible() const override;

    const std::string& GetClipBoardText() const override;
    void SetClipBoardText(const std::string&) override;
private:
    CSDLWindow* Window = nullptr;
    std::array<bool, static_cast<std::size_t>(EKey::Max)> KeyDown;
    std::array<bool, static_cast<std::size_t>(EKey::Max)> KeyPressed;
    std::array<bool, static_cast<std::size_t>(EMouseKey::Max)> MouseKeyDown;
    std::array<bool, static_cast<std::size_t>(EMouseKey::Max)> MouseKeyPressed;
    Vector2 MousePosition;
    Vector2 RelativeMouseMotion;
    Vector2 MouseWheel;
    EMouseMode MouseMode = EMouseMode::Normal;
    mutable char* ClipSDLText = nullptr;
    mutable std::string ClipBoardText;
};


