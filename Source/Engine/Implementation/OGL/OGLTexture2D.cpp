#include "OGLTexture2D.hpp"
#include "OGLGraphics.hpp"
#include "OGLShader.hpp"
#include "../SDLSurface.hpp"
#include "../../Graphics/Renderer2D.hpp"
#include "../../Graphics/VertexBuffer.hpp"
#include "../../Core/Log.hpp"
#include "../../Core/Utils.hpp"

COGLTexture2D::COGLTexture2D(const std::string& Name, CResources* Resources, COGLGraphics* aGraphics):
    ITexture2D( Name, Resources ),
    Graphics( aGraphics )
{
}

COGLTexture2D::~COGLTexture2D()
{
    glDeleteTextures( 1, &Handle );
}

bool COGLTexture2D::CreateFromSurface(ISurface* Surface)
{
    CSDLSurface* SDLSurface = dynamic_cast<CSDLSurface*>( Surface );
    if( !SDLSurface || !SDLSurface->GetSDLSurface() )
    {
        LOG( ESeverity::Error ) << "Invalid surface\n";
        return false;
    }

    Width = SDLSurface->GetWidth();
    Height = SDLSurface->GetHeight();

    glGenTextures( 1, &Handle );
    glBindTexture( GL_TEXTURE_2D, Handle );

    std::shared_ptr<CSDLSurface> Converted = std::make_shared<CSDLSurface>();
    if( !Converted->Create( Width, Height ) )
    {
        LOG( ESeverity::Error ) << "Unable to convert surface\n";
        glBindTexture( GL_TEXTURE_2D, 0 );
        return false;
    }
    Converted->Blit( SDLSurface, {0.0f, 0.0f, static_cast<float>(Width), static_cast<float>(Height)} );

    GLenum Fmt;
    switch(Converted->GetSDLSurface()->format->BytesPerPixel)
    {
        case 3:
            Fmt = GL_RGB;
            break;
        case 4:
            Fmt = GL_RGBA;
            break;
        default:
            LOG( ESeverity::Error ) << "Invalid SDL Surface Format\n";
            glBindTexture( GL_TEXTURE_2D, 0 );
            return false;
    }

    glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );
    if( SDL_MUSTLOCK( Converted->GetSDLSurface() ) )
    {
        SDL_LockSurface( Converted->GetSDLSurface() );
    }

    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, Width, Height, 0, Fmt, GL_UNSIGNED_BYTE, Converted->GetSDLSurface()->pixels );

    if( SDL_MUSTLOCK( Converted->GetSDLSurface() ) )
    {
        SDL_UnlockSurface( Converted->GetSDLSurface() );
    }
    glPixelStorei( GL_UNPACK_ALIGNMENT, 4 ); // 4 Default value

    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

    glBindTexture( GL_TEXTURE_2D, 0 );

    OGL::CheckErrorOpenGL();

    Valid = true;
    return Valid;
}

bool COGLTexture2D::Load()
{
    std::unique_ptr<CSDLSurface> Surface = std::make_unique<CSDLSurface>();
    if( Surface && Surface->Load( GetPath() ) )
    {
        if( CreateFromSurface( Surface.get() ) )
        {
            return true;
        }
    }
    return false;
}