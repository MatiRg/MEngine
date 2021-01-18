#pragma once
#include "VertexBuffer.hpp"
#include "Texture2D.hpp"
#include "../Engine/EngineModule.hpp"
#include "../Math/Rect2.hpp"
#include "../Math/Color.hpp"
#include "../UI/ImGUIRenderer.hpp"
#include <string>
#include <memory>

class ISurface;
class IFont;
class IShader;
class IFrameBuffer;
class CResources;

enum class EBlendMode
{
    None,
    Alpha,
    Add,
    Mod
};

enum class EPolygonMode
{
    Point,
    Line,
    Fill
};

enum class EDepthMode
{
    Never,
    Less,
    LessOrEqual,
    Greater,
    GreaterOrEqual,
    Equal,
    NotEqual,
    Always
};

enum class ECullMode
{
    Front,
    Back,
    FrontBack
};

enum class EFrontFace
{
    CW, // Clockwise
    CCW // Counterclockwise
};

class IGraphics: public IEngineModule
{
public:
    IGraphics(): IEngineModule( "Graphics" ) {}
    virtual ~IGraphics() {}

    ENGINE_MODULE(IGraphics)

    virtual bool SaveWindowSurface(const std::string&) const = 0;

    virtual const IntVector2& GetMaxTextureSize() const = 0;
    virtual bool IsSoftware() const = 0;

    virtual std::unique_ptr<ITexture2D> CreateRenderSurface(const ERenderTargetType, const int, const int) = 0;
    // Create From Surface
    virtual std::unique_ptr<ITexture2D> CreateTexture2D(ISurface*) = 0;
    // Name - Resources Usage
    virtual std::unique_ptr<ITexture2D> CreateTexture2D(const std::string&) = 0;
    // Name - Resources Usage
    virtual std::unique_ptr<IFont> CreateFont(const std::string&) = 0;
    // Name - Resources Usage
    virtual std::unique_ptr<IShader> CreateShader(const std::string&) = 0;

    // Vertex Buffer Description, IsDynamic
    virtual std::unique_ptr<IVertexBuffer> CreateVertexBuffer(const std::vector<SVertexElement>&, const bool) = 0;

    //
    virtual std::unique_ptr<IFrameBuffer> CreateFrameBuffer() = 0;

    virtual void SetClearColor(const Color&) = 0;
    virtual const Color& GetClearColor() const = 0;

    virtual void Clear() = 0;
    virtual void SwapBuffers() = 0;

    virtual void SetBlendActive(const bool) = 0;
    virtual bool IsBlendActive() const = 0;

    virtual bool SetBlendMode(const EBlendMode) = 0;
    virtual EBlendMode GetBlendMode() const = 0;

    virtual bool SetPolygonMode(const EPolygonMode) = 0;
    virtual EPolygonMode GetPolygonMode() const = 0;

    virtual bool SetDepthActive(const bool) = 0;
    virtual bool IsDepthActive() const = 0;

    virtual bool SetDepthFunction(const EDepthMode) = 0;
    virtual EDepthMode GetDepthFunction() const = 0;

    virtual void SetCullActive(const bool) = 0;
    virtual bool IsCullActive() const = 0;

    virtual void SetCullMode(const ECullMode) = 0;
    virtual ECullMode GetCullMode() const = 0;

    virtual void SetFrontFace(const EFrontFace) = 0;
    virtual EFrontFace GetFrontFace() const = 0;

    // [0...1] Normalized
    virtual bool SetViewport(const Rect2&) = 0;
    virtual const Rect2& GetViewport() const = 0;

    virtual IImGUIRenderer* GetImGUIRenderer() const = 0;
};

