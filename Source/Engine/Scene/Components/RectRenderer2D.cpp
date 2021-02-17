#include "RectRenderer2D.hpp"
#include "../Entity.hpp"
#include "../../Graphics/Renderer2D.hpp"
#include "../../Engine/Engine.hpp"
#include "../../Core/XML.hpp"

void CRectRenderer2D::OnRender()
{
    CRenderer2D* Renderer = Engine->GetRenderer2D();

    const auto& Transform = GetOwner()->GetTransform();
    Vector2 Pos = Transform.GetWorldPosition2D();
    Vector2 Pivot = Transform.HasParent() && GetOwner()->GetParent()->GetID() != WORLD_ENTITY
        ? Transform.GetParent()->GetWorldPosition2D() : Pos;
    Pos -= Size / 2.0f;
    Matrix4 Tmp = Math::Transform2D(Pivot, Transform.GetWorldRotation2D(), Transform.GetWorldScale2D());
    //
    Renderer->DrawRect( Rect2(Pos, Size ),
        Filled, DrawColor, Tmp, Transform.GetLayer());
}

bool CRectRenderer2D::OnLoad(CXMLElement* Root)
{
	Size = XML::LoadVector2( Root, "Size", Vector2(32.0f) );
	DrawColor = XML::LoadColor( Root, "DrawColor", Color::WHITE );
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
    const auto& Transform = GetOwner()->GetTransform();

    Vector2 WorldSize = Size*Transform.GetWorldScale2D();
    Box = Rect2( Transform.GetWorldPosition2D()-WorldSize/2.0f, WorldSize );

    return Box;
}