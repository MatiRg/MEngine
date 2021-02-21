#pragma once
#include "../Component.hpp"
#include "../../Math/Rect2.hpp"
#include "../../Math/Color.hpp"
#include "../../Graphics/Texture2D.hpp"

class CStaticSprite2D: public IComponent
{
public:
    CStaticSprite2D(CEngine* aEngine): IComponent(aEngine) {}

    COMPONENT(CStaticSprite2D)

    void OnRender() override;

    bool OnLoad(CXMLElement*) override;
    bool OnSave(CXMLElement*) override;

    void SetTexture(ITexture2D*);
    ITexture2D* GetTexture() const { return Texture; }
    bool HasTexture() const { return Texture; }

    void SetFlip(const ETextureFlip aFlip) { Flip = aFlip; }
    ETextureFlip GetFlip() const { return Flip; }

    void SetSize(const Vector2& aSize) { Size = aSize; }
    const Vector2& GetSize() const { return Size; }

    void SetColorMod(const Color& aColor) { ColorMod = aColor; }
    const Color& GetColorMod() const { return ColorMod; }

    const Rect2& GetBox() const;
private:
    ITexture2D* Texture = nullptr;
    ETextureFlip Flip = ETextureFlip::None;
    Vector2 Size = VECTOR2_ONE;
    Color ColorMod = COLOR_WHITE;
    mutable Rect2 Box;
};
