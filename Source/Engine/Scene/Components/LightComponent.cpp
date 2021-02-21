#include "LightComponent.hpp"
#include "../Entity.hpp"
#include "../../Math/Transform.hpp"
#include "../../Core/XML.hpp"
#include "../../Engine/Engine.hpp"
#include "../../Graphics/Renderer3D.hpp"

CLightComponent::CLightComponent(CEngine* aEngine):
    IComponent(aEngine)
{
}

bool CLightComponent::OnLoad(CXMLElement* Root)
{
    SetLightType(XML::LoadEnum(Root, "LightType", ELightType::None));
    SetColor(XML::LoadColor(Root, "Color", COLOR_WHITE));
    SetRange(XML::LoadFloat(Root, "Range", 1.0f));
    return true;
}

bool CLightComponent::OnSave(CXMLElement* Root)
{
    XML::SaveEnum( Root, "LightType", GetLightType() );
    XML::SaveColor( Root, "Color", GetColor() );
    XML::SaveFloat( Root, "Range", GetRange() );
    return true;
}

void CLightComponent::OnRender()
{
    const CTransform& Transform = Owner->GetTransform();
    
    LightData.SetMatrix( Transform.GetWorldMatrix() );
    LightData.SetPosition( Transform.GetWorldPosition() );
    LightData.SetDirection( Transform.GetForward() );

    if (LightData.GetLightType() != ELightType::None)
    {
        GetEngine()->GetRenderer3D()->AddRenderable(&LightData);
    }
}
