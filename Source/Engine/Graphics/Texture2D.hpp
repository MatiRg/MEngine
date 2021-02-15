#pragma once
#include "../Resources/ResourceManager.hpp"
#include "../Math/Rect2.hpp"
#include "../Math/Color.hpp"
#include "../Math/Matrix3.hpp"
#include <memory>

class ISurface;
class IGraphics;

// Same As C# TextureFlip
enum class ETextureFlip
{
    None = 0,
    Horizontal,
    Vertical,
    Both
};

enum class ETextureWrap
{
    Repeat,
    MirroredRepeat,
    ClampToEdge,
    ClampToBorder
};

enum class ETextureFilter
{
    Linear,
    Bilinear,
    Trilinear // Bilinear + Mipmaps
};

enum class ERenderTargetType
{
    Color,
    Depth,
    Color_MSAA,
    Depth_MSAA
};

class ITexture2D: public IResource
{
public:
    ITexture2D(const std::string& Name):
        IResource( Name )
    {
    }

    virtual ~ITexture2D() 
    {
    }

    RESOURCE(ITexture2D)

    // Render Target Type, Width, Height, MSAA Samples
    virtual bool CreateAsRenderSurface(const ERenderTargetType, const int, const int, const int) = 0;
    virtual bool CreateFromSurface(ISurface*) = 0;

    virtual bool IsRenderTarget() const = 0;
    virtual ERenderTargetType GetRenderTargetType() const = 0;

    virtual int GetWidth() const = 0;
    virtual int GetHeight() const = 0;

    void SetColorMod(const Color& aColor) { ColorMod = aColor; } 
    const Color& GetColorMod() const { return ColorMod; }

    virtual void SetWrapS(const ETextureWrap) = 0;
    virtual ETextureWrap GetWrapS() const = 0;

    virtual void SetWrapT(const ETextureWrap) = 0;
    virtual ETextureWrap GetWrapT() const = 0;

    void SetWrap(const ETextureWrap);

    virtual void SetFilter(const ETextureFilter) = 0;
    virtual ETextureFilter GetFilter() const = 0;

    virtual void SetAnisotropicFiltering(const float) = 0;
    virtual float GetAnisotropicFiltering() const = 0;
protected:
    Color ColorMod = Color(1.0f);
};

//

class CTextureManager: public TResourceManager<ITexture2D>
{
public:
    CTextureManager(IGraphics*);

    RESOURCE_MANAGER(CTextureManager)
protected:
    std::unique_ptr<IResource> MakeResource(const std::string&, const ResourceCreateMap&) override;
private:
    IGraphics* Graphics = nullptr;
};