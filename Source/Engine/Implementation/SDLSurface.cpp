#include "SDLSurface.hpp"
#include "../Core/Log.hpp"
#include "../Core/Utils.hpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

CSDLSurface::CSDLSurface()
{
}

CSDLSurface::~CSDLSurface()
{
    Destroy();
}

bool CSDLSurface::Create(SDL_Surface* aSurface)
{
    if( !aSurface )
    {
        LOG( ESeverity::Error ) << "Invalid Source Surface\n";
        return false;
    }
    Destroy();

    Surface = aSurface;
    Width = Surface->w;
    Height = Surface->h;
    BPP = Surface->format->BitsPerPixel;
    return true;
}

bool CSDLSurface::Create(const int aWidth, const int aHeight)
{
    if( aWidth <= 0 || aHeight <= 0 )
    {
        LOG( ESeverity::Error ) << "Invalid Surface Size\n";
        return false;
    }
    Destroy();

    Width = aWidth;
    Height = aHeight;
    Uint32 RMask, GMask, BMask, AMask;
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    RMask = 0xff000000;
    GMask = 0x00ff0000;
    BMask = 0x0000ff00;
    AMask = 0x000000ff;
#else
    RMask = 0x000000ff;
    GMask = 0x0000ff00;
    BMask = 0x00ff0000;
    AMask = 0xff000000;
#endif
    Surface = SDL_CreateRGBSurface( 0, Width, Height, 32, RMask, GMask, BMask, AMask );
    if( !Surface )
    {
        LOG( ESeverity::Error ) << "Unable to create surface: " << SDL_GetError() << "\n";
        return false;
    }
    BPP = Surface->format->BitsPerPixel;
    return true;
}

bool CSDLSurface::Load(const std::string& Path)
{
    if( Path.empty() )
    {
        LOG( ESeverity::Error ) << "Invalid Path for Surface loading\n";
        return false;
    }
    Destroy();

    Surface = IMG_Load( Path.c_str() );
    if( !Surface )
    {
        LOG( ESeverity::Error ) << "Unable to load surface: " << IMG_GetError() << "\n";
        return false;
    }
    Width = Surface->w;
    Height = Surface->h;
    BPP = Surface->format->BitsPerPixel;
    return true;
}

bool CSDLSurface::Save(const std::string& Path)
{
    if( Path.empty() )
    {
        LOG( ESeverity::Error ) << "Invalid Path\n";
        return false;
    }
    if( IsValid() )
    {
        if( IMG_SavePNG( Surface, Path.c_str() ) != 0 )
        {
            LOG( ESeverity::Error ) << "Unable to Save PNG:" << IMG_GetError() << "\n";
            return false;
        }
        return true;
    }
    LOG( ESeverity::Error ) << "Invalid Window Surface\n";
    return false;
}

bool CSDLSurface::SetColorKey(const Color& aColorKey)
{
    if( !IsValid() )
    {
        LOG( ESeverity::Error ) << "Invalid Surface\n";
        return false;
    }

    SDL_SetColorKey( Surface, SDL_TRUE, SDL_MapRGBA( Surface->format, aColorKey.GetR_UInt(), aColorKey.GetG_UInt(), 
        aColorKey.GetB_UInt(), aColorKey.GetA_UInt() ) );
    ColorKey = aColorKey;
    return true;
}

bool CSDLSurface::Fill(const Color& aColor)
{
    if( !IsValid() )
    {
        LOG( ESeverity::Error ) << "Invalid Surface\n";
        return false;
    }

    if( SDL_FillRect( Surface, nullptr, SDL_MapRGBA(Surface->format, aColor.GetR_UInt(), aColor.GetG_UInt(), aColor.GetB_UInt(), aColor.GetA_UInt() ) ) != 0 )
    {
        LOG( ESeverity::Error ) << "SDL: " << SDL_GetError() << "\n";
        return false;
    }
    return true;
}

bool CSDLSurface::Fill(const Color& aColor, const Rect2& Rect)
{
    if( !IsValid() )
    {
        LOG( ESeverity::Error ) << "Invalid Surface\n";
        return false;
    }

    SDL_Rect Dst{ std::lround( Rect.GetX() ), std::lround( Rect.GetY() ), std::lround( Rect.GetWidth() ), std::lround( Rect.GetHeight() ) };
    if( SDL_FillRect( Surface, &Dst, SDL_MapRGBA(Surface->format, aColor.GetR_UInt(), aColor.GetG_UInt(), aColor.GetB_UInt(), aColor.GetA_UInt() ) ) != 0 )
    {
        LOG( ESeverity::Error ) << "SDL: " << SDL_GetError() << "\n";
        return false;
    }
    return true;
}

bool CSDLSurface::Blit(ISurface* aSurface, const Rect2& aDestRect)
{
    if( !IsValid() )
    {
        LOG( ESeverity::Error ) << "Invalid Surface\n";
        return false;
    }

    CSDLSurface* SDLSurface = dynamic_cast<CSDLSurface*>( aSurface );
    if( !SDLSurface )
    {
        LOG( ESeverity::Error ) << "Invalid Source Surface\n";
        return false;
    }

    SDL_Rect Dst{ std::lround( aDestRect.GetX() ), std::lround( aDestRect.GetY() ), std::lround( aDestRect.GetWidth() ), std::lround( aDestRect.GetHeight() ) };
    if( SDL_BlitScaled( SDLSurface->GetSDLSurface(), nullptr, Surface, &Dst ) != 0 )
    {
        LOG( ESeverity::Error ) << "SDL: " << SDL_GetError() << "\n";
        return false;
    }
    return true;
}

void CSDLSurface::Destroy()
{
    if( IsValid() )
    {
        SDL_FreeSurface( Surface );
        Surface = nullptr;
    }
}
