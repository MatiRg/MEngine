#include "SDLEventQueue.hpp"
#include "../Core/Log.hpp"
#include "../Core/Utils.hpp"
#include <SDL2/SDL.h>

CSDLEventQueue::CSDLEventQueue()
{
}

CSDLEventQueue::~CSDLEventQueue()
{
}

bool CSDLEventQueue::Init(const SEngineParams&)
{
    SDL_StartTextInput();
    return true;
}

void CSDLEventQueue::PullEvents()
{
    SDL_Event E;
    while( SDL_PollEvent( &E ) )
    {
        if( E.type == SDL_QUIT )
        {
            EventQueue.emplace( EEventType::Quit );
        }
        else if( E.type == SDL_KEYDOWN || E.type == SDL_KEYUP )
        {
            bool Down = E.type == SDL_KEYDOWN ? (E.key.repeat != 0 ? false : true) : false;
            bool Pressed = E.type == SDL_KEYDOWN ? true : false;
            EventQueue.emplace( EEventType::Keyboard, std::make_unique<SEventKeyboard>( ToKey( E.key.keysym.sym ), ToKeyMod( E.key.keysym.mod ), Down, Pressed ) );
        }
        else if( E.type == SDL_MOUSEBUTTONDOWN || E.type == SDL_MOUSEBUTTONUP )
        {
            bool Down = E.type == SDL_MOUSEBUTTONDOWN ? true : false;
            EventQueue.emplace( EEventType::MouseButton, std::make_unique<SEventMouseButton>( ToMouseKey( E.button.button ), Down ) );
        }
        else if( E.type == SDL_MOUSEMOTION )
        {
            Vector2 Motion( static_cast<float>(E.motion.x), static_cast<float>(E.motion.y) );
            Vector2 Wheel( static_cast<float>(E.motion.xrel), static_cast<float>(E.motion.yrel) );
            EventQueue.emplace( EEventType::MouseMotion, std::make_unique<SEventMouseMotion>( Motion, Wheel) );
        }
        else if( E.type == SDL_MOUSEWHEEL )
        {
            EventQueue.emplace( EEventType::MouseWheel, std::make_unique<SEventMouseWheel>(E.wheel.x, E.wheel.y) );
        }
        else if( E.type == SDL_TEXTINPUT )
        {
            EventQueue.emplace( EEventType::TextInput, std::make_unique<SEventTextInput>(E.text.text) );
        }
		else if( E.type == SDL_WINDOWEVENT )
		{
			if( E.window.event == SDL_WINDOWEVENT_SIZE_CHANGED )
			{
				EventQueue.emplace( EEventType::WindowSize, std::make_unique<SEventWindowSize>(E.window.data1, E.window.data2) );
			}
		}
    }
}

bool CSDLEventQueue::PopEvent(CEvent& Event)
{
    if( EventQueue.empty() )
    {
		return false;
	}
    Event = std::move(EventQueue.front());
    EventQueue.pop();
    return true;
}

bool CSDLEventQueue::PushEvent(CEvent&& Event)
{
	EventQueue.push( std::move(Event) );
	return true;
}

