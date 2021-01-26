#pragma once

//! Event type eg. Keyboard, Mouse ...
enum class EEventType
{
    Null = 0, //! No Data, For Empty Event
    Quit, //! No Data 
    Keyboard, //! For SEventKeyboard
    MouseButton, //! For SEventMouseButton
    MouseMotion, //! For SEventMouseMotion
    MouseWheel, //! For SEventMouseWheel
    TextInput, //! For SEventTextInput
	WindowSize, //! For SEventWindowSize
	Custom //! Must inherits from SEventData
};

//! Keyboard Key Enum
enum class EKey
{
	A = 0,
	B,
	C,
	D,
	E,
	F,
	G,
	H,
	I,
	J,
	K,
	L,
	M,
	N,
	O,
	P,
	Q,
	R,
	S,
	T,
	U,
	V,
	W,
	X,
	Y,
	Z,
	Num0,
	Num1,
	Num2,
	Num3,
	Num4,
	Num5,
	Num6,
	Num7,
	Num8,
	Num9,
	Escape,
	Space,
	Return,
	BackSpace,
	Tab,
	LControl,
	LShift,
	LAlt,
	RControl,
	RShift,
	RAlt,
	Left,
	Right,
	Up,
	Down,
	F1,
	F2,
	F3,
	F4,
	F5,
	F6,
	F7,
	F8,
	F9,
	F10,
	F11,
	F12,
	PageUp,
	PageDown,
	Home,
	End,
	Insert,
	Delete,
	Unknown,
	Any, //! Any key from up
    Max
};

//! Keyboard Mod eg. Shift, Alt
enum class EKeyMod
{
    None  = 0,
    Alt   = 1<<0,
    Ctrl  = 1<<1,
    Shift = 1<<2
};

inline EKeyMod& operator|=(EKeyMod& lhs, const EKeyMod rhs)
{
    lhs = static_cast<EKeyMod>( static_cast<int>(lhs) | static_cast<int>(rhs) );
    return lhs;
}

inline EKeyMod operator|(const EKeyMod lhs, const EKeyMod rhs)
{
    return static_cast<EKeyMod>( static_cast<int>(lhs) | static_cast<int>(rhs) );
}

inline bool operator&(const EKeyMod lhs, const EKeyMod rhs)
{
    return static_cast<int>(lhs) & static_cast<int>(rhs);
}

//! Mouse Key Enum
enum class EMouseKey
{
    LButton = 0,
    MButton,
    RButton,
    Unknown,
    Any, //! Any key from up
    Max
};
