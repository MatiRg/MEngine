#pragma once
#include "../Math/Rect2.hpp"
#include "../Math/Vector2.hpp"
#include "../Math/Color.hpp"
#include "../Scene/Transform2D.hpp"
#include "Graphics.hpp"
#include "Texture2D.hpp"
#include "Font.hpp"
#include "Vertex2D.hpp"
#include "VertexBuffer.hpp"
#include <memory>
#include <vector>

class CDrawer2D;

class IRenderable2D
{
public:
    IRenderable2D(const CTransform2D& aTransform):
        Transform(aTransform)
    {
        SetBlendMode( EBlendMode::Alpha );
    }
    virtual ~IRenderable2D() = default;

    void SetBlendMode(const EBlendMode Mode) { BlendMode = Mode; }
    EBlendMode GetBlendMode() const { return BlendMode; }

    virtual void Render(CDrawer2D*) = 0;
protected:
    EBlendMode BlendMode = EBlendMode::None;
    //
    CTransform2D Transform;
};

class CPolygonRenderable2D: public IRenderable2D
{
public:
    CPolygonRenderable2D(const std::vector<SVertexPC>& aVertices, const EPrimitiveMode aType, const CTransform2D& aTransform):
        IRenderable2D(aTransform),
        Vertices(aVertices),
        Type(aType)
    {
    }

    void Render(CDrawer2D*) override;
protected:
    std::vector<SVertexPC> Vertices; 
    EPrimitiveMode Type;
};

class CTextRenderable2D: public IRenderable2D
{
public:
    CTextRenderable2D(IFont* aFont, const std::string& aText, const Vector2& aPosition,
const Color& aColor, const int aSize, const CTransform2D& aTransform):
        IRenderable2D(aTransform),
        Font(aFont),
        Text(aText),
        Position(aPosition),
        DrawColor(aColor),
        Size(aSize)
    {
    }

    void Render(CDrawer2D*) override;
protected:
    IFont* Font = nullptr;
    std::string Text;
    Vector2 Position;
    Color DrawColor;
    int Size;
};

class CTextureRenderable2D: public IRenderable2D
{
public:
    CTextureRenderable2D(ITexture2D* aTexture, const Rect2& aSource, const Rect2& aDestination, const ETextureFlip aFlip, 
const CTransform2D& aTransform):
        IRenderable2D(aTransform),
        Texture(aTexture),
        Source(aSource),
        Destination(aDestination),
        Flip(aFlip)
    {
    }

    void Render(CDrawer2D*) override;
protected:
    ITexture2D* Texture = nullptr;
    Rect2 Source;
    Rect2 Destination;
    ETextureFlip Flip;
};

class CPointRenderable2D: public IRenderable2D
{
public:
    CPointRenderable2D(const Vector2& aPosition, const Color& aDrawColor, const CTransform2D& aTransform):
        IRenderable2D(aTransform),
        Position(aPosition),
        DrawColor( aDrawColor )
    {
    }

    void Render(CDrawer2D*) override;
protected:
    Vector2 Position;
    Color DrawColor;
};


class CLineRenderable2D: public IRenderable2D
{
public:
    CLineRenderable2D(const Vector2& aStart, const Vector2& aEnd, const Color& aDrawColor, const CTransform2D& aTransform):
        IRenderable2D(aTransform),
        Start(aStart),
        End(aEnd),
        DrawColor( aDrawColor )
    {
    }

    void Render(CDrawer2D*) override;
protected:
    Vector2 Start;
    Vector2 End;
    Color DrawColor;
};

class CRectRenderable2D: public IRenderable2D
{
public:
    CRectRenderable2D(const Rect2& aRectangle, const bool aFill, const Color& aDrawColor, const CTransform2D& aTransform):
        IRenderable2D(aTransform),
        Rectangle(aRectangle),
        Fill(aFill),
        DrawColor( aDrawColor )
    {
    }

    void Render(CDrawer2D*) override;
protected:
    Rect2 Rectangle;
    bool Fill;
    Color DrawColor;
};
