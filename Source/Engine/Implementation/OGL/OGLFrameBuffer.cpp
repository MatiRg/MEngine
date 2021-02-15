#include "OGLFrameBuffer.hpp"
#include "OGLGraphics.hpp"
#include "OGLTexture2D.hpp"
#include "../SDLWindow.hpp"
#include "../../Core/Log.hpp"

COGLFrameBuffer::COGLFrameBuffer(COGLGraphics* aGraphics, CSDLWindow* aWindow):
    Graphics(aGraphics),
    Window(aWindow)
{
    LOG( ESeverity::Debug ) << "OGL Frame Buffer - Created\n";
}

COGLFrameBuffer::~COGLFrameBuffer()
{
    ColorAttachment.reset();
    DepthAttachment.reset();
    glDeleteFramebuffers(1, &FBO);
    LOG( ESeverity::Debug ) << "OGL Frame Buffer - Destroyed\n";
}

bool COGLFrameBuffer::Create(const bool aMSAA, const int Samples)
{
    MSAA = aMSAA;
    Size = Window->GetSize();

    ColorAttachment = Graphics->CreateRenderSurface(MSAA ? ERenderTargetType::Color_MSAA : ERenderTargetType::Color, Size.x, Size.y, Samples);
    if (!ColorAttachment)
    {
        LOG(ESeverity::Error) << "Unable to Create Color Render Surface\n";
        return false;
    }

    DepthAttachment = Graphics->CreateRenderSurface(MSAA ? ERenderTargetType::Depth_MSAA : ERenderTargetType::Depth, Size.x, Size.y, Samples);
    if (!DepthAttachment)
    {
        LOG(ESeverity::Error) << "Unable to Create Depth Render Surface\n";
        return false;
    }

    glGenFramebuffers(1, &FBO);
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, MSAA ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D, static_cast<COGLTexture2D*>(ColorAttachment.get())->GetHandle(), 0);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, MSAA ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D, static_cast<COGLTexture2D*>(DepthAttachment.get())->GetHandle(), 0);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        LOG(ESeverity::Error) << "ICcomplete Frame Buffer\n";
        glBindFramebuffer(GL_FRAMEBUFFER, 0u);
        return false;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0u);

    OGL::CheckErrorOpenGL();

    Valid = true;
    return true;
}

void COGLFrameBuffer::Bind()
{
    if( IsValid() )
    {
        glBindFramebuffer(GL_FRAMEBUFFER, FBO);
        OGL::CheckErrorOpenGL();
    }
}

void COGLFrameBuffer::UnBind()
{
    if( IsValid() )
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0u);
        OGL::CheckErrorOpenGL();
    }
}

void COGLFrameBuffer::Blit(IFrameBuffer* aOther)
{
    COGLFrameBuffer* Other = static_cast<COGLFrameBuffer*>(aOther);
    if (IsValid() && Other)
    {
        glBindFramebuffer(GL_READ_FRAMEBUFFER, FBO);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, Other->FBO);
        //
        glBlitFramebuffer(0, 0, Size.x, Size.y, 0, 0, Other->Size.x, Other->Size.y, GL_COLOR_BUFFER_BIT, GL_NEAREST);
        glBlitFramebuffer(0, 0, Size.x, Size.y, 0, 0, Other->Size.x, Other->Size.y, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
        //
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
}