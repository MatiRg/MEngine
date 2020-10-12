#pragma once

enum class EImGUIWindowFlags
{
    None = 0,
    NoTitleBar = 1 << 0,
    NoResize = 1 << 1,
    NoMove = 1 << 2,
    NoCollapse = 1 << 3,
};

inline EImGUIWindowFlags operator|(EImGUIWindowFlags a, EImGUIWindowFlags b)
{
    return static_cast<EImGUIWindowFlags>( static_cast<int>(a) | static_cast<int>(b) );
}

inline EImGUIWindowFlags& operator|=(EImGUIWindowFlags& a, EImGUIWindowFlags b)
{
    a = static_cast<EImGUIWindowFlags>( static_cast<int>(a) | static_cast<int>(b) );
    return a;
}

inline bool operator&(EImGUIWindowFlags a, EImGUIWindowFlags b)
{
    return (static_cast<int>(a) & static_cast<int>(b)) != 0;
}