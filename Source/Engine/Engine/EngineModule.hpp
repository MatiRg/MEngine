#pragma once
#include "EngineParams.hpp"
#include "Updatable.hpp"
#include <memory>

/**
 * \brief Macro used for setting up primitive reflection for IEngineModule
 * \param X Class Name that is derrivied from IEngineModule
 */
#define ENGINE_MODULE(X) std::string GetType() const override final { return #X; } \
                         static std::string GetTypeStatic() { return #X; }

class CConfig;

/**
  \class IEngineModule
  \brief Interface for Engine modules, in derrivied class ENGINE_MODULE(CLASS_NAME) must be used.
*/
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

    //! Init Engine Module
    virtual bool Init(const SEngineParams&) { return true; }
    //! Shutdown Engine Module
    virtual void Exit() {}
};
