#include "Resources.hpp"
#include "../Engine/Engine.hpp"
#include "../Graphics/Graphics.hpp"
#include "../Graphics/Model.hpp"
#include "../Graphics/Texture2D.hpp"
#include "../Graphics/Font.hpp"
#include "../Graphics/Shader.hpp"
#include "../Graphics/Material.hpp"
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
    RegisterManager<CTextureManager>( Engine->GetGraphics() );
    RegisterManager<CFontManager>( Engine->GetGraphics() );
    RegisterManager<CShaderManager>( Engine->GetGraphics() );
    RegisterManager<CSoundDataManager>( Engine->GetAudio() );
    RegisterManager<CModelManager>(Engine->GetGraphics());
    RegisterManager<CMaterialManager>();

    LOG( ESeverity::Info ) << "Resources - Init\n";
    return true;
}

void CResources::Exit()
{
    Managers.clear();
	SearchPath.clear();
}

IResource* CResources::CreateResource(const std::string& Type, const std::string& Name, const ResourceCreateMap& Vars)
{
    if( Type.empty() )
    {
        LOG(ESeverity::Error) << "Invalid Type\n";
        return nullptr;
    }

    if( Name.empty() )
    {
        LOG( ESeverity::Error ) << "Invalid Name\n";
        return nullptr;
    }

    IResourceManager* Manager = GetManagerFor(Type);
    if( !Manager )
    {
        LOG(ESeverity::Error) << "Unable to Find Resource Manager: " << Type << "\n";
        return nullptr;
    }

    return Manager->CreateResource(Name, Vars);
}

IResourceManager* CResources::GetManagerFor(const std::string& Type) const
{
    auto It = std::find_if(Managers.begin(), Managers.end(), [&](const std::unique_ptr<IResourceManager>& C)
    {
            return C->IsSupported(Type);
    });
    if (It != Managers.end())
    {
        return (*It).get();
    }
    return {};
}

IResourceManager* CResources::GetManager(const std::string& Type) const
{
    auto It = std::find_if(Managers.begin(), Managers.end(), [&](const std::unique_ptr<IResourceManager>& C)
        {
            return C->GetType() == Type;
        });
    if (It != Managers.end())
    {
        return (*It).get();
    }
    return {};
}

void CResources::AddPath(const std::string& Path, const bool SubDirs)
{
    AddPathInternal(Path, SubDirs, false);
}

void CResources::AddStaticPath(const std::string& Path, const bool SubDirs)
{
    AddPathInternal(Path, SubDirs, true);
}

void CResources::AddPathInternal(const std::string& Path, const bool SubDirs, const bool Persist)
{
    if (Path.empty())
    {
        return;
    }
    std::string Tmp = Utils::AddTrailingDirSeparator(Path);
    SearchPath.push_back({ Tmp, Persist });
    if (SubDirs)
    {
        std::vector<CFileRecord> SubDirectories = Engine->GetSystem()->ListDirectory(Tmp, true);
        for (const auto& i : SubDirectories)
        {
            if (i.Directory)
            {
                SearchPath.push_back({ Utils::AddTrailingDirSeparator(i.Path), Persist });
            }
        }
    }
}

bool CResources::FindPath(const std::string& Name, std::string& Path) const
{
    if( GetSystem()->FileExist( Name ) )
    {
        Path = Name;
        return true;
    }
    std::string Tmp;
    for(const auto& i: SearchPath)
    {
        Tmp = i.Path+Name;
        if( GetSystem()->FileExist(Tmp) )
        {
            Path = Tmp;
            return true;
        }
    }
    LOG( ESeverity::Error ) << "Unable to find path for file specified: " << Name << "\n";
    return false;
}

void CResources::ClearPath()
{
    SearchPath.erase( std::remove_if( SearchPath.begin(), SearchPath.end(), [](const CSearchPath& Path)
    {
        return !Path.Persist;
    }), SearchPath.end() );
}
