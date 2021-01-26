#pragma once
#include "EngineModule.hpp"
#include "../Core/NonCopyable.hpp"
#include <vector>
#include <unordered_map>
#include <string>
#include <type_traits>

class CConfig;

/**
  \class CEngineUpdater
  \brief Container class for all Engine Modules
*/
class CEngineUpdater final: public NonCopyableMovable
{
public:
    CEngineUpdater();
    ~CEngineUpdater();

    //! Called By CEngine
    bool Init(const SEngineParams&);
    //! Called By CEngine
    void OnInit();
    //
    //! Called By CEngine
    void OnEnter();
    //
    //! Called By CEngine
    void OnBeginFrame();
    //! Called By CEngine
    void OnEvent(const CEvent&);
    //! Called By CEngine
    void OnUpdate(const float);
    //! Called By CEngine
    void OnLateUpdate(const float);
    //! Called By CEngine
    void OnGUI();
    //! Called By CEngine
    void OnRender();
    //! Called By CEngine
    void OnEndFrame();
    //
    //! Called By CEngine
    void OnLeave();
    //
    //! Called By CEngine
    void OnExit();
    //! Called By CEngine
    void Exit();

    /**
     * \brief Add Engine Module, ownership is not taken
     * \param Ptr Engine Module pointer
     * \return Return true on success or false if not
     */
    bool AddEngineModule(IEngineModule* Ptr);

    /**
     * \brief Get Engine Module by Type Name
     * \param aType Name of the Class
     * \return Return Pointer if found given module or nullptr if not
     */
    IEngineModule* GetEngineModule(const std::string&) const;
    /**
     * \brief Get Engine Module by Template, class must be derrivied from IEngineModule
     * \return Return Pointer if found given module or nullptr if not
     */
    template<class T>
    T* GetEngineModule() const
    {
        static_assert(std::is_base_of<IEngineModule, T>::value, "Must be base of IEngineModule");
        return dynamic_cast<T*>( GetEngineModule(T::GetTypeStatic()) );
    }
private:
    std::vector<IEngineModule*> Modules;
};
