#include "World.hpp"
#include "../Core/Log.hpp"
#include "../Core/XML.hpp"
#include "../Engine/Engine.hpp"
#include "../System/System.hpp"
#include "../Graphics/Renderer3D.hpp"

CWorld::CWorld(CEngine* aEngine):
    CEntity(aEngine)
{
    SetID(WORLD_ENTITY);
    SetWorld( this );
    //
    LOG( ESeverity::Debug ) << "World - Created\n";
}

CWorld::~CWorld()
{
    LOG( ESeverity::Debug ) << "World - Destroyed\n";
}

void CWorld::OnRender()
{
    Engine->GetRenderer3D()->SetAmbientColor(AmbientColor);
}

bool CWorld::Load(const std::string& Path)
{
    ISystem* System = Engine->GetSystem();
    if( !System->FileExist(Path) )
    {
        LOG(ESeverity::Error) << "World is Not Existing: " << Path << "\n";
        return false;
    }

	CXMLDocument Doc;
    if( !Doc.Load(Path) )
    {
        LOG(ESeverity::Error) << "Unable to Load World - " << Path << "\n";
        return false;
    }

    CXMLElement* Root = Doc.GetElement("World");
    if( !Root )
    {
        LOG(ESeverity::Error) << "No Root Element in World - " << Path << "\n";
        return false;
    }

	IDPool = XML::LoadInt(Root, "IDPool", 0);
    AmbientColor = XML::LoadColor(Root, "AmbientColor", Color(0.1f, 0.1f, 0.1f));

    if( !CEntity::Load(Root) )
    {
        return false;
    }
    
    LOG( ESeverity::Info ) << "Loaded World - " << Path << "\n";
    return true;
}

bool CWorld::Save(const std::string& Path)
{
    CXMLDocument Doc;
    CXMLElement* Root = Doc.NewElement("World");

	XML::SaveInt(Root, "IDPool", IDPool);
    XML::SaveColor(Root, "AmbientColor", AmbientColor);

    if( !CEntity::Save(Root) )
    {
        return false;
    }

    if( !Doc.Save(Path) )
    {
        LOG(ESeverity::Error) << "Unable to Save World - " << Path << "\n";
        return false;
    }
    LOG( ESeverity::Info ) << "World Saved - " << Path << "\n";
    return true;
}

void CWorld::OnEntityCreate(CEntity* Entity)
{
    if( Entity )
    {
        Entity->SetID(GetNextID());
        Entity->SetWorld(this);
    }
}

void CWorld::OnEntityDestroy(CEntity* Entity)
{
    if( Entity )
    {
        Entity->SetID(INVALID_ENTITY);
        Entity->SetWorld(nullptr);
    }
}

int CWorld::GetNextID()
{
    return IDPool++;
}