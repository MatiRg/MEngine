#include "RectRenderer2D.hpp"
#include "../Entity2D.hpp"
#include "../../Graphics/Renderer2D.hpp"
#include "../../Engine/Engine.hpp"
#include "../../Core/XML.hpp"

void CRectRenderer2D::OnRender()
{
    CRenderer2D* Renderer = Engine->GetRenderer2D();

    auto Transform = GetOwner()->GetTransform().GetWorldTransform();
    Renderer->DrawRect( Rect2( Transform.GetPosition()-Size/2.0f, Size ),
        Filled, DrawColor, Transform );
}

bool CRectRenderer2D::OnLoad(CXMLElement* Root)
{
	Size = XML::LoadVector2( Root, "Size", Vector2(32.0f) );
	DrawColor = XML::LoadColor( Root, "DrawColor", Color::WHITE() );
	Filled = XML::LoadBool( Root, "Filled", true );
	Box = XML::LoadRect2( Root, "Box", Rect2(0.0f, 0.0f, 32.0f, 32.0f) );
    return true;
}

bool CRectRenderer2D::OnSave(CXMLElement* Root)
{
    XML::SaveVector2( Root, "Size", Size);
    XML::SaveColor( Root, "DrawColor", DrawColor);
    XML::SaveBool( Root, "Filled", Filled);
    XML::SaveRect2( Root, "Box", Box);
    return true;
}

const Rect2& CRectRenderer2D::GetBox() const
{
    auto Transform = GetOwner()->GetTransform();

    Vector2 WorldSize = Size*Transform.GetWorldScale();
    Box = Rect2( Transform.GetWorldPosition()-WorldSize/2.0f, WorldSize );

    return Box;
}