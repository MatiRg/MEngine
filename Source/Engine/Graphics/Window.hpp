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
    
    virtual bool SetSize(const int, const int) = 0;
    virtual bool SetSize(const IntVector2&) = 0;
    virtual const IntVector2& GetSize() const  = 0;

    virtual float GetAspectRatio() const = 0;

    // Icon Name, CResources
    virtual bool SetIcon(const std::string&, CResources*) = 0;
};

