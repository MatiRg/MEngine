#pragma once
#include "../../Graphics/Texture2D.hpp"
#include "OGLUtils.hpp"

class COGLGraphics;

class COGLTexture2D: public ITexture2D
{
public:
    COGLTexture2D(const std::string&, COGLGraphics*);
    ~COGLTexture2D();

    bool Load(CResources*, const ResourceCreateMap&) override;
    bool CreateAsRenderSurface(const ERenderTargetType, const int, const int) override;
    bool CreateFromSurface(ISurface*) override;

    bool IsRenderTarget() const override { return RenderTargetFlag; }
    ERenderTargetType GetRenderTargetType() const override { return RenderTarget; }

    int GetWidth() const override { return Width; }
    int GetHeight() const override { return Height; }

    void SetWrapS(const ETextureWrap) override;
    ETextureWrap GetWrapS() const override { return WrapS; }

    void SetWrapT(const ETextureWrap) override;
    ETextureWrap GetWrapT() const override { return WrapT; }

    void SetFilter(const ETextureFilter) override;
    ETextureFilter GetFilter() const override { return Filter; }

    void SetAnisotropicFiltering(const float) override;
    float GetAnisotropicFiltering() const override { return Anisotropic; }

    GLuint GetHandle() const { return Handle; }
private:
    COGLGraphics* Graphics = nullptr;
    int Width = 0;
    int Height = 0;
    GLuint Handle = 0u;
    ETextureWrap WrapS = ETextureWrap::ClampToEdge;
    ETextureWrap WrapT = ETextureWrap::ClampToEdge;
    ETextureFilter Filter = ETextureFilter::Linear;
    float Anisotropic = 1.0f; // 1 - No filtering
    bool RenderTargetFlag = false;
    ERenderTargetType RenderTarget = ERenderTargetType::Color;
};
