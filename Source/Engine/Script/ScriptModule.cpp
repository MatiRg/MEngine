#include "ScriptModule.hpp"
#include "../Core/Platform.hpp"
#include "../Core/Log.hpp"
#include "../Engine/Engine.hpp"
#include "../Engine/UserUpdater.hpp"
#include "../Input/Input.hpp"
#include "../Graphics/Graphics.hpp"
#include "../Graphics/Window.hpp"
#include "../Graphics/Renderer2D.hpp"
#include "../Audio/Audio.hpp"
#include "../Audio/AudioHandler.hpp"
#include "../Resources/Resources.hpp"
#include "../Scene/Scene.hpp"
#include "../Audio/SoundData.hpp"
#include "../Graphics/Font.hpp"
#include "../Graphics/Texture2D.hpp"
#include "../Graphics/Drawer2D.hpp"

namespace
{
    CUserUpdater* UserUpdater = nullptr;
    IInput* Input = nullptr;
    CRenderer2D* Renderer2D = nullptr;
    CResources* Resources = nullptr;
    IWindow* Window = nullptr;
    IGraphics* Graphics = nullptr;
    IAudio* Audio = nullptr;
    CAudioHandler* AudioHandler = nullptr;
    CScene* Scene = nullptr;
    CDrawer2D* Drawer2D = nullptr;
    CEngine* EngineGlobal = nullptr;
}

CScriptModule::CScriptModule(CEngine* aEngine):
    IEngineModule( "Script Module" ),
    Engine(aEngine)
{
}


CScriptModule::~CScriptModule()
{
}

bool CScriptModule::Init(const SEngineParams&)
{
    UserUpdater = Engine->GetUserUpdater();
    Input = Engine->GetInput();
    Renderer2D = Engine->GetRenderer2D();
    Resources = Engine->GetResources();
    Window = Engine->GetWindow();
    Graphics = Engine->GetGraphics();
    Audio = Engine->GetAudio();
    AudioHandler = Engine->GetAudioHandler();
    Scene = Engine->GetScene();
    Drawer2D = Engine->GetDrawer2D();
    EngineGlobal = Engine;

    if( !InitBindings() )
    {
        LOG( ESeverity::Fatal ) << "Unable to Init Script Bindings\n";
        return false;
    }

    LOG( ESeverity::Info ) << "Script Module - Init\n";
    return true;
}

void CScriptModule::Exit()
{
    LOG( ESeverity::Info ) << "Script Module - Exit\n";
}

bool CScriptModule::InitBindings()
{
    LOG(ESeverity::Info) << "Setup - Script Bindings - Success\n";
    return true;
}
