#include "Font.hpp"
#include "Graphics.hpp"

void CFontCache::AddTexture(std::unique_ptr<ITexture2D>&& Texture)
{
    if( !Texture )
    {
        return;
    }
    Textures.push_back( std::move(Texture) );
}

ITexture2D* CFontCache::GetTexture(SGlyph* Glyph) const
{
    if( !Glyph )
    {
        return nullptr;
    }
    return Textures[Glyph->Texture].get();
}

void CFontCache::AddGlyph(const char Code, std::unique_ptr<SGlyph>&& Glyph)
{
    if( !Glyph )
    {
        return;
    }
    Glyphs[Code] = std::move(Glyph);
}

bool CFontCache::HasGlyph(const char Code) const
{
    return Glyphs.count(Code);
}

SGlyph* CFontCache::GetGlyph(const char Code) const
{
    if( Glyphs.count(Code) )
    {
        return Glyphs.at(Code).get();
    }
    return nullptr;
}

//

float IFont::GetTextLength(const std::string& Text, const int Size)
{
    if( !IsValid() )
    {
        return 0.0f;
    }

    CFontCache* Cache = CreateFontCache( Size );
    if( !Cache )
    {
        return 0.0f;
    }

    float Maximum = 0.0f;
    float Length = 0.0f;
    for(const auto &i: Text)
    {
        if( i == '\n' )
        {
            Maximum = Math::Max( Length, Maximum );
            Length = 0.0f;
        }
        else if( Cache->HasGlyph(i) )
        {
            Length += Cache->GetGlyph(i)->Advance;
        }
    }
    return Math::Max( Length, Maximum );
}

float IFont::GetTextHeight(const std::string& Text, const int Size)
{
    if( !IsValid() )
    {
        return 0.0f;
    }

    CFontCache* Cache = CreateFontCache( Size );
    if( !Cache )
    {
        return 0.0f;
    }

    float Height = Cache->GetLineHeight();
    for(const auto &i: Text)
    {
        if( i == '\n' )
        {
            Height += Cache->GetLineHeight();
        }
    }
    return Height;
}

float IFont::GetLineHeight(const int Size)
{
    if( !IsValid() )
    {
        return 0.0f;
    }

    CFontCache* Cache = CreateFontCache( Size );
    if( !Cache )
    {
        return 0.0f;
    }

    return Cache->GetLineHeight();
}

CFontCache* IFont::GetFontCache(const int Size)
{
    if( !IsValid() )
    {
        return nullptr;
    }

    return CreateFontCache( Size );
}

//

CFontFactory::CFontFactory(IGraphics* aGraphics):
    Graphics(aGraphics)
{
}

std::unique_ptr<IResource> CFontFactory::CreateResource(const std::string& Name, CResources* Resources)
{
    return Graphics->CreateFont( Name, Resources );
}