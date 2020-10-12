#pragma once
#include "../Resources/Resource.hpp"
#include "../Resources/ResourceFactory.hpp"
#include "../Math/Rect2.hpp"
#include "../Math/Color.hpp"
#include "../Math/Matrix3.hpp"
#include "Texture2D.hpp"
#include <string>
#include <map>
#include <vector>
#include <memory>

class IGraphics;
class CRenderer2D;

struct SGlyph
{
    int Advance = 0;
    int Texture = 0;
    Rect2 Rect;
};

class CFontCache
{
public:
    CFontCache(const float aLineHeight):
        LineHeight( aLineHeight )
    {
    }

    void AddTexture(std::unique_ptr<ITexture2D>&&);
    ITexture2D* GetTexture(SGlyph*) const;

    void AddGlyph(const char, std::unique_ptr<SGlyph>&&);
    bool HasGlyph(const char) const;
    SGlyph* GetGlyph(const char) const;

    float GetLineHeight() const { return LineHeight; }
private:
    float LineHeight = 0;
    std::vector<std::unique_ptr<ITexture2D>> Textures;
    std::map<char, std::unique_ptr<SGlyph>> Glyphs;
};

class IFont: public IResource
{
public:
    IFont(const std::string& Name, CResources* Resources):
        IResource( Name, Resources )
    {
    }

    virtual ~IFont()
    {
        FontCache.clear();
    }

    RESOURCE(IFont)

    // Get Longest x-axis Span
    float GetTextLength(const std::string&, const int);
    float GetTextHeight(const std::string&, const int);

    float GetLineHeight(const int);

    CFontCache* GetFontCache(const int);

    // Create or Get Font Cache for given Size
    virtual CFontCache* CreateFontCache(const int) = 0;
protected:
    std::map<int, std::unique_ptr<CFontCache>> FontCache;
};

//

class CFontFactory: public TResourceFactory<IFont>
{
public:
    CFontFactory(IGraphics*);

    std::unique_ptr<IResource> CreateResource(const std::string&, CResources*) override;
private:
    IGraphics* Graphics = nullptr;
};
