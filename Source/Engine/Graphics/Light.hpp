#pragma once
#include "../Core/NonCopyable.hpp"
#include "../Math/Color.hpp"
#include "../Math/Vector3.hpp"

enum class ELightType
{
    Point,
    Direction,
    None
};

class CLight: public NonCopyableMovable
{
public:
    CLight();
    ~CLight();

    void SetType(ELightType x) { Type = x; }
    ELightType GetType() const { return Type; }

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
    ELightType Type = ELightType::None;
    Color Diffuse = Color::WHITE;
    float Range = 0.0f;
    Vector3 Position = Vector3::ZERO;
    Vector3 Direction = Vector3::FORWARD;
};