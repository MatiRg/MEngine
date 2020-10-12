#include "SDLFont.hpp"
#include "SDLSurface.hpp"
#include "../Graphics/Graphics.hpp"
#include "../Graphics/Texture2D.hpp"
#include "../Core/Log.hpp"
#include "../Core/Utils.hpp"
#include "../Core/BinaryFile.hpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

CSDLFont::CSDLFont(const std::string& Name, CResources* Resources, IGraphics* aGraphics):
    IFont( Name, Resources ),
    Graphics( aGraphics )
{
}

CSDLFont::~CSDLFont()
{
}

bool CSDLFont::Load()
{
    CBinaryFile File( GetPath(), EFileMode::Read );
    if( !File.IsOpen() )
    {
        LOG( ESeverity::Error ) << "Unable to open: " << GetPath() << "\n";
        return false;
    }
    File.Seek( 0, EFileSeek::End );
    int Len = File.Tell();
    File.Seek( 0, EFileSeek::Begin );
    Data.resize( Len );
    if( static_cast<int>(File.Read( Data.data(), sizeof(char), Len )) != Len )
    {
        LOG( ESeverity::Error ) << "Unable to read file: " << GetPath() << "\n";
        return false;
    }

	Valid = true;
    return Valid;
}

namespace
{
    class CSDLTTFWrapper: public NonCopyableMovable
    {
    public:
        CSDLTTFWrapper(const std::vector<char>& Data, const int Size):
            RWops( SDL_RWFromConstMem( Data.data(), static_cast<int>(Data.size()) ) )
        {
            if( RWops )
            {
                Font = TTF_OpenFontRW( RWops, 0, Size );
            }
        }

        ~CSDLTTFWrapper()
        {
            if( RWops )
            {
                SDL_RWclose( RWops );
                RWops = nullptr;
            }
            if( Font )
            {
                TTF_CloseFont( Font );
                Font = nullptr;
            }
        }

        TTF_Font* GetFont() const { return Font; }
    private:
        SDL_RWops* RWops = nullptr;
        TTF_Font* Font = nullptr;
    };
}

CFontCache* CSDLFont::CreateFontCache(const int Size)
{
    if( Size < 6 || Size > 99 )
    {
        LOG( ESeverity::Error ) << "Invalid font size, make sure is between [6-99]\n";
        return nullptr;
    }

    if( FontCache.count( Size ) )
    {
        return FontCache[Size].get();
    }

    CSDLTTFWrapper Wrapper( Data, Size );
    TTF_Font* Font = Wrapper.GetFont();
    if( !Font )
    {
        LOG( ESeverity::Error ) << "Unable to load font: " << TTF_GetError() << "\n";
        return nullptr;
    }

    const int AtlasSize = 1024; // pow 2
    std::unique_ptr<CSDLSurface> LastGlyphAtlas = std::make_unique<CSDLSurface>();
    if( !LastGlyphAtlas->Create( AtlasSize, AtlasSize ) )
    {
        LOG( ESeverity::Error ) << "Unable to create glyph atlas\n";
        return nullptr;
    }
    int x = 0, y = 0, TextureID = 0;
    std::unique_ptr<CFontCache> Cache = std::make_unique<CFontCache>( TTF_FontLineSkip( Font ) );
    for(char i = 32; i < 127; ++i)
    {
        if( TTF_GlyphIsProvided( Font, i ) )
        {
            std::unique_ptr<SGlyph> Glyph = std::make_unique<SGlyph>();

            TTF_GlyphMetrics( Font, i, nullptr, nullptr, nullptr, nullptr, &Glyph->Advance );

            std::unique_ptr<CSDLSurface> SurfaceTmp = std::make_unique<CSDLSurface>();

            SDL_Color _Color{0xFF, 0xFF, 0xFF, 0xFF};
            if( !SurfaceTmp->Create( TTF_RenderGlyph_Blended( Font, i, _Color ) ) )
            {
                continue;
            }

            if( x >= AtlasSize - SurfaceTmp->GetWidth() )
            {
                x = 0;
                y += Cache->GetLineHeight();
                if( y >= AtlasSize - Cache->GetLineHeight() )
                {
                    y = 0;
                    std::unique_ptr<ITexture2D> Texture = Graphics->CreateTexture2D(Resources);
                    if( !Texture->CreateFromSurface( LastGlyphAtlas.get() ) )
                    {
                        LOG( ESeverity::Error ) << "Unable to create glyph atlas\n";
                        return nullptr;
                    }
                    ++TextureID;
                    Cache->AddTexture( std::move(Texture) );
                    LastGlyphAtlas = std::make_unique<CSDLSurface>();
                    if( !LastGlyphAtlas->Create( AtlasSize, AtlasSize ) )
                    {
                        LOG( ESeverity::Error ) << "Unable to create glyph atlas\n";
                        return nullptr;
                    }
                }
            }
            Rect2 Rect( static_cast<float>(x), static_cast<float>(y), 
				static_cast<float>(SurfaceTmp->GetWidth()), static_cast<float>(SurfaceTmp->GetHeight()) );
            LastGlyphAtlas->Blit( SurfaceTmp.get(), Rect );
            Glyph->Texture = TextureID;
            Glyph->Rect = Rect;
            Cache->AddGlyph( i, std::move( Glyph ) );
            x += SurfaceTmp->GetWidth();
        }
    }
    std::unique_ptr<ITexture2D> Texture = Graphics->CreateTexture2D(Resources);
    if( !Texture->CreateFromSurface( LastGlyphAtlas.get() ) )
    {
        LOG( ESeverity::Error ) << "Unable to create glyph atlas\n";
        return nullptr;
    }
    Cache->AddTexture( std::move(Texture) );

    CFontCache* Tmp = Cache.get();
    FontCache[Size] = std::move( Cache );
    return Tmp;
}
