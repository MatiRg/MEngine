#pragma once
#include "../Component.hpp"
#include "../../Graphics/Light.hpp"

class CLightComponent: public IComponent
{
public:
    CLightComponent(CEngine*);

    COMPONENT(CLightComponent)

    bool OnLoad(CXMLElement*) override;
    bool OnSave(CXMLElement*) override;

    void OnRender() override;

    // Self
    void SetLightType(ELightType x) { LightData.SetLightType(x); }
    ELightType GetLightType() const { return LightData.GetLightType(); }

    void SetColor(const Color& x) { LightData.SetColor(x); }
    const Color& GetColor() const { return LightData.GetColor(); }
    // In Kelvin`s
    void SetTemperature(const float Temperature) { LightData.SetTemperature(Temperature); }

    // Point; Interpreted as Radius for Point Light
    void SetRange(const float x) { LightData.SetRange(x); }
    float GetRange() const { return LightData.GetRange(); }
private:
    CLight LightData;
};
