#pragma once
#include "../Engine/EngineModule.hpp"
#include "../Math/Rect2.hpp"
#include "../Math/Color.hpp"
#include "../UI/ImGUIRenderer.hpp"
#include <string>
#include <memory>

class ITexture2D;
class ISurface;
class IFont;
class IVertexBufferColorTexCoords;
class IShader;
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

class IGraphics: public IEngineModule
{
public:
    IGraphics(): IEngineModule( "Graphics" ) {}
    virtual ~IGraphics() {}

    ENGINE_MODULE(IGraphics)

    virtual bool SaveWindowSurface(const std::string&) const = 0;

    virtual const IntVector2& GetMaxTextureSize() const = 0;
    virtual bool IsSoftware() const = 0;

    // Create Empty Texture
    virtual std::unique_ptr<ITexture2D> CreateTexture2D(CResources*) = 0;
    // Name, Path
    virtual std::unique_ptr<ITexture2D> CreateTexture2D(const std::string&, CResources*) = 0;
    // Name, Path
    virtual std::unique_ptr<IFont> CreateFont(const std::string&, CResources*) = 0;

    // Vertex Buffer, Arg: IsDynamic
    virtual std::unique_ptr<IVertexBufferColorTexCoords> CreateVertexBufferColorCoords(const bool) = 0;
    //
    virtual std::unique_ptr<IShader> CreateShader(const std::string&, CResources*) = 0;

    virtual void SetClearColor(const Color&) = 0;
    virtual const Color& GetClearColor() const = 0;

    virtual void Clear() = 0;
    virtual void SwapBuffers() = 0;

    virtual bool SetBlendMode(const EBlendMode) = 0;
    virtual EBlendMode GetBlendMode() const = 0;

    virtual bool SetPolygonMode(const EPolygonMode) = 0;
    virtual EPolygonMode GetPolygonMode() const = 0;

    virtual bool SetDepthActive(const bool) = 0;
    virtual bool IsDepthActive() const = 0;

    virtual bool SetDepthFunction(const EDepthMode) = 0;
    virtual EDepthMode GetDepthFunction() const = 0;

    // [0...1] Normalized
    virtual bool SetViewport(const Rect2&) = 0;
    virtual const Rect2& GetViewport() const = 0;

    virtual IImGUIRenderer* GetImGUIRenderer() const = 0;
};

