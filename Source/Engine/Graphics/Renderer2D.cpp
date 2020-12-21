#include "Renderer2D.hpp"
#include "Drawer2D.hpp"
#include "Renderable2D.hpp"
#include "Font.hpp"
#include "Texture2D.hpp"
#include "../Core/Log.hpp"

CRenderer2D::CRenderer2D(CDrawer2D* aDrawer):
    IEngineModule( "Renderer2D" ),
    Drawer2D(aDrawer)
{
}

CRenderer2D::~CRenderer2D()
{
}

bool CRenderer2D::Init(const SEngineParams&)
{
    ViewProjection = Drawer2D->GetViewProjection();

    LOG( ESeverity::Info ) << "Renderer2D - Init\n";
    return true;
}

void CRenderer2D::Exit()
{
    LOG( ESeverity::Info ) << "Renderer2D - Exit\n";
}

void CRenderer2D::Render()
{
    Matrix4 Tmp = Drawer2D->GetViewProjection();
    Drawer2D->SetViewProjection(ViewProjection);
    EBlendMode Mode = Drawer2D->GetBlendMode();
    
    for(const auto& Renderable: Renderables)
    {
        Renderable->Render(Drawer2D);
    }
    Renderables.clear();
    
    Drawer2D->SetViewProjection(Tmp);
    Drawer2D->SetBlendMode(Mode);
}

void CRenderer2D::SetViewProjection(const Matrix4& aViewProjecion)
{
    ViewProjection = aViewProjecion;
}

IRenderable2D* CRenderer2D::DrawPolygon(const std::vector<SVertex2D>& Vertices, const EPrimitiveMode Type,
    const Matrix4& aMatrix, const float aLayer)
{
    Renderables.push_back( std::make_unique<CPolygonRenderable2D>(Vertices, Type, aMatrix, aLayer) );
    return Renderables.back().get();
}

IRenderable2D* CRenderer2D::DrawPoint(const Vector2& Position, const Color& DrawColor, const Matrix4& aMatrix, const float aLayer)
{
    Renderables.push_back( std::make_unique<CPointRenderable2D>(Position, DrawColor, aMatrix, aLayer) );
    return Renderables.back().get();
}

IRenderable2D* CRenderer2D::DrawLine(const Vector2& Start, const Vector2& End, const Color& DrawColor, 
    const Matrix4& aMatrix, const float aLayer)
{
    Renderables.push_back( std::make_unique<CLineRenderable2D>(Start, End, DrawColor, aMatrix, aLayer) );
    return Renderables.back().get();
}

IRenderable2D* CRenderer2D::DrawRect(const Rect2& Rect, const bool Fill, const Color& DrawColor, 
    const Matrix4& aMatrix, const float aLayer)
{
    Renderables.push_back( std::make_unique<CRectRenderable2D>(Rect, Fill, DrawColor, aMatrix, aLayer) );
    return Renderables.back().get();
}

IRenderable2D* CRenderer2D::DrawText(IFont* Font, const std::string& Text, const Vector2& Start, const Color& DrawColor, const int Size, 
    const Matrix4& aMatrix, const float aLayer)
{
    Renderables.push_back( std::make_unique<CTextRenderable2D>( Font, Text, Start, DrawColor, Size, aMatrix, aLayer) );
    return Renderables.back().get();
}

IRenderable2D* CRenderer2D::DrawTexture(ITexture2D* Texture, const Rect2& Src, const Rect2& Dst, const ETextureFlip Flip, 
    const Matrix4& aMatrix, const float aLayer)
{
    Renderables.push_back( std::make_unique<CTextureRenderable2D>( Texture, Src, Dst, Flip, aMatrix, aLayer) );
    return Renderables.back().get();
}