EKey CSDLEventQueue::ToKey(const int x) const
{
    switch( x )
    {
	case SDLK_a:
		return EKey::A;
	case SDLK_b:
		return EKey::B;
	case SDLK_c:
		return EKey::C;
	case SDLK_d:
		return EKey::D;
	case SDLK_e:
		return EKey::E;
	case SDLK_f:
		return EKey::F;
	case SDLK_g:
		return EKey::G;
	case SDLK_h:
		return EKey::H;
	case SDLK_i:
		return EKey::I;
	case SDLK_j:
		return EKey::J;
	case SDLK_k:
		return EKey::K;
	case SDLK_l:
		return EKey::L;
	case SDLK_m:
		return EKey::M;
	case SDLK_n:
		return EKey::N;
	case SDLK_o:
		return EKey::O;
	case SDLK_p:
		return EKey::P;
	case SDLK_q:
		return EKey::Q;
	case SDLK_r:
		return EKey::R;
	case SDLK_s:
		return EKey::S;
	case SDLK_t:
		return EKey::T;
	case SDLK_u:
		return EKey::U;
	case SDLK_v:
		return EKey::V;
	case SDLK_w:
		return EKey::W;
	case SDLK_x:
		return EKey::X;
	case SDLK_y:
		return EKey::Y;
	case SDLK_z:
		return EKey::Z;
	case SDLK_0:
		return EKey::Num0;
	case SDLK_1:
		return EKey::Num1;
	case SDLK_2:
		return EKey::Num2;
	case SDLK_3:
		return EKey::Num3;
	case SDLK_4:
		return EKey::Num4;
	case SDLK_5:
		return EKey::Num5;
	case SDLK_6:
		return EKey::Num6;
	case SDLK_7:
		return EKey::Num7;
	case SDLK_8:
		return EKey::Num8;
	case SDLK_9:
		return EKey::Num9;
	case SDLK_ESCAPE:
		return EKey::Escape;
	case SDLK_SPACE:
		return EKey::Space;
	case SDLK_RETURN:
		return EKey::Return;
	case SDLK_BACKSPACE:
		return EKey::BackSpace;
	case SDLK_TAB:
		return EKey::Tab;
	case SDLK_LCTRL:
		return EKey::LControl;
	case SDLK_LSHIFT:
		return EKey::LShift;
	case SDLK_LALT:
		return EKey::LAlt;
	case SDLK_RCTRL:
		return EKey::RControl;
	case SDLK_RSHIFT:
		return EKey::RShift;
	case SDLK_RALT:
		return EKey::RAlt;
	case SDLK_LEFT:
		return EKey::Left;
	case SDLK_RIGHT:
		return EKey::Right;
	case SDLK_UP:
		return EKey::Up;
	case SDLK_DOWN:
		return EKey::Down;
	case SDLK_F1:
		return EKey::F1;
	case SDLK_F2:
		return EKey::F2;
	case SDLK_F3:
		return EKey::F3;
	case SDLK_F4:
		return EKey::F4;
	case SDLK_F5:
		return EKey::F5;
	case SDLK_F6:
		return EKey::F6;
	case SDLK_F7:
		return EKey::F7;
	case SDLK_F8:
		return EKey::F8;
	case SDLK_F9:
		return EKey::F9;
	case SDLK_F10:
		return EKey::F10;
	case SDLK_F11:
		return EKey::F11;
	case SDLK_F12:
		return EKey::F12;
	case SDLK_PAGEUP:
		return EKey::PageUp;
	case SDLK_PAGEDOWN:
		return EKey::PageDown;
	case SDLK_HOME:
		return EKey::Home;
	case SDLK_END:
		return EKey::End;
	case SDLK_INSERT:
		return EKey::Insert;
	case SDLK_DELETE:
		return EKey::Delete;
    default:
        return EKey::Unknown;
    }
}

EKeyMod CSDLEventQueue::ToKeyMod(const int x) const
{
    EKeyMod Mod = EKeyMod::None;
    if( x & KMOD_CTRL ) Mod |= EKeyMod::Ctrl;
    if( x & KMOD_ALT ) Mod |= EKeyMod::Alt;
    if( x & KMOD_SHIFT ) Mod |= EKeyMod::Shift;
    return Mod;
}

EMouseKey CSDLEventQueue::ToMouseKey(const int x) const
{
    switch( x )
    {
    case SDL_BUTTON_LEFT:
        return EMouseKey::LButton;
    case SDL_BUTTON_MIDDLE:
        return EMouseKey::MButton;
    case SDL_BUTTON_RIGHT:
        return EMouseKey::RButton;
    default:
        return EMouseKey::Unknown;
    }
}
