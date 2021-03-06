#include "Engine.hpp"
#include "../System/System.hpp"
#include "../Input/EventQueue.hpp"
#include "../Input/Input.hpp"
#include "../Graphics/Window.hpp"
#include "../Graphics/Graphics.hpp"
#include "../Audio/Audio.hpp"
#include "../Resources/Resources.hpp"
#include "../Audio/AudioHandler.hpp"
#include "../Core/Log.hpp"
#include "../Core/Utils.hpp"
#include "../Graphics/Renderer2D.hpp"
#include "../Graphics/Drawer2D.hpp"
#include "../Graphics/Renderer3D.hpp"
#include "../Graphics/DebugDrawer.hpp"
#include "../Scene/Scene.hpp"
#include "../UI/ImGUI.hpp"
#include "../Script/ScriptModule.hpp"
#include "../Math/Functions.hpp"
#include "../Physics/Physics2D.hpp"
#include "../Physics/Physics3D.hpp"
#include "Context.hpp"
#include "EngineUpdater.hpp"
#include "UserUpdater.hpp"
#include "PerformanceCounter.hpp"

CEngine::CEngine(IContext* aContext):
    Context( aContext )
{
    LOG( ESeverity::Debug ) << "Engine - Created\n";
}

CEngine::~CEngine()
{
    Counter.reset();
    ScriptModule.reset();
    Scene.reset();
    ImGUI.reset();
    DebugDrawer.reset();
    Renderer2D.reset();
    Drawer2D.reset();
    Renderer3D.reset();
    AudioHandler.reset();
    Resources.reset();
    UserUpdater.reset();
    EngineUpdater.reset();
    LOG( ESeverity::Debug ) << "Engine - Destroyed\n";
}

bool CEngine::Create()
{
    System = Context->GetSystem();
    EventQueue = Context->GetEventQueue();
    Input = Context->GetInput();
    Window = Context->GetWindow();
    Graphics = Context->GetGraphics();
    Audio = Context->GetAudio();
    Physics3D = Context->GetPhysics3D();
    Physics2D = Context->GetPhysics2D();

    EngineUpdater = std::make_unique<CEngineUpdater>();
    UserUpdater = std::make_unique<CUserUpdater>();
    Resources = std::make_unique<CResources>( this );
    AudioHandler = std::make_unique<CAudioHandler>( Audio, Resources.get() );
    Renderer3D = std::make_unique<CRenderer3D>(Graphics, Resources.get());
    Drawer2D = std::make_unique<CDrawer2D>( Graphics, Resources.get(), Window );
    Renderer2D = std::make_unique<CRenderer2D>( Drawer2D.get() );
    DebugDrawer = std::make_unique<CDebugDrawer>(Renderer3D.get(), Graphics, Resources.get() );
    ImGUI = std::make_unique<CImGUI>( Input, Graphics );
    Scene = std::make_unique<CScene>( this );
    ScriptModule = std::make_unique<CScriptModule>( this );
    Counter = std::make_unique<CPerformanceCounters>( ImGUI.get(), System, Renderer3D.get() );

    EngineUpdater->AddEngineModule( System );
    EngineUpdater->AddEngineModule( EventQueue );
    EngineUpdater->AddEngineModule( Input );
    EngineUpdater->AddEngineModule( Window );
    EngineUpdater->AddEngineModule( Graphics );
    EngineUpdater->AddEngineModule( Audio );
    EngineUpdater->AddEngineModule( Resources.get() );
    EngineUpdater->AddEngineModule( AudioHandler.get() );
    EngineUpdater->AddEngineModule(Renderer3D.get());
    EngineUpdater->AddEngineModule( Drawer2D.get() );
    EngineUpdater->AddEngineModule( Renderer2D.get() );
    EngineUpdater->AddEngineModule(Physics3D);
    EngineUpdater->AddEngineModule( Physics2D );
    EngineUpdater->AddEngineModule( ImGUI.get() );
    EngineUpdater->AddEngineModule( Scene.get() );
    EngineUpdater->AddEngineModule( ScriptModule.get() );
    EngineUpdater->AddEngineModule( Counter.get() );
    EngineUpdater->AddEngineModule( UserUpdater.get() );
    EngineUpdater->AddEngineModule( DebugDrawer.get() );

    if( !System->DirectoryExist("Core") )
    {
        LOG(ESeverity::Fatal) << "No 'Core' Directory\n";
        return false;
    }
    if (!System->DirectoryExist("Core/Shaders"))
    {
        LOG(ESeverity::Fatal) << "No 'Core/Shaders' Directory\n";
        return false;
    }
    Resources->AddStaticPath("Core", true); // Search path for default data eg. fonts, shaders ...

    LOG( ESeverity::Debug ) << "Engine Created\n";
    return true;
}

