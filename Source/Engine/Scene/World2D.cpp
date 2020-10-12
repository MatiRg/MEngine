#include "World2D.hpp"
#include "../Core/Log.hpp"
#include "../Core/XML.hpp"
#include "../Engine/Engine.hpp"
#include "../System/System.hpp"

CWorld2D::CWorld2D(CEngine* aEngine):
    CEntity2D(aEngine)
{
    SetID(WORLD_ENTITY);
    SetWorld( this );
    //
    LOG( ESeverity::Debug ) << "World 2D - Created\n";
}

CWorld2D::~CWorld2D()
{
    LOG( ESeverity::Debug ) << "World 2D - Destroyed\n";
}

bool CWorld2D::Load(const std::string& Path)
{
    ISystem* System = Engine->GetSystem();
    if( !System->FileExist(Path) )
    {
        LOG(ESeverity::Error) << "World 2D Not Exist: " << Path << "\n";
        return false;
    }

	CXMLDocument Doc;
    if( !Doc.Load(Path) )
    {
        LOG(ESeverity::Error) << "Unable to Load World 2D - " << Path << "\n";
        return false;
    }

    CXMLElement* Root = Doc.GetElement("World2D");
    if( !Root )
    {
        LOG(ESeverity::Error) << "No Root Element in World 2D - " << Path << "\n";
        return false;
    }

	IDPool = XML::LoadInt(Root, "IDPool", 0);

    if( !CEntity2D::Load(Root) )
    {
        return false;
    }
    
    LOG( ESeverity::Info ) << "Loaded World 2D - " << Path << "\n";
    return true;
}

bool CWorld2D::Save(const std::string& Path)
{
    CXMLDocument Doc;
    CXMLElement* Root = Doc.NewElement("World2D");

	XML::SaveInt(Root, "IDPool", IDPool);

    if( !CEntity2D::Save(Root) )
    {
        return false;
    }

    if( !Doc.Save(Path) )
    {
        LOG(ESeverity::Error) << "Unable to Save World 2D - " << Path << "\n";
        return false;
    }
    LOG( ESeverity::Info ) << "World 2D Saved - " << Path << "\n";
    return true;
}

void CWorld2D::OnBeginFrame()
{
}

void CWorld2D::OnUpdate(const float)
{
}

void CWorld2D::OnLateUpdate(const float)
{
}

void CWorld2D::OnGUI()
{
}

void CWorld2D::OnRender()
{
}

void CWorld2D::OnEndFrame()
{
}

void CWorld2D::OnEntityCreate(CEntity2D* Entity)
{
    if( Entity )
    {
        Entity->SetID(GetNextID());
        Entity->SetWorld(this);
    }
}

void CWorld2D::OnEntityDestroy(CEntity2D* Entity)
{
    if( Entity )
    {
        Entity->SetID(INVALID_ENTITY);
        Entity->SetWorld(nullptr);
    }
}

int CWorld2D::GetNextID()
{
    return IDPool++;
}