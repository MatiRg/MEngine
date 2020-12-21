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

bool COGLFrameBuffer::Create()
{
    ColorAttachment = Graphics->CreateRenderSurface(ERenderTargetType::Color, Window->GetSize().x, Window->GetSize().y);
    if (!ColorAttachment)
    {
        LOG(ESeverity::Error) << "Unable to Create Color Render Surface\n";
        return false;
    }

    DepthAttachment = Graphics->CreateRenderSurface(ERenderTargetType::Depth, Window->GetSize().x, Window->GetSize().y);
    if (!DepthAttachment)
    {
        LOG(ESeverity::Error) << "Unable to Create Depth Render Surface\n";
        return false;
    }

    glGenFramebuffers(1, &FBO);
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, static_cast<COGLTexture2D*>(ColorAttachment.get())->GetHandle(), 0);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, static_cast<COGLTexture2D*>(DepthAttachment.get())->GetHandle(), 0);

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
