#pragma once
#include "EngineModule.hpp"
#include "../Core/NonCopyable.hpp"
#include <vector>
#include <unordered_map>
#include <string>
#include <type_traits>

class CConfig;

class CEngineUpdater final: public NonCopyableMovable
{
public:
    CEngineUpdater();
    ~CEngineUpdater();

    bool Init(const SEngineParams&);
    void OnInit();
    //
    void OnEnter();
    //
    void OnBeginFrame();
    void OnEvent(const CEvent&);
    void OnUpdate(const float);
    void OnLateUpdate(const float);
    void OnGUI();
    void OnRender();
    void OnEndFrame();
    //
    void OnLeave();
    //
    void OnExit();
    void Exit();

    bool AddEngineModule(IEngineModule*);

    // Type
    IEngineModule* GetEngineModule(const std::string&) const;
    // Type
    template<class T>
    T* GetEngineModule() const
    {
        static_assert(std::is_base_of<IEngineModule, T>::value, "Must be base of IEngineModule");
        return dynamic_cast<T*>( GetEngineModule(T::GetTypeStatic()) );
    }
private:
    std::vector<IEngineModule*> Modules;
};
