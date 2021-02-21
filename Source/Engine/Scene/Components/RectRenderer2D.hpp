#pragma once
#include "../Component.hpp"
#include "../../Math/Rect2.hpp"
#include "../../Math/Color.hpp"

class CRectRenderer2D: public IComponent
{
public:
    CRectRenderer2D(CEngine* aEngine): IComponent(aEngine) {}

    COMPONENT(CRectRenderer2D)

    void OnRender() override;

    bool OnLoad(CXMLElement*) override;
    bool OnSave(CXMLElement*) override;

    void SetSize(const Vector2& aSize) { Size = aSize; }
    const Vector2& GetSize() const { return Size; }

    void SetColor(const Color& aColor) { DrawColor = aColor; }
    const Color& GetColor() const { return DrawColor; }

    void SetFill(const bool Value) { Filled = Value; }
    bool IsFilled() const { return Filled; }

    const Rect2& GetBox() const;
private:
    Vector2 Size = Vector2(32.0f);
    Color DrawColor = COLOR_WHITE;
    bool Filled = true;
    mutable Rect2 Box;
};
