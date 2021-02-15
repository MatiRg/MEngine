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

bool COGLTexture2D::CreateAsRenderSurface(const ERenderTargetType aTarget, const int aW, const int aH, const int MSAASamples)
{
    RenderTargetFlag = true;
    RenderTarget = aTarget;
    Width = aW;
    Height = aH;
    TextureType = RenderTarget == ERenderTargetType::Color || RenderTarget == ERenderTargetType::Depth ? GL_TEXTURE_2D : GL_TEXTURE_2D_MULTISAMPLE;

    glGenTextures(1, &Handle);
    glBindTexture(TextureType, Handle);

    if (RenderTarget == ERenderTargetType::Color )
    {
        glTexImage2D(TextureType, 0, GL_RGB, Width, Height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    }
    else if (RenderTarget == ERenderTargetType::Color_MSAA)
    {
        glTexImage2DMultisample(TextureType, MSAASamples, GL_RGB, Width, Height, GL_TRUE); // TO DO: Samples from File
    }
    else if (RenderTarget == ERenderTargetType::Depth)
    {
        glTexImage2D(TextureType, 0, GL_DEPTH_COMPONENT24, Width, Height, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, nullptr);
    }
    else // Depth MSAA
    {
        glTexImage2DMultisample(TextureType, MSAASamples, GL_DEPTH_COMPONENT24, Width, Height, GL_TRUE); // TO DO: Samples from File
    }

    glBindTexture(TextureType, 0);

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
    TextureType = GL_TEXTURE_2D;

    glGenTextures( 1, &Handle );
    glBindTexture(TextureType, Handle );

    std::shared_ptr<CSDLSurface> Converted = std::make_shared<CSDLSurface>();
    if( !Converted->Create( Width, Height ) )
    {
        LOG( ESeverity::Error ) << "Unable to convert surface\n";
        glBindTexture(TextureType, 0 );
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
            glBindTexture(TextureType, 0 );
            return false;
    }

    glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );
    if( SDL_MUSTLOCK( Converted->GetSDLSurface() ) )
    {
        SDL_LockSurface( Converted->GetSDLSurface() );
    }

    glTexImage2D(TextureType, 0, GL_RGBA, Width, Height, 0, Fmt, GL_UNSIGNED_BYTE, Converted->GetSDLSurface()->pixels );

    if( SDL_MUSTLOCK( Converted->GetSDLSurface() ) )
    {
        SDL_UnlockSurface( Converted->GetSDLSurface() );
    }
    glPixelStorei( GL_UNPACK_ALIGNMENT, 4 ); // 4 Default value

    glBindTexture(TextureType, 0 );

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
        glBindTexture(TextureType, Handle);
        glTexParameteri(TextureType, GL_TEXTURE_WRAP_S, ToOGLWrap(WrapS));
        glBindTexture(TextureType, 0);
        OGL::CheckErrorOpenGL();
    }
}

void COGLTexture2D::SetWrapT(const ETextureWrap Arg)
{
    if (IsValid())
    {
        WrapT = Arg;
        glBindTexture(TextureType, Handle);
        glTexParameteri(TextureType, GL_TEXTURE_WRAP_T, ToOGLWrap(WrapT));
        glBindTexture(TextureType, 0);
        OGL::CheckErrorOpenGL();
    }
}

void COGLTexture2D::SetFilter(const ETextureFilter Arg)
{
    if (IsValid())
    {
        Filter = Arg;
        glBindTexture(TextureType, Handle);
        if (Filter == ETextureFilter::Linear)
        {
            glTexParameteri(TextureType, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(TextureType, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        }
        else if (Filter == ETextureFilter::Bilinear)
        {
            glTexParameteri(TextureType, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(TextureType, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        }
        else // Wrong?
        {
            glTexParameteri(TextureType, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(TextureType, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glGenerateMipmap(TextureType);
        }
        glBindTexture(TextureType, 0);
        OGL::CheckErrorOpenGL();
    }
}

void COGLTexture2D::SetAnisotropicFiltering(const float Arg)
{
    if (IsValid())
    {
        Anisotropic = Math::Clamp(Arg, 1.0f, 8.0f);
        glBindTexture(TextureType, Handle);
        // Wrong?
        glTexParameterf(TextureType, GL_TEXTURE_MAX_ANISOTROPY, Anisotropic);
        glBindTexture(TextureType, 0);
        OGL::CheckErrorOpenGL();
    }
}