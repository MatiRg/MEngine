#pragma once
#include "Renderable3D.hpp"
#include "../Math/Color.hpp"
#include "../Math/Vector3.hpp"

enum class ELightType
{
    Direction = 0,
    //Point = 1,
    //Spot = 2,
    None
};

class CLight: public CRenderable3D
{
public:
    CLight();
    ~CLight();

    void SetLightType(ELightType x) { LightType = x; }
    ELightType GetLightType() const { return LightType; }

    void SetColor(const Color& x) { Diffuse = x; }
    const Color& GetColor() const { return Diffuse; }
    // In Kelvin`s
    void SetTemperature(const float);

    // Point; Interpreted as Radius for Point Light
    void SetRange(const float x) { Range = Math::Max(x, 0.0f); }
    float GetRange() const { return Range; }

    // Point
    void SetPosition(const Vector3& x) { Position = x; }
    const Vector3& GetPosition() const { return Position; }

    // Directional
    void SetDirection(const Vector3& x) { Direction = x; }
    const Vector3& GetDirection() const { return Direction; }
private:
    ELightType LightType = ELightType::None;
    Color Diffuse = COLOR_WHITE;
    float Range = 1.0f;
    Vector3 Position = VECTOR3_ZERO;
    Vector3 Direction = VECTOR3_FORWARD;
};