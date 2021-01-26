#pragma once
#include "EngineModule.hpp"
#include <vector>
#include <unordered_Map>

/**
  \class CUserUpdater
  \brief Container class for all Game Updatables
*/
class CUserUpdater final: public IEngineModule
{
    using UpdatableArray = std::vector<IUpdatable*>;
public:
    CUserUpdater();
    ~CUserUpdater();

    ENGINE_MODULE(CUserUpdater)

    void OnInit() override;
    //
    void OnEnter() override;
    //
    void OnBeginFrame() override;
    void OnEvent(const CEvent&) override;
    void OnUpdate(const float) override;
    void OnLateUpdate(const float) override;
    void OnGUI() override;
    void OnRender() override;
    void OnEndFrame() override;
    //
    void OnLeave() override;
    //
    void OnExit() override;
    void Exit() override;

    /**
     * \brief Add Global Updatable, ownership is not taken
     * \param Ptr Updatable pointer
     */
    void AddUpdatable(IUpdatable* Ptr);
    /**
     * \brief Add Updatable to given Container, ownership is not taken
     * \param Container Container Name
     * \param Ptr Updatable pointer
     */
    void AddUpdatable(const std::string& Container, IUpdatable* Ptr);

    /**
     * \brief Get Updatable by Name
     * \param aName Name of Updatable
     * \return Return Pointer if found given Updatable or nullptr if not
     */
    IUpdatable* GetUpdatableByName(const std::string& aName) const;
    /**
     * \brief Get Updatable by Type Name
     * \param aType Name of the Class
     * \return Return Pointer if found given Updatable or nullptr if not
     */
    IUpdatable* GetUpdatableByType(const std::string& aType) const;
    /**
     * \brief Get Updatable by Template, class must be derrivied from IUpdatable
     * \return Return Pointer if found given Updatable or nullptr if not
     */
    template<class T>
    T* GetUpdatable() const
    {
        static_assert(std::is_base_of<IUpdatable, T>::value, "Must be base of IUpdatable");
        return dynamic_cast<T*>( GetUpdatableByType(T::GetTypeStatic()) );
    }

    /**
     * \brief Set active container
     * \param Container Container Name
     * \return Return true on success or false if not
     */
    bool SetContainer(const std::string& Container);
    /**
     * \brief Add new container
     * \param Container Container Name
     * \return Return true on success or false if not
     */
    bool AddContainer(const std::string& Container);
private:
    void ChangeContainer();
private:
    UpdatableArray Global;
    UpdatableArray* NewContainer = nullptr;
    UpdatableArray* ActiveContainer = nullptr;
    std::unordered_map<std::string, UpdatableArray> Containers;
};
