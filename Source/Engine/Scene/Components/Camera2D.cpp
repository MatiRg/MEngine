#include "Camera2D.hpp"
#include "../../Core/XML.hpp"

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
