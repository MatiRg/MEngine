#include "OGLTexture2D.hpp"
#include "OGLGraphics.hpp"
#include "OGLShader.hpp"
#include "../SDLSurface.hpp"
#include "../../Math/Functions.hpp"
#include "../../Graphics/VertexBuffer.hpp"
#include "../../Core/Log.hpp"
#include "../../Core/Utils.hpp"

namespace
{
    GLenum ToOGLWrap(ETextureWrap Val)
    {
        switch (Val)
        {
        case ETextureWrap::Repeat:
            return GL_REPEAT;
        case ETextureWrap::MirroredRepeat:
            return GL_MIRRORED_REPEAT;
        case ETextureWrap::ClampToEdge:
            return GL_CLAMP_TO_EDGE;
        case ETextureWrap::ClampToBorder:
            return GL_CLAMP_TO_BORDER;
        default:
            return GL_CLAMP_TO_EDGE;
        }
    }
}

COGLTexture2D::COGLTexture2D(const std::string& Name, COGLGraphics* aGraphics):
    ITexture2D( Name ),
    Graphics( aGraphics )
{
}

COGLTexture2D::~COGLTexture2D()
{
    glDeleteTextures( 1, &Handle );
}

bool COGLTexture2D::CreateAsRenderSurface(const ERenderTargetType aTarget, const int aW, const int aH)
{
    RenderTargetFlag = true;
    RenderTarget = aTarget;
    Width = aW;
    Height = aH;

    glGenTextures(1, &Handle);
    glBindTexture(GL_TEXTURE_2D, Handle);

    if (RenderTarget == ERenderTargetType::Color)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, Width, Height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    }
    else // Depth
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, Width, Height, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, nullptr);
    }

    glBindTexture(GL_TEXTURE_2D, 0);

    OGL::CheckErrorOpenGL();
    Valid = true;

    SetWrap(ETextureWrap::ClampToEdge);
    SetFilter(ETextureFilter::Bilinear);
    SetAnisotropicFiltering(1.0f);

    return true;
}

bool COGLTexture2D::CreateFromSurface(ISurface* Surface)
{
    CSDLSurface* SDLSurface = dynamic_cast<CSDLSurface*>( Surface );
    if( !SDLSurface || !SDLSurface->GetSDLSurface() )
    {
        LOG( ESeverity::Error ) << "Invalid surface\n";
        return false;
    }

    RenderTargetFlag = false;
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

    glBindTexture( GL_TEXTURE_2D, 0 );

    OGL::CheckErrorOpenGL();
    Valid = true;

    SetWrap(ETextureWrap::ClampToEdge);
    SetFilter(ETextureFilter::Bilinear);
    SetAnisotropicFiltering(1.0f);

    return Valid;
}

bool COGLTexture2D::Load(CResources*, const ResourceCreateMap&)
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

void COGLTexture2D::SetWrapS(const ETextureWrap Arg)
{
    if (IsValid())
    {
        WrapS = Arg;
        glBindTexture(GL_TEXTURE_2D, Handle);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, ToOGLWrap(WrapS));
        glBindTexture(GL_TEXTURE_2D, 0);
        OGL::CheckErrorOpenGL();
    }
}

void COGLTexture2D::SetWrapT(const ETextureWrap Arg)
{
    if (IsValid())
    {
        WrapT = Arg;
        glBindTexture(GL_TEXTURE_2D, Handle);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, ToOGLWrap(WrapT));
        glBindTexture(GL_TEXTURE_2D, 0);
        OGL::CheckErrorOpenGL();
    }
}

void COGLTexture2D::SetFilter(const ETextureFilter Arg)
{
    if (IsValid())
    {
        Filter = Arg;
        glBindTexture(GL_TEXTURE_2D, Handle);
        if (Filter == ETextureFilter::Linear)
        {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        }
        else if (Filter == ETextureFilter::Bilinear)
        {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        }
        else // Wrong?
        {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        glBindTexture(GL_TEXTURE_2D, 0);
        OGL::CheckErrorOpenGL();
    }
}

void COGLTexture2D::SetAnisotropicFiltering(const float Arg)
{
    if (IsValid())
    {
        Anisotropic = Math::Clamp(Arg, 1.0f, 8.0f);
        glBindTexture(GL_TEXTURE_2D, Handle);
        // Wrong?
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY, Anisotropic);
        glBindTexture(GL_TEXTURE_2D, 0);
        OGL::CheckErrorOpenGL();
    }
}