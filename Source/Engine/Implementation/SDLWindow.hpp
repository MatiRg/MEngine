#pragma once
#include "../Graphics/Window.hpp"

struct SDL_Window;

class CSDLWindow: public IWindow
{
public:
    CSDLWindow();
    ~CSDLWindow();

    bool Init(const SEngineParams&) override;

    bool SetCaption(const std::string&) override;
    const std::string& GetCaption() const override { return Caption; }
    bool SetWindowSize(const int x, const int y) override { return SetWindowSize( IntVector2(x, y) ); }
    bool SetWindowSize(const IntVector2&) override;
    const IntVector2& GetWindowSize() const override { return WindowSize; }

    /**< Nazwa Icony, wskaznik do zasobow */
    bool SetIcon(const std::string&, CResources*) override;

    SDL_Window* GetSDLWindow() const { return Window; }
private:
    SDL_Window* Window = nullptr;
    std::string Caption;
    IntVector2 WindowSize;
};
