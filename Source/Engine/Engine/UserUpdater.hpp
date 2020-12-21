#pragma once
#include "EngineModule.hpp"
#include <vector>
#include <unordered_Map>

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

    // Ownership not taken
    void AddUpdatable(IUpdatable*);
    // Same Updatables can be push to many Containers, Ownership not taken
    void AddUpdatable(const std::string&, IUpdatable*);

    IUpdatable* GetUpdatableByName(const std::string&) const;
    //
    IUpdatable* GetUpdatableByType(const std::string&) const;
    // By Type
    template<class T>
    T* GetUpdatable() const
    {
        static_assert(std::is_base_of<IUpdatable, T>::value, "Must be base of IUpdatable");
        return dynamic_cast<T*>( GetUpdatableByType(T::GetTypeStatic()) );
    }

    bool SetContainer(const std::string&);
    bool AddContainer(const std::string&);
private:
    void ChangeContainer();
private:
    UpdatableArray Global;
    UpdatableArray* NewContainer = nullptr;
    UpdatableArray* ActiveContainer = nullptr;
    std::unordered_map<std::string, UpdatableArray> Containers;
};
