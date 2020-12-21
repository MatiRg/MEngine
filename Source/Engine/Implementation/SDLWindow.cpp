#include "SDLWindow.hpp"
#include "SDLSurface.hpp"
#include "../Resources/Resources.hpp"
#include "../Core/Log.hpp"
#include "../Core/Utils.hpp"
#include <SDL2/SDL.h>

CSDLWindow::CSDLWindow()
{
}

CSDLWindow::~CSDLWindow()
{
    if( Window )
    {
        SDL_DestroyWindow( Window );
        Window = nullptr;
    }
}

bool CSDLWindow::Init(const SEngineParams& Parameters)
{
    int Width = Parameters.Width <= 0 ? 800 : Parameters.Width,
        Height = Parameters.Height <= 0 ? 600 : Parameters.Height,
        WinPosX = SDL_WINDOWPOS_CENTERED,
        WinPosY = SDL_WINDOWPOS_CENTERED;
    Caption = Parameters.Caption;
    WindowSize = IntVector2( Width, Height );

    Uint32 Flags = SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL;

    SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 4 );
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 6 );
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE );
    SDL_GL_SetAttribute( SDL_GL_RED_SIZE, 8 );
    SDL_GL_SetAttribute( SDL_GL_GREEN_SIZE, 8 );
    SDL_GL_SetAttribute( SDL_GL_BLUE_SIZE, 8 );
    SDL_GL_SetAttribute( SDL_GL_ALPHA_SIZE, 8 );
    SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );
    SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE, 24 );
    SDL_GL_SetAttribute( SDL_GL_STENCIL_SIZE, 8 );

    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);

    Window = SDL_CreateWindow( Caption.c_str(), WinPosX, WinPosY, Width, Height, Flags );
    if( !Window )
    {
        LOG( ESeverity::Fatal ) << "Unable to create window: " << SDL_GetError() << "\n";
        return false;
    }

    LOG( ESeverity::Info ) << Utils::Format( "Window created: %dx%d", Width, Height ) << "\n";
    return true;
}

bool CSDLWindow::SetCaption(const std::string& C)
{
    Caption = C;
    SDL_SetWindowTitle( Window, Caption.c_str() );
    return true;
}

bool CSDLWindow::SetSize(const IntVector2& Size)
{
    WindowSize = Size;
    SDL_SetWindowSize( Window, WindowSize.x, WindowSize.y );
    return true;
}

float CSDLWindow::GetAspectRatio() const
{
    return static_cast<float>(WindowSize.x) / static_cast<float>(WindowSize.y);
}

bool CSDLWindow::SetIcon(const std::string& Name, CResources* Resources)
{
    if( Name.empty() )
    {
        LOG( ESeverity::Error ) << "Invalid Name\n";
        return false;
    }

    std::string FullPath;
    if( !Resources->FindPath(Name, FullPath) )
    {
        return false;
    }

    std::shared_ptr<CSDLSurface> Surface = std::make_shared<CSDLSurface>();
    if( Surface->Load( FullPath ) )
    {
        SDL_SetWindowIcon( Window, Surface->GetSDLSurface() );
        return true;
    }
    return false;
}
