#pragma once
#include "../Math/Rect2.hpp"
#include "../Math/Vector2.hpp"
#include "../Math/Matrix4.hpp"
#include "../Math/Color.hpp"
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
    IRenderable2D(const Matrix4& aMatrix, const float aLayer):
        Matrix(aMatrix),
        Layer(aLayer)
    {
        SetBlendMode( EBlendMode::Alpha );
    }
    virtual ~IRenderable2D() = default;

    void SetBlendMode(const EBlendMode Mode) { BlendMode = Mode; }
    EBlendMode GetBlendMode() const { return BlendMode; }

    float GetLayer() const { return Layer; }

    virtual void Render(CDrawer2D*) = 0;
protected:
    EBlendMode BlendMode = EBlendMode::None;
    //
    Matrix4 Matrix;
    float Layer;
};

class CPolygonRenderable2D: public IRenderable2D
{
public:
    CPolygonRenderable2D(const std::vector<SVertex2D>& aVertices, const EPrimitiveType aType, const Matrix4& aMatrix, const float aLayer):
        IRenderable2D(aMatrix, aLayer),
        Vertices(aVertices),
        Type(aType)
    {
    }

    void Render(CDrawer2D*) override;
protected:
    std::vector<SVertex2D> Vertices;
    EPrimitiveType Type;
};

class CTextRenderable2D: public IRenderable2D
{
public:
    CTextRenderable2D(IFont* aFont, const std::string& aText, const Vector2& aPosition,
const Color& aColor, const int aSize, const Matrix4& aMatrix, const float aLayer):
        IRenderable2D(aMatrix, aLayer),
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
const Matrix4& aMatrix, const float aLayer):
        IRenderable2D(aMatrix, aLayer),
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
    CPointRenderable2D(const Vector2& aPosition, const Color& aDrawColor, const Matrix4& aMatrix, const float aLayer):
        IRenderable2D(aMatrix, aLayer),
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
    CLineRenderable2D(const Vector2& aStart, const Vector2& aEnd, const Color& aDrawColor, 
const Matrix4& aMatrix, const float aLayer):
        IRenderable2D(aMatrix, aLayer),
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
    CRectRenderable2D(const Rect2& aRectangle, const bool aFill, const Color& aDrawColor, 
const Matrix4& aMatrix, const float aLayer):
        IRenderable2D(aMatrix, aLayer),
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
