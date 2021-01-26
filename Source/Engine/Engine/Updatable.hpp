#pragma once
#include "../Core/Log.hpp"
#include "../Core/NonCopyable.hpp"
#include "../System//MemoryManager.hpp"
#include <string>

class CEvent;

/**
 * \brief Macro used for setting up primitive reflection for IUpdatable
 * \param X Class Name that is derrivied from IUpdatable
 */
#define UPDATABLE(X) virtual std::string GetType() const override { return #X; } \
                     static std::string GetTypeStatic() { return #X; }

/**
  \class IUpdatable
  \brief Interface for Updatables - Game States, in derrivied class UPDATABLE(CLASS_NAME) must be used.
*/
class IUpdatable: public NonCopyableMovable
{
public:
    /**
     * \brief Construct IUpdatable with given name
     * \param aName IUpdatable Name
     */
    IUpdatable(const std::string& aName):
        Name( aName )
    {
        LOG( ESeverity::Debug ) << GetName() << " - Created\n";
        ADD_MEMORY_RECORD(this);
    }

    virtual ~IUpdatable()
    {
        LOG( ESeverity::Debug ) << GetName() << " - Destroyed\n";
        ERASE_MEMORY_RECORD(this);
    }

    virtual std::string GetType() const = 0;

    //! Set Name
    void SetName(const std::string& aName) { Name = aName; }
    //! Get Name
    const std::string& GetName() const { return Name; }

    //! Set Active - Engine Modules Can`t be deactivated
    void SetActive(const bool x) { Active = x; }
    //! Is Active - Engine Modules Can`t be deactivated
    bool IsActive() const { return Active; }

    //! Called only once, can be used to init something
    virtual void OnInit() {}
    //
    //! Called everytime Container holding this Updatable is actived
    virtual void OnEnter() {}
    //
    //! Called just after main loop cycle is started
    virtual void OnBeginFrame() {}
    /**
     * \brief Called for every sysem event
     * \param Event Event
     */
    virtual void OnEvent(const CEvent&) {}
    /**
     * \brief Can be called 0, 1 or many time during update
     * \param DT Delta Time
     */
    virtual void OnUpdate(const float) {}
    /**
     * \brief Called only once after main update
     * \param DT Delta Time
     */
    virtual void OnLateUpdate(const float) {}
    //! Used for ImGUI
    virtual void OnGUI() {}
    //! Called on start rendering, can be used to drwa something or add something to Renderer3D
    virtual void OnRender() {}
    //! Called just before main loop cycle is ended
    virtual void OnEndFrame() {}
    //
    //! Called everytime Container holding this Updatable is deactived
    virtual void OnLeave() {}
    //
    //! Called only once, can be used to shutdown something
    virtual void OnExit() {}
private:
    std::string Name;
    bool Active = true;
};
