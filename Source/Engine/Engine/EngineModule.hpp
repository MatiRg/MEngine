#pragma once
#include "EngineParams.hpp"
#include "Updatable.hpp"
#include <memory>

#define ENGINE_MODULE(X) std::string GetType() const override final { return #X; } \
                         static std::string GetTypeStatic() { return #X; }

class IEngineModule: public IUpdatable
{
public:
    IEngineModule(const std::string& aName): 
        IUpdatable(aName) 
    {
    }

    virtual ~IEngineModule() 
    {
    }

    virtual bool Init(const SEngineParams&) { return true; }
    virtual void Exit() {}
};
