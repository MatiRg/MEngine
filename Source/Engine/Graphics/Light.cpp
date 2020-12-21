#include "Light.hpp"
#include "../System/MemoryManager.hpp"

CLight::CLight()
{
    ADD_MEMORY_RECORD(this);
}
	
CLight::~CLight()
{
    ERASE_MEMORY_RECORD(this);
}

// https://tannerhelland.com/2012/09/18/convert-temperature-rgb-algorithm-code.html
void CLight::SetTemperature(const float x)
{
    float temp = x / 100.0f;
    float red = 0.0f, blue = 0.0f, green = 0.0f;

    if (temp <= 66.0f) 
    {
        red = 255.0f;
    }
    else 
    {
        red = temp - 60.0f;
        red = 329.698727466f * Math::Pow(red, -0.1332047592f);
        red = Math::Clamp(red, 0.0f, 255.0f);
    }

    if (temp <= 66.0f) 
    {
        green = temp;
        green = 99.4708025861f * Math::Log(green) - 161.1195681661f;
        green = Math::Clamp(green, 0.0f, 255.0f);
    }
    else 
    {
        green = temp - 60.0f;
        green = 288.1221695283f * Math::Pow(green, -0.0755148492f);
        green = Math::Clamp(green, 0.0f, 255.0f);
    }

    if (temp >= 66.0f) 
    {
        blue = 255.0f;
    }
    else 
    {
        if (temp <= 19.0f ) 
        {
            blue = 0.0f;
        }
        else 
        {
            blue = temp - 10.0f;
            blue = 138.5177312231f * Math::Log(blue) - 305.0447927307f;
            blue = Math::Clamp(blue, 0.0f, 255.0f);
        }
    }

    SetColor( {red / 255.0f, green / 255.0f, blue / 255.0f} );
}