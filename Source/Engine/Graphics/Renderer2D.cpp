#include "Renderer2D.hpp"
#include "Drawer2D.hpp"
#include "Renderable2D.hpp"
#include "Font.hpp"
#include "Texture2D.hpp"
#include "../Core/Log.hpp"
#include "../Scene/Entity2D.hpp"
#include "../Scene/Components/Camera2D.hpp"

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
    if( HasCamera() )
    {
        CCamera2D* Component = Camera->GetComponent<CCamera2D>();
        Matrix4 Mat(
            1.0f, 0.0f, 0.0f, Component->GetTranslation().x,
            0.0f, 1.0f, 0.0f, Component->GetTranslation().y,
            0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
        );
        ViewProjection = Math::Ortho(Camera->GetComponent<CCamera2D>()->GetSize().x, 0.0f, Camera->GetComponent<CCamera2D>()->GetSize().y, 0.0f)*Mat;

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
}

void CRenderer2D::SetCamera(CEntity2D* aCamera)
{
    Camera = aCamera;
}

bool CRenderer2D::HasCamera() const
{
    return Camera && Camera->HasComponent<CCamera2D>();
}

IRenderable2D* CRenderer2D::DrawPolygon(const std::vector<SVertexPC>& Vertices, const EPrimitiveMode Type, const CTransform2D& Transform)
{
    Renderables.push_back( std::make_unique<CPolygonRenderable2D>(Vertices, Type, Transform) );
    return Renderables.back().get();
}

IRenderable2D* CRenderer2D::DrawPoint(const Vector2& Position, const Color& DrawColor, const CTransform2D& Transform)
{
    Renderables.push_back( std::make_unique<CPointRenderable2D>(Position, DrawColor, Transform) );
    return Renderables.back().get();
}

IRenderable2D* CRenderer2D::DrawLine(const Vector2& Start, const Vector2& End, const Color& DrawColor, const CTransform2D& Transform)
{
    Renderables.push_back( std::make_unique<CLineRenderable2D>(Start, End, DrawColor, Transform) );
    return Renderables.back().get();
}

IRenderable2D* CRenderer2D::DrawRect(const Rect2& Rect, const bool Fill, const Color& DrawColor, const CTransform2D& Transform)
{
    Renderables.push_back( std::make_unique<CRectRenderable2D>(Rect, Fill, DrawColor, Transform) );
    return Renderables.back().get();
}

IRenderable2D* CRenderer2D::DrawText(IFont* Font, const std::string& Text, const Vector2& Start, const Color& DrawColor, const int Size, const CTransform2D& Transform)
{
    Renderables.push_back( std::make_unique<CTextRenderable2D>( Font, Text, Start, DrawColor, Size, Transform ) );
    return Renderables.back().get();
}

IRenderable2D* CRenderer2D::DrawTexture(ITexture2D* Texture, const Rect2& Src, const Rect2& Dst, const ETextureFlip Flip, const CTransform2D& Transform)
{
    Renderables.push_back( std::make_unique<CTextureRenderable2D>( Texture, Src, Dst, Flip, Transform ) );
    return Renderables.back().get();
}
