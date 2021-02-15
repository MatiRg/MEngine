#pragma once
#include "../../Graphics/FrameBuffer.hpp"
#include "OGLUtils.hpp"
#include <memory>

class COGLGraphics;
class CSDLWindow;

class COGLFrameBuffer final: public IFrameBuffer
{
public:
    COGLFrameBuffer(COGLGraphics*, CSDLWindow*);
    ~COGLFrameBuffer();

    bool Create(const bool, const int) override;

    bool IsValid() const override { return Valid; }

    bool IsMSAA() const override { return MSAA; }

    ITexture2D* GetColorAttachment() const override { return ColorAttachment.get(); }
    ITexture2D* GetDepthAttachment() const override { return DepthAttachment.get(); }

    void Bind() override;
    void UnBind() override;

    void Blit(IFrameBuffer*) override;
private:
    COGLGraphics* Graphics = nullptr;
    CSDLWindow* Window = nullptr;
    bool Valid = false;
    GLuint FBO = 0u;
    std::unique_ptr<ITexture2D> ColorAttachment;
    std::unique_ptr<ITexture2D> DepthAttachment;
    bool MSAA = false;
    IntVector2 Size;
};
