#include "SDLContext.hpp"
#include "SDLSystem.hpp"
#include "SDLEventQueue.hpp"
#include "SDLInput.hpp"
#include "SDLWindow.hpp"
#include "OALAudio.hpp"
#include "OGL/OGLGraphics.hpp"
#include "NullAudio/NullAudio.hpp"
#include "../Core/Log.hpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

CSDLContext::CSDLContext()
{
}

CSDLContext::~CSDLContext()
{
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}

bool CSDLContext::Init(const SEngineParams& Params)
{
    if( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER ) != 0 )
    {
        LOG(ESeverity::Fatal) << "SDL: " << SDL_GetError() << "\n";
        return false;
    }
    int ImgFlags = IMG_INIT_PNG | IMG_INIT_JPG | IMG_INIT_TIF;
    if( IMG_Init( ImgFlags ) != ImgFlags )
    {
        LOG(ESeverity::Fatal) << "IMG: " << IMG_GetError() << "\n";
        return false;
    }
    if( TTF_Init() != 0 )
    {
        LOG(ESeverity::Fatal) << "TTF: " << TTF_GetError() << "\n";
        return false;
    }

    System = std::make_unique<CSDLSystem>();
    EventQueue = std::make_unique<CSDLEventQueue>();
    Window = std::make_unique<CSDLWindow>();
    Graphics = std::make_unique<COGLGraphics>( Window.get() );
    Input = std::make_unique<CSDLInput>( Window.get() );
    if( Params.UseAudio )
    {
        Audio = std::make_unique<COALAudio>();
    }
    else
    {
        Audio = std::make_unique<CNullAudio>();
    }

    LOG( ESeverity::Debug ) << "SDL Context - Init\n";

    return true;
}

ISystem* CSDLContext::GetSystem() const
{
    return System.get();
}

IEventQueue* CSDLContext::GetEventQueue() const
{
    return EventQueue.get();
}

IInput* CSDLContext::GetInput() const
{
    return Input.get();
}

IWindow* CSDLContext::GetWindow() const
{
    return Window.get();
}

IGraphics* CSDLContext::GetGraphics() const
{
    return Graphics.get();
}

IAudio* CSDLContext::GetAudio() const
{
    return Audio.get();
}
