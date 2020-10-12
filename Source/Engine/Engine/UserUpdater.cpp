#include "UserUpdater.hpp"
#include "../Core/Log.hpp"
#include <algorithm>

CUserUpdater::CUserUpdater():
    IEngineModule( "User Updater" )
{
}

CUserUpdater::~CUserUpdater()
{
}

void CUserUpdater::OnInit()
{
    for(const auto &i: Global)
    {
        i->OnInit();
    }
    for(const auto &i: Containers)
    {
        for(const auto &j: i.second)
        {
            j->OnInit();
        }
    }
}

void CUserUpdater::OnEnter()
{
    for(const auto &i: Global)
    {
        i->OnEnter();
    }
}

void CUserUpdater::OnBeginFrame()
{
    ChangeContainer();
    //
    for(const auto &i: Global)
    {
        if( i->IsActive() )
        {
            i->OnBeginFrame();
        }
    }
    if( ActiveContainer )
    {
        for(const auto &i: *ActiveContainer)
        {
            if( i->IsActive() )
            {
                i->OnBeginFrame();
            }
        }
    }
}

void CUserUpdater::OnEvent(const CEvent& Event)
{
    for(const auto &i: Global)
    {
        if( i->IsActive() )
        {
            i->OnEvent( Event );
        }
    }
    if( ActiveContainer )
    {
        for(const auto &i: *ActiveContainer)
        {
            if( i->IsActive() )
            {
                i->OnEvent( Event );
            }
        }
    }
}

void CUserUpdater::OnUpdate(const float TimeStep)
{
    for(const auto &i: Global)
    {
        if( i->IsActive() )
        {
            i->OnUpdate( TimeStep );
        }
    }
    if( ActiveContainer )
    {
        for(const auto &i: *ActiveContainer)
        {
            if( i->IsActive() )
            {
                i->OnUpdate( TimeStep );
            }
        }
    }
}

void CUserUpdater::OnLateUpdate(const float TimeStep)
{
    for(const auto &i: Global)
    {
        if( i->IsActive() )
        {
            i->OnLateUpdate( TimeStep );
        }
    }
    if( ActiveContainer )
    {
        for(const auto &i: *ActiveContainer)
        {
            if( i->IsActive() )
            {
                i->OnLateUpdate( TimeStep );
            }
        }
    }
}

void CUserUpdater::OnGUI()
{
    for(const auto &i: Global)
    {
        if( i->IsActive() )
        {
            i->OnGUI();
        }
    }
    if( ActiveContainer )
    {
        for(const auto &i: *ActiveContainer)
        {
            if( i->IsActive() )
            {
                i->OnGUI();
            }
        }
    }
}

void CUserUpdater::OnRender()
{
    for(const auto &i: Global)
    {
        if( i->IsActive() )
        {
            i->OnRender();
        }
    }
    if( ActiveContainer )
    {
        for(const auto &i: *ActiveContainer)
        {
            if( i->IsActive() )
            {
                i->OnRender();
            }
        }
    }
}

void CUserUpdater::OnEndFrame()
{
    for(const auto &i: Global)
    {
        if( i->IsActive() )
        {
            i->OnEndFrame();
        }
    }
    if( ActiveContainer )
    {
        for(const auto &i: *ActiveContainer)
        {
            if( i->IsActive() )
            {
                i->OnEndFrame();
            }
        }
    }
}

void CUserUpdater::OnLeave()
{
    if( ActiveContainer )
    {
        for(const auto &i: *ActiveContainer)
        {
            i->OnLeave();
        }
    }
    for(const auto &i: Global)
    {
        i->OnLeave();
    }
}

void CUserUpdater::OnExit()
{
    for(const auto &i: Containers)
    {
        for(auto it = i.second.crbegin(); it != i.second.crend(); ++it)
        {
            (*it)->OnExit();
        }
    }
    for(auto it = Global.crbegin(); it != Global.crend(); ++it)
    {
        (*it)->OnExit();
    }
}

void CUserUpdater::Exit()
{
    NewContainer = nullptr;
    ActiveContainer = nullptr;
    Global.clear();
    Containers.clear();
}

bool CUserUpdater::SetContainer(const std::string& Name)
{
    if( Containers.count( Name ) )
    {
        NewContainer = &Containers[Name];
        LOG( ESeverity::Info ) << "Active Container - " << Name << "\n";
        return true;
    }
    LOG( ESeverity::Error ) << "Trying to Set Container That Not Exist - " << Name << "\n";
    return false;
}

bool CUserUpdater::AddContainer(const std::string& Name)
{
    if( !Containers.count( Name ) )
    {
        Containers.insert( {Name, UpdatableArray()} );
        return true;
    }
    LOG( ESeverity::Error ) << "Trying to Add Container That Already Exist - " << Name << "\n";
    return false;
}

void CUserUpdater::AddUpdatable(IUpdatable* Ptr)
{
    if( !Ptr )
    {
        LOG( ESeverity::Error ) << "Invalid IUpdatable\n";
        return;
    }

    auto Iterator = std::find_if(Global.begin(), Global.end(), [&](IUpdatable* Other)
    {
        return Ptr == Other;
    });
    if( Iterator != Global.end() )
    {
        LOG( ESeverity::Error ) << "IUpdatable Already Added\n";
        return;
    }

    Global.push_back(Ptr);
}

void CUserUpdater::AddUpdatable(const std::string& Name, IUpdatable* Ptr)
{
    if( !Ptr )
    {
        LOG( ESeverity::Error ) << "Invalid IUpdatable\n";
        return;
    }

    if( !Containers.count( Name ) )
    {
        LOG( ESeverity::Error ) << "Trying to Add IUpdatable to Non-Existing Container - " << Name << "\n";
        return;
    }

    auto& Container = Containers[Name];
    auto Iterator = std::find_if(Container.begin(), Container.end(), [&](IUpdatable* Other)
    {
        return Ptr == Other;
    });
    if( Iterator != Container.end() )
    {
        LOG( ESeverity::Error ) << "IUpdatable Already Added in " << Name << "\n";
        return;
    }

    Container.push_back(Ptr);
}

IUpdatable* CUserUpdater::GetUpdatableByName(const std::string& Name) const
{
    for(const auto &i: Global)
    {
        if( i->GetName() == Name )
        {
            return i;
        }
    }
    for(const auto &j: Containers)
    {
        for(const auto &i: j.second)
        {
            if( i->GetName() == Name )
            {
                return i;
            }
        }
    }
    return nullptr;
}

IUpdatable* CUserUpdater::GetUpdatableByType(const std::string& Type) const
{
    for(const auto &i: Global)
    {
        if( i->GetType() == Type )
        {
            return i;
        }
    }
    for(const auto &j: Containers)
    {
        for(const auto &i: j.second)
        {
            if( i->GetType() == Type )
            {
                return i;
            }
        }
    }
    return nullptr;
}

void CUserUpdater::ChangeContainer()
{
    if( NewContainer )
    {
        if( ActiveContainer )
        {
            for(const auto &i: *ActiveContainer)
            {
                i->OnLeave();
            }
        }
        for(const auto &i: *NewContainer)
        {
            i->OnEnter();
        }
        ActiveContainer = NewContainer;
        NewContainer = nullptr;
    }
}