#include "EngineUpdater.hpp"
#include "../Core/Log.hpp"

CEngineUpdater::CEngineUpdater()
{
    LOG( ESeverity::Debug ) << "Engine Updater - Created\n";
}

CEngineUpdater::~CEngineUpdater()
{
    LOG( ESeverity::Debug ) << "Engine Updater - Destroyed\n";
}

bool CEngineUpdater::Init(const SEngineParams& Parameters)
{
    for(const auto &i: Modules)
    {
        if( !i->Init( Parameters ) )
        {
            return false;
        }
    }
    return true;
}

void CEngineUpdater::OnInit()
{
    for(const auto &i: Modules)
    {
        i->OnInit();
    }
}

void CEngineUpdater::OnEnter()
{
    for(const auto &i: Modules)
    {
        i->OnEnter();
    }
}

void CEngineUpdater::OnBeginFrame()
{
    for(const auto &i: Modules)
    {
        i->OnBeginFrame();
    }
}

void CEngineUpdater::OnEvent(const CEvent& Event)
{
    for(const auto &i: Modules)
    {
        i->OnEvent( Event );
    }
}

void CEngineUpdater::OnUpdate(const float TimeStep)
{
    for(const auto &i: Modules)
    {
        i->OnUpdate( TimeStep );
    }
}

void CEngineUpdater::OnLateUpdate(const float TimeStep)
{
    for(const auto &i: Modules)
    {
        i->OnLateUpdate( TimeStep );
    }
}

void CEngineUpdater::OnGUI()
{
    for(const auto &i: Modules)
    {
        i->OnGUI();
    }
}

void CEngineUpdater::OnRender()
{
    for(const auto &i: Modules)
    {
        i->OnRender();
    }
}

void CEngineUpdater::OnEndFrame()
{
    for(const auto &i: Modules)
    {
        i->OnEndFrame();
    }
}

void CEngineUpdater::OnLeave()
{
    for(const auto &i: Modules)
    {
        i->OnLeave();
    }
}

void CEngineUpdater::OnExit()
{
	for(auto it = Modules.crbegin(); it != Modules.crend(); ++it)
	{
	    (*it)->OnExit();
	}
}

void CEngineUpdater::Exit()
{
	for(auto it = Modules.crbegin(); it != Modules.crend(); ++it)
	{
	    (*it)->Exit();
	}
}

bool CEngineUpdater::AddEngineModule(IEngineModule* Ptr)
{
    if( !Ptr )
    {
        LOG( ESeverity::Error ) << "Invalid IEngineModule\n";
        return false;
    }
    Modules.push_back( Ptr );
    return true;
}

IEngineModule* CEngineUpdater::GetEngineModule(const std::string& Type) const
{
    for(const auto &i: Modules)
    {
        if( i->GetType() == Type )
        {
            return i;
        }
    }
    return nullptr;
}