#pragma once
#include "../Engine/EngineModule.hpp"
#include "../Math/Vector2.hpp"
#include <string>

class CResources;

class IWindow: public IEngineModule
{
public:
    IWindow(): IEngineModule( "Window" ) {}
    virtual ~IWindow() {}

    ENGINE_MODULE(IWindow)

    virtual bool SetCaption(const std::string&) = 0;
    virtual const std::string& GetCaption() const  = 0;
    
    virtual bool SetWindowSize(const int, const int) = 0;
    virtual bool SetWindowSize(const IntVector2&) = 0;
    virtual const IntVector2& GetWindowSize() const  = 0;

    // Icon Name, CResources
    virtual bool SetIcon(const std::string&, CResources*) = 0;
};

