#include "DebugDrawer.hpp"
#include "Renderer3D.hpp"
#include "../Resources/Resources.hpp"
#include "../Core/Log.hpp"

CDebugDrawer::CDebugDrawer(CRenderer3D* aRenderer3D, CResources* aResources):
    IEngineModule( "DebugDrawer" ),
    Renderer3D(aRenderer3D),
    Resources(aResources)
{
}

CDebugDrawer::~CDebugDrawer()
{
}

bool CDebugDrawer::Init(const SEngineParams&)
{
    LOG( ESeverity::Info ) << "DebugDrawer - Init\n";
    return true;
}

void CDebugDrawer::Exit()
{
    LOG( ESeverity::Info ) << "DebugDrawer - Exit\n";
}
