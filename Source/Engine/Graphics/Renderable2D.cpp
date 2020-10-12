#include "Renderable2D.hpp"
#include "Drawer2D.hpp"

void CPolygonRenderable2D::Render(CDrawer2D* Drawer)
{
    Drawer->SetBlendMode(BlendMode);
    Drawer->DrawPolygon(Vertices, Type, Transform.GetMatrix(), Transform.GetLayer() );
}

void CTextRenderable2D::Render(CDrawer2D* Drawer)
{
    if( Font && Font->IsValid() )
    {
        Drawer->SetBlendMode(BlendMode);
        Drawer->DrawText(Font, Text, Position, Size, DrawColor, Transform.GetMatrix(), Transform.GetLayer() );
    }
}

void CTextureRenderable2D::Render(CDrawer2D* Drawer)
{
    if( Texture && Texture->IsValid() )
    {
        Drawer->SetBlendMode(BlendMode);
        Drawer->DrawTexture(Texture, Source, Destination, Flip, Transform.GetMatrix(), Transform.GetLayer() );
    }
}

void CPointRenderable2D::Render(CDrawer2D* Drawer)
{
    Drawer->SetBlendMode(BlendMode);
    Drawer->DrawPoint(Position, DrawColor, Transform.GetMatrix(), Transform.GetLayer() );
}

void CLineRenderable2D::Render(CDrawer2D* Drawer)
{
    Drawer->SetBlendMode(BlendMode);
    Drawer->DrawLine(Start, End, DrawColor, Transform.GetMatrix(), Transform.GetLayer() );
}

void CRectRenderable2D::Render(CDrawer2D* Drawer)
{
    Drawer->SetBlendMode(BlendMode);
    Drawer->DrawRect(Rectangle, Fill, DrawColor, Transform.GetMatrix(), Transform.GetLayer() );
}