#include "Renderable2D.hpp"
#include "Drawer2D.hpp"
#include "../Math/Quaternion.hpp"

void CPolygonRenderable2D::Render(CDrawer2D* Drawer)
{
    Drawer->SetBlendMode(BlendMode);
    Drawer->DrawPolygon(Vertices, Type, Matrix, Layer);
}

void CTextRenderable2D::Render(CDrawer2D* Drawer)
{
    if( Font && Font->IsValid() )
    {
        Drawer->SetBlendMode(BlendMode);
        Drawer->DrawText(Font, Text, Position, Size, DrawColor, Matrix, Layer);
    }
}

void CTextureRenderable2D::Render(CDrawer2D* Drawer)
{
    if( Texture && Texture->IsValid() )
    {
        Drawer->SetBlendMode(BlendMode);
        Drawer->DrawTexture(Texture, Source, Destination, Flip, Matrix, Layer);
    }
}

void CPointRenderable2D::Render(CDrawer2D* Drawer)
{
    Drawer->SetBlendMode(BlendMode);
    Drawer->DrawPoint(Position, DrawColor, Matrix, Layer);
}

void CLineRenderable2D::Render(CDrawer2D* Drawer)
{
    Drawer->SetBlendMode(BlendMode);
    Drawer->DrawLine(Start, End, DrawColor, Matrix, Layer);
}

void CRectRenderable2D::Render(CDrawer2D* Drawer)
{
    Drawer->SetBlendMode(BlendMode);
    Drawer->DrawRect(Rectangle, Fill, DrawColor, Matrix, Layer);
}