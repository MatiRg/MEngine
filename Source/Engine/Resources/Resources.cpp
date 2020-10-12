#include "Resources.hpp"
#include "Resource.hpp"
#include "ResourceFactory.hpp"
#include "../Engine/Engine.hpp"
#include "../Graphics/Graphics.hpp"
#include "../Graphics/Texture2D.hpp"
#include "../Graphics/Font.hpp"
#include "../Graphics/Shader.hpp"
#include "../Audio/Audio.hpp"
#include "../Audio/SoundData.hpp"
#include "../Audio/Sound.hpp"
#include "../System/System.hpp"
#include "../Core/Utils.hpp"
#include "../Core/Log.hpp"
#include <algorithm>

CResources::CResources(CEngine* aEngine):
    IEngineModule( "Resources" ),
    Engine( aEngine )
{
}

CResources::~CResources()
{
}

ISystem* CResources::GetSystem() const
{
    return Engine->GetSystem();
}

bool CResources::Init(const SEngineParams&)
{
    RegisterFactory<CTexture2DFactory>( Engine->GetGraphics() );
    RegisterFactory<CFontFactory>( Engine->GetGraphics() );
    RegisterFactory<CShaderFactory>( Engine->GetGraphics() );
    RegisterFactory<CSoundDataFactory>( Engine->GetAudio() );

    LOG( ESeverity::Info ) << "Resources - Init\n";
    return true;
}

void CResources::Exit()
{
	SearchPath.clear();
	Factories.clear();
	Resources.clear();
}

IResource* CResources::CreateResource(const std::string& Type, const std::string& Name)
{
    if( Name.empty() )
    {
        LOG( ESeverity::Error ) << "Invalid Name\n";
        return nullptr;
    }

    if( Resources.count( Name ) )
    {
        return Resources.at(Name).get();
    }

    IResourceFactory* Factory = GetFactory( Type );
    if( !Factory )
    {
        LOG( ESeverity::Error ) << "Unable to Find Resource Factory: " << Type << "\n";
        return nullptr;  
    }

    auto Ptr = Factory->CreateResource( Name, this );
    if( !Ptr )
    {
        LOG( ESeverity::Error ) << "Unable to Create Resource: " << Name << "\n";
        return nullptr; 
    }

    if( !Ptr->Find() )
    {
        return nullptr; 
    }

    if( !Ptr->Load() )
    {
        LOG( ESeverity::Error ) << "Unable to Load Resource: " << Name << "\n";
        return nullptr; 
    }
    auto Raw = Ptr.get();
    Resources[Name] = std::move(Ptr);
    return Raw;
}


IResourceFactory* CResources::GetFactory(const std::string& Name) const
{
    auto It = std::find_if( Factories.begin(), Factories.end(), [&](const std::unique_ptr<IResourceFactory>& C)
    {
        return C->GetName() == Name;
    });
    if( It != Factories.end() )
    {
        return (*It).get();
    }
    return {};
}

void CResources::AddPath(const std::string& Path, const bool Persist)
{
    if( Path.empty() )
    {
        return;
    }
    std::string Tmp = Path;
    char c = Tmp.back();
    if( c != '/' && c != '\\' )
    {
        Tmp.push_back( '/' );
    }
    SearchPath.push_back( { Tmp, Persist } );
}

std::tuple<bool, std::string> CResources::FindPath(const std::string& Name) const
{
    if( GetSystem()->FileExist( Name ) )
    {
        return {true, Name};
    }
    std::string Path;
    for(const auto& i: SearchPath)
    {
        Path = i.Path+Name;
        if( GetSystem()->FileExist( Path ) )
        {
            return {true, Path};
        }
    }
    LOG( ESeverity::Error ) << "Unable to find path for file specified: " << Name << "\n";
    return {false, ""};
}

void CResources::ClearPath()
{
    SearchPath.erase( std::remove_if( SearchPath.begin(), SearchPath.end(), [](const CSearchPath& Path)
    {
        return !Path.Persist;
    }), SearchPath.end() );
}
