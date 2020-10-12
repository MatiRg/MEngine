#include "SDLInput.hpp"
#include "../Core/Log.hpp"
#include "../Input/Event.hpp"
#include "SDLWindow.hpp"
#include <SDL2/SDL.h>
#include <algorithm>

CSDLInput::CSDLInput(CSDLWindow* aWindow):
    Window( aWindow )
{
}

CSDLInput::~CSDLInput()
{
    if( ClipSDLText )
    {
        SDL_free( ClipSDLText );
        ClipSDLText = nullptr;
    }
}

bool CSDLInput::Init(const SEngineParams&)
{
    KeyDown.fill( false );
    KeyPressed.fill( false );
    MouseKeyDown.fill( false );
    MouseKeyPressed.fill( false );

    SetMouseMode( EMouseMode::Normal );

    return true;
}

void CSDLInput::ClearInput()
{
    KeyDown.fill( false );
    MouseKeyDown.fill( false );
    KeyPressed[static_cast<std::size_t>(EKey::Any)] = false;
    MouseKeyPressed[static_cast<std::size_t>(EMouseKey::Any)] = false;
    RelativeMouseMotion = Vector2( 0.0f );
    MouseWheel = Vector2( 0.0f );
}

void CSDLInput::OnEvent(const CEvent& Event)
{
    switch( Event.GetType() )
    {
        case EEventType::Keyboard:
        {
            SEventKeyboard* Data = Event.GetData<SEventKeyboard>();
            if( Data )
            {
                KeyDown[static_cast<std::size_t>(Data->Key)] = Data->Down;
                if( Data->Down ) KeyDown[static_cast<std::size_t>(EKey::Any)] = true;
                KeyPressed[static_cast<std::size_t>(Data->Key)] = Data->Pressed;
                if( Data->Pressed ) KeyPressed[static_cast<std::size_t>(EKey::Any)] = true;
            }
            break;
        }
        case EEventType::MouseButton:
        {
            SEventMouseButton* Data = Event.GetData<SEventMouseButton>();
            if( Data )
            {
                MouseKeyDown[static_cast<std::size_t>(Data->Key)] = Data->Down;
                if( Data->Down ) MouseKeyDown[static_cast<std::size_t>(EMouseKey::Any)] = true;
                MouseKeyPressed[static_cast<std::size_t>(Data->Key)] = Data->Down;
                if( Data->Down ) MouseKeyPressed[static_cast<std::size_t>(EMouseKey::Any)] = true;
            }
            break;
        }
        case EEventType::MouseMotion:
        {
            SEventMouseMotion* Data = Event.GetData<SEventMouseMotion>();
            if( Data )
            {
                MousePosition = Data->Position;
                RelativeMouseMotion = Data->Relative;
            }
            break;
        }
        case EEventType::MouseWheel:
        {
            SEventMouseWheel* Data = Event.GetData<SEventMouseWheel>();
            if( Data ) MouseWheel = Vector2( Data->x, Data->y );
            break;
        }
        default:
            break;
    }
}

void CSDLInput::SetMousePosition(const Vector2& Position)
{
    MousePosition = Position;
    SDL_WarpMouseInWindow( Window->GetSDLWindow(), std::lround( MousePosition.x ), std::lround( MousePosition.y ) );
}

bool CSDLInput::SetMouseMode(const EMouseMode Mode)
{
    if( SDL_SetRelativeMouseMode( Mode == EMouseMode::Relative ? SDL_TRUE : SDL_FALSE ) != 0 )
    {
        LOG( ESeverity::Error ) << "SDL: " << SDL_GetError() << "\n";
        return false;
    }
    MouseMode = Mode;
    return true;
}

void CSDLInput::ShowCursor(const bool x)
{
    SDL_ShowCursor( x ? SDL_ENABLE : SDL_DISABLE );
}

bool CSDLInput::IsCursorVisible() const
{
    return SDL_ShowCursor( SDL_QUERY ) == SDL_ENABLE ? true : false;
}

const std::string& CSDLInput::GetClipBoardText() const
{
    if( ClipSDLText )
    {
        SDL_free( ClipSDLText );
        ClipSDLText = nullptr;
    }
    ClipSDLText = SDL_GetClipboardText();
    ClipBoardText = ClipSDLText;
    return ClipBoardText;
}

void CSDLInput::SetClipBoardText(const std::string& T)
{
    SDL_SetClipboardText( T.c_str() );
}