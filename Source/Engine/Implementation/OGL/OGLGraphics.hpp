#pragma once
#include "../../Graphics/Graphics.hpp"
#include <memory>
#include <SDL2/SDL_video.h>

class CSDLWindow;
class COGLBuffer2D;
class COGLShader;

class COGLGraphics: public IGraphics
{
public:
    COGLGraphics(CSDLWindow*);
    ~COGLGraphics();

    bool Init(const SEngineParams&) override;
    void OnEvent(const CEvent&) override;

    bool SaveWindowSurface(const std::string&) const override;

    const IntVector2& GetMaxTextureSize() const override { return MaxTextureSize; }
    bool IsSoftware() const override { return false; }

    std::unique_ptr<ITexture2D> CreateTexture2D(CResources*) override;
    std::unique_ptr<ITexture2D> CreateTexture2D(const std::string&, CResources*) override;
    std::unique_ptr<IFont> CreateFont(const std::string&, CResources*) override;

    // Vertex Buffer, Arg: IsDynamic
    std::unique_ptr<IVertexBufferColorTexCoords> CreateVertexBufferColorCoords(const bool) override;
    //
    std::unique_ptr<IShader> CreateShader(const std::string&, CResources*) override;

    void SetClearColor(const Color& ClearCr) override { ClearColor = ClearCr; }
    const Color& GetClearColor() const override { return ClearColor; }

    void Clear() override;
    void SwapBuffers() override;

    bool SetBlendMode(const EBlendMode) override;
    EBlendMode GetBlendMode() const override { return BlendMode; }

    bool SetPolygonMode(const EPolygonMode) override;
    EPolygonMode GetPolygonMode() const override { return PolygonMode; }

    bool SetDepthActive(const bool) override;
    bool IsDepthActive() const override { return DepthActive; }

    bool SetDepthFunction(const EDepthMode) override;
    EDepthMode GetDepthFunction() const override { return DepthMode; }

    bool SetViewport(const Rect2&) override;
    const Rect2& GetViewport() const override { return Viewport; }

    IImGUIRenderer* GetImGUIRenderer() const override;
private:
    CSDLWindow* Window = nullptr;
    SDL_GLContext Context = nullptr;
    Color ClearColor = Color(0.0f, 1.0f);
    EBlendMode BlendMode = EBlendMode::None;
    Rect2 Viewport;
    IntVector2 MaxTextureSize;
    std::unique_ptr<IImGUIRenderer> ImGUIRenderer;
    EPolygonMode PolygonMode = EPolygonMode::Fill;
    bool DepthActive = false;
    EDepthMode DepthMode = EDepthMode::Less;
};
