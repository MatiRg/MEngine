#include "Camera2D.hpp"
#include "../../Core/XML.hpp"
#include "../../Engine/Engine.hpp"
#include "../../Graphics/Renderer2D.hpp"

bool CCamera2D::OnLoad(CXMLElement* Root)
{
	CameraBox = XML::LoadRect2( Root, "CameraBox", Rect2(0.0f, 0.0f, 800.0f, 600.0f) );
    return true;
}

bool CCamera2D::OnSave(CXMLElement* Root)
{
    XML::SaveRect2(Root, "CameraBox", CameraBox);
    return true;
}

void CCamera2D::OnLateUpdate(const float)
{
    Engine->GetRenderer2D()->SetViewProjection( GetViewProjection() );
}

Matrix4 CCamera2D::GetViewProjection() const
{
    return Math::Translation(GetTranslation().x, GetTranslation().y, 0.0f) * Math::Ortho(GetSize().x, 0.0f, GetSize().y, 0.0f, 1000.0f, -1000.0f);
}