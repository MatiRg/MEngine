#pragma once
#include "../Resources/Resource.hpp"
#include "../Resources/ResourceFactory.hpp"
#include "../Math/Rect2.hpp"
#include "../Math/Color.hpp"
#include "../Math/Matrix3.hpp"
#include <memory>

class ISurface;
class IGraphics;
class CRenderer2D;

// Same As C# TextureFlip
enum class ETextureFlip
{
    None = 0,
    Horizontal,
    Vertical,
    Both
};

class ITexture2D: public IResource
{
public:
    ITexture2D(const std::string& Name, CResources* Resources):
        IResource( Name, Resources )
    {
    }

    virtual ~ITexture2D() 
    {
    }

    RESOURCE(ITexture2D)

    virtual bool CreateFromSurface(ISurface*) = 0;

    virtual int GetWidth() const = 0;
    virtual int GetHeight() const = 0;

    void SetColorMod(const Color& aColor) { ColorMod = aColor; } 
    const Color& GetColorMod() const { return ColorMod; }
protected:
    Color ColorMod = Color(1.0f);
};

//

class CTexture2DFactory: public TResourceFactory<ITexture2D>
{
public:
    CTexture2DFactory(IGraphics*);

    std::unique_ptr<IResource> CreateResource(const std::string&, CResources*) override;
private:
    IGraphics* Graphics = nullptr;
};