bool CEngine::Init(const SEngineParams& Params)
{
    if( !EngineUpdater->Init( Params ) )
    {
        LOG(ESeverity::Fatal) << "CEngineUpdater::Init()\n";
        return false;
    }

    SetupTicks( Params.Ticks );

    Input->SetMousePosition( Vector2( Window->GetSize().x, Window->GetSize().y) / 2.0f );

    LOG( ESeverity::Debug ) << "Engine Init\n";
    return true;
}

void CEngine::OnInit()
{
    EngineUpdater->OnInit();
    LOG( ESeverity::Debug ) << "Engine On Init\n";
}

void CEngine::OnExit()
{
    EngineUpdater->OnExit();
    LOG( ESeverity::Debug ) << "Engine On Exit\n";
}

void CEngine::Exit()
{
    EngineUpdater->Exit();
    LOG( ESeverity::Debug ) << "Engine Exit\n";
}

void CEngine::SetupTicks(const uint32_t aTicks)
{
    Ticks = Math::Max( 30u, aTicks );
    DeltaTime = 1.0f / static_cast<float>(Ticks);
    CurrentTime = static_cast<float>(System->GetTime()) / 1000.0f;
}

void CEngine::Run()
{
    LOG( ESeverity::Debug ) << "Game Loop - Start\n";
    EngineUpdater->OnEnter();
    while( Loop )
    {
        Counter->StartMeasurment();

        float NewTime = static_cast<float>(System->GetTime()) / 1000.0f;
        float FrameTime = NewTime - CurrentTime;
        Accumulator += FrameTime;
        CurrentTime = NewTime;

        {
            CPerformanceCounter PerfCounter{ EPerformanceCounter::Begin, Counter.get() };
            EngineUpdater->OnBeginFrame();
        }

        {
            CPerformanceCounter PerfCounter{ EPerformanceCounter::Event, Counter.get() };
            CEvent Event;
            EventQueue->PullEvents();
            while( EventQueue->PopEvent( Event ) )
            {
                EngineUpdater->OnEvent( Event );
                #ifdef DEV_BUILD
                if( Event.GetType() == EEventType::Keyboard )
                {
                    SEventKeyboard* Data = Event.GetData<SEventKeyboard>();
                    if( Data )
                    {
                        if( Data->Down && Data->Key == EKey::F10 )
                        {
                            Counter->Toggle();
                        }
                    }
                }
                #endif // DEV_BUILD
                if( Event.GetType() == EEventType::Quit )
                {
                    Quit();
                }
            }
        }

        {
            CPerformanceCounter PerfCounter{ EPerformanceCounter::Update, Counter.get() };
            while( Accumulator >= DeltaTime ) // Fixed Delta Time for Simulation & Rendering at Different Framerates
            {
                EngineUpdater->OnUpdate( DeltaTime );
                Accumulator -= DeltaTime;
            }
        }

        {
            CPerformanceCounter PerfCounter{ EPerformanceCounter::LateUpdate, Counter.get() };
            EngineUpdater->OnLateUpdate(DeltaTime);
        }

        {
            CPerformanceCounter PerfCounter{ EPerformanceCounter::ImGUI, Counter.get() };
            EngineUpdater->OnGUI();
        }

        Graphics->Clear();
        {
            CPerformanceCounter PerfCounter{ EPerformanceCounter::OnRender, Counter.get() };
            EngineUpdater->OnRender();
        }
        //
        {
            CPerformanceCounter PerfCounter{ EPerformanceCounter::Render3D, Counter.get() };
            Renderer3D->Render();
        }
        //
        {
            CPerformanceCounter PerfCounter{ EPerformanceCounter::Render2D, Counter.get() };
            Renderer2D->Render();
        }
        //
        {
            CPerformanceCounter PerfCounter{ EPerformanceCounter::RenderImGUI, Counter.get() };
            ImGUI->Draw();
        }
        Graphics->SwapBuffers();

        {
            CPerformanceCounter PerfCounter{ EPerformanceCounter::End, Counter.get() };
            EngineUpdater->OnEndFrame();
        }

        Counter->EndMeasurment();
    }
    EngineUpdater->OnLeave();
    LOG( ESeverity::Debug ) << "Game Loop - Shutdown\n";
}

IEngineModule* CEngine::GetModule(const std::string& Type) const
{
    return EngineUpdater->GetEngineModule(Type);
}