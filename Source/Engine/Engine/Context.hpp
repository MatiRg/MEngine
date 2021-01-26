#pragma once
#include "EngineParams.hpp"
#include "../Core/Log.hpp"
#include "../Core/NonCopyable.hpp"
#include "../System//MemoryManager.hpp"

class ISystem;
class IEventQueue;
class IInput;
class IWindow;
class IGraphics;
class IAudio;
class IPhysics3D;
class IPhysics2D;
class CConfig;

/**
  \class IContext
  \brief Interface descripting Lower Level Modules Creation
*/
class IContext: public NonCopyableMovable
{
public:
    IContext()
    {
        LOG( ESeverity::Debug ) << "Context - Created\n";
        ADD_MEMORY_RECORD(this);
    }

    virtual ~IContext()
    {
        LOG( ESeverity::Debug ) << "Context - Destroyed\n";
        ERASE_MEMORY_RECORD(this);
    }

    /**
     * \brief Create Lower level Modules
     * \param aParams Engine Creation Parameters
     * \return Return true on success or false if not
     */
    virtual bool Init(const SEngineParams& aParams) = 0;

    //! Get Engine Module
    virtual ISystem* GetSystem() const = 0;
    //! Get Engine Module
    virtual IEventQueue* GetEventQueue() const = 0;
    //! Get Engine Module
    virtual IInput* GetInput() const = 0;
    //! Get Engine Module
    virtual IWindow* GetWindow() const = 0;
    //! Get Engine Module
    virtual IGraphics* GetGraphics() const = 0;
    //! Get Engine Module
    virtual IAudio* GetAudio() const = 0;
    //! Get Engine Module
    virtual IPhysics3D* GetPhysics3D() const = 0;
    //! Get Engine Module
    virtual IPhysics2D* GetPhysics2D() const = 0;
};
