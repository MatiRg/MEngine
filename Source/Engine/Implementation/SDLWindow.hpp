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
    bool SetSize(const int x, const int y) override { return SetSize( IntVector2(x, y) ); }
    bool SetSize(const IntVector2&) override;
    const IntVector2& GetSize() const override { return WindowSize; }

    float GetAspectRatio() const override;

    /**< Nazwa Icony, wskaznik do zasobow */
    bool SetIcon(const std::string&, CResources*) override;

    SDL_Window* GetSDLWindow() const { return Window; }
private:
    SDL_Window* Window = nullptr;
    std::string Caption;
    IntVector2 WindowSize;
};
