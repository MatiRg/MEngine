#pragma once
#include "../Component.hpp"
#include "../../Math/Rect2.hpp"

class CCamera2D: public IComponent
{
public:
    CCamera2D(CEngine* aEngine): IComponent(aEngine) {}

    COMPONENT(CCamera2D)

    bool OnLoad(CXMLElement*) override;
    bool OnSave(CXMLElement*) override;

    // Set View Size
    void SetSize(const Vector2& aSize) { CameraBox.SetSize( aSize ); }
    Vector2 GetSize() const { return CameraBox.GetSize(); }

    void Translate(const Vector2& aVector) { CameraBox.Translate(aVector); }
    Vector2 GetTranslation() const { return CameraBox.GetPosition(); }
private:
    Rect2 CameraBox{0.0f, 0.0f, 800.0f, 600.0f};
};