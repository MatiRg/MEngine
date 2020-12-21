#pragma once
#include "../Engine/EngineModule.hpp"
#include "../Math/Matrix4.hpp"
#include "../Math/Rect2.hpp"
#include "../Math/Color.hpp"
#include "Texture2D.hpp"
#include "Vertex2D.hpp"
#include "VertexBuffer.hpp"
#include <map>
#include <memory>
#include <vector>

class IRenderable2D;
class IFont;
class ITexture2D;
class CDrawer2D;

class CRenderer2D: public IEngineModule
{
public:
    CRenderer2D(CDrawer2D*);
    ~CRenderer2D();

    ENGINE_MODULE(CRenderer2D)

    bool Init(const SEngineParams&) override;
    void Exit() override;

    void Render();

    void SetViewProjection(const Matrix4&);

    // Vertexes, Type, Matrix, Layer
    IRenderable2D* DrawPolygon(const std::vector<SVertex2D>&, const EPrimitiveMode, const Matrix4&, const float);

    // Position, Color, Matrix, Layer
    IRenderable2D* DrawPoint(const Vector2&, const Color&, const Matrix4&, const float);

    // Start, Stop, Color, Matrix, Layer
    IRenderable2D* DrawLine(const Vector2&, const Vector2&, const Color&, const Matrix4&, const float);

    // Rectangle, Filled, Color, Matrix, Layer
    IRenderable2D* DrawRect(const Rect2&, const bool, const Color&, const Matrix4&, const float);

    // Font, Text, Position, Color, Size, Matrix, Layer
    IRenderable2D* DrawText(IFont*, const std::string&, const Vector2&, const Color&, const int, const Matrix4&, const float);

    // Texture, SrcRect, DestRect, Flip, Matrix, Layer
    IRenderable2D* DrawTexture(ITexture2D*, const Rect2&, const Rect2&, const ETextureFlip, const Matrix4&, const float);
private:
    CDrawer2D* Drawer2D = nullptr;
    Matrix4 ViewProjection;
    std::vector<std::unique_ptr<IRenderable2D>> Renderables;
};
