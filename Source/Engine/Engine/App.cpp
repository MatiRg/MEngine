#include "App.hpp"
#include "Engine.hpp"
#include "../Core/Log.hpp"
#include "../Implementation/SDLContext.hpp"

CApp::CApp()
{
    LOG( ESeverity::Debug ) << "App - Created\n";
}

CApp::~CApp()
{
    Engine.reset();
    Context.reset();
    Config.reset();
    LOG( ESeverity::Debug ) << "App - Destroyed\n";
}

int CApp::Run(const StringVec& Cmd)
{
    try
    {
        CommandLine = Cmd;

        LOG(ESeverity::Info) << "Entering CApp::PreInit()\n";
        if( !PreInit() )
        {
            LOG(ESeverity::Fatal) << "CApp::PreInit()\n";
            return 1;
        }

        if (UseConfigFile)
        {
            Config = std::make_unique<CConfig>(ConfigFile);
            LOG(ESeverity::Debug) << "Loading Engine Configuration from: " << ConfigFile << "\n";
            if (!Config->Load())
            {
                LOG(ESeverity::Fatal) << "Config::Load()\n";
                return 1;
            }
            Params.Width = Config->GetInt("Engine", "Width", 800);
            Params.Height = Config->GetInt("Engine", "Height", 600);
            Params.Caption = Config->GetString("Engine", "Caption", "Engine");
            Params.VSync = Config->GetBool("Engine", "VSync", true);
            Params.UseAudio = Config->GetBool("Engine", "UseAudio", true);
            Params.Ticks = static_cast<uint32_t>(Config->GetInt("Engine", "Ticks", 60));
        }
        else
        {
            LOG(ESeverity::Debug) << "Using Default Engine Configuration\n";
        }

        LOG(ESeverity::Info) << "Creating Context\n";
        Context = std::make_unique<CSDLContext>();
        if( !Context->Init( Params ) )
        {
            LOG(ESeverity::Fatal) << "IContext::Init()\n";
            return 1;
        }

        LOG(ESeverity::Info) << "Creating Engine\n";
        Engine = std::make_unique<CEngine>( Context.get() );
        if( !Engine->Create() )
        {
            LOG(ESeverity::Fatal) << "CEngine::Create()\n";
            return 1;
        }

        LOG(ESeverity::Info) << "Entering CApp::Init()\n";
        if( !Init() )
        {
            LOG(ESeverity::Fatal) << "CApp::Init()\n";
            return 1;
        }
        LOG(ESeverity::Info) << "Entering CEngine::Init()\n";
        if( !Engine->Init( Params ) )
        {
            LOG(ESeverity::Fatal) << "CEngine::Init()\n";
            return 1;
        }

        LOG(ESeverity::Info) << "Entering CApp::OnInit()\n";
        OnInit();
        LOG(ESeverity::Info) << "Entering CEngine::OnInit()\n";
        Engine->OnInit();

        try
        {
            LOG(ESeverity::Info) << "Entering CEngine::Run()\n";
            Engine->Run();
        }
        catch(const std::exception& Exception)
        {
            LOG( ESeverity::Fatal ) << Exception.what() << "\n";
        }
        catch(...)
        {
            LOG(ESeverity::Fatal) << "Unknown Exception\n";
        }

        LOG(ESeverity::Info) << "Entering CApp::OnExit()\n";
        OnExit();
        LOG(ESeverity::Info) << "Entering CEngine::OnExit()\n";
        Engine->OnExit();

        LOG(ESeverity::Info) << "Entering CApp::Exit()\n";
        Exit();
        LOG(ESeverity::Info) << "Entering CEngine::Exit()\n";
        Engine->Exit();

        LOG(ESeverity::Info) << "Entering CApp::PostExit()\n";
        PostExit();

        LOG(ESeverity::Info) << "Destroying Engine\n";
        Engine.reset();

        LOG(ESeverity::Info) << "Destroying Context\n";
        Context.reset();

        if (UseConfigFile)
        {
            LOG(ESeverity::Info) << "Saving Config\n";
            Config->Save();
            Config.reset();
        }

        return 0;
    }
    catch(const std::exception& Exception)
    {
        LOG( ESeverity::Fatal ) << Exception.what() << "\n";
    }
    return 1;
}

void CApp::Quit()
{
    if( Engine.get() )
    {
        Engine->Quit();
    }
}

CEngine* CApp::GetEngine() const
{
    return Engine.get();
}

ISystem* CApp::GetSystem() const
{
    return (!Engine.get()) ? nullptr : Engine->GetSystem();
}

IEventQueue* CApp::GetEventQueue() const
{
    return (!Engine.get()) ? nullptr : Engine->GetEventQueue();
}

IInput* CApp::GetInput() const
{
    return (!Engine.get()) ? nullptr : Engine->GetInput();
}

IWindow* CApp::GetWindow() const
{
    return (!Engine.get()) ? nullptr : Engine->GetWindow();
}

IGraphics* CApp::GetGraphics() const
{
    return (!Engine.get()) ? nullptr : Engine->GetGraphics();
}

IAudio* CApp::GetAudio() const
{
    return (!Engine.get()) ? nullptr : Engine->GetAudio();
}

CEngineUpdater* CApp::GetEngineUpdater() const
{
    return (!Engine.get()) ? nullptr : Engine->GetEngineUpdater();
}

CUserUpdater* CApp::GetUserUpdater() const
{
    return (!Engine.get()) ? nullptr : Engine->GetUserUpdater();
}

CResources* CApp::GetResources() const
{
    return (!Engine.get()) ? nullptr : Engine->GetResources();
}

CAudioHandler* CApp::GetAudioHandler() const
{
    return (!Engine.get()) ? nullptr : Engine->GetAudioHandler();
}

CRenderer3D* CApp::GetRenderer3D() const
{
    return (!Engine.get()) ? nullptr : Engine->GetRenderer3D();
}

CDrawer2D* CApp::GetDrawer2D() const
{
    return (!Engine.get()) ? nullptr : Engine->GetDrawer2D();
}

CRenderer2D* CApp::GetRenderer2D() const
{
    return (!Engine.get()) ? nullptr : Engine->GetRenderer2D();
}

IPhysics3D* CApp::GetPhysics3D() const
{
    return (!Engine.get()) ? nullptr : Engine->GetPhysics3D();
}

IPhysics2D* CApp::GetPhysics2D() const
{
    return (!Engine.get()) ? nullptr : Engine->GetPhysics2D();
}

CImGUI* CApp::GetImGUI() const
{
    return (!Engine.get()) ? nullptr : Engine->GetImGUI();
}

CScene* CApp::GetScene() const
{
    return (!Engine.get()) ? nullptr : Engine->GetScene();
}

CScriptModule* CApp::GetScriptModule() const
{
    return (!Engine.get()) ? nullptr : Engine->GetScriptModule();
}
