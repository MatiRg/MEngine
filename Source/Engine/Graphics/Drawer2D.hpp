#pragma once
#include "../Engine/EngineModule.hpp"
#include "../Math/Matrix4.hpp"
#include "VertexBuffer.hpp"
#include "Texture2D.hpp"
#include "Graphics.hpp"
#include "Vertex2D.hpp"
#include <memory>
#include <vector>

class CResources;
class IWindow;
class IVertexBuffer;
class IShader;
class IFont;

class CDrawer2D: public IEngineModule
{
public:
    CDrawer2D(IGraphics*, CResources*, IWindow*);
    ~CDrawer2D();

    ENGINE_MODULE(CDrawer2D)

    void SetViewProjection(const Matrix4& Matrix) { ViewProjection = Matrix; }
    const Matrix4& GetViewProjection() const { return ViewProjection; }

    void SetBlendMode(const EBlendMode Mode) { BlendMode = Mode; }
    EBlendMode GetBlendMode() const { return BlendMode; }

    bool Init(const SEngineParams&) override;
    void Exit() override;

    // Vertexes, Type, Layer
    void DrawPolygon(const std::vector<SVertex2D>&, const EPrimitiveType, const float);
    // Vertexes, Type, Transform, Layer
    void DrawPolygon(const std::vector<SVertex2D>&, const EPrimitiveType, const Matrix4&, const float);

    // Position, Color, Layer
    void DrawPoint(const Vector2&, const Color&, const float);
    // Position, Color, Transform, Layer
    void DrawPoint(const Vector2&, const Color&, const Matrix4&, const float);

    // Start, Stop, Color, Layer
    void DrawLine(const Vector2&, const Vector2&, const Color&, const float);
    // Start, Stop, Color, Transform, Layer
    void DrawLine(const Vector2&, const Vector2&, const Color&, const Matrix4&, const float);

    // Rect, Fill, Color, Layer
    void DrawRect(const Rect2&, const bool, const Color&, const float);
    // Rect, Fill, Color, Transform, Layer
    void DrawRect(const Rect2&, const bool, const Color&, const Matrix4&, const float);

    // Font, Text, Position, Size, Color, Layer
    void DrawText(IFont*, const std::string&, const Vector2&, const int, const Color&, const float);
    // Font, Text, Position, Size, Color, Transform, Layer
    void DrawText(IFont*, const std::string&, const Vector2&, const int, const Color&, const Matrix4&, const float);

    // Texture, Source, Destination, Flip, Layer
    void DrawTexture(ITexture2D*, const Rect2&, const Rect2&, const ETextureFlip, const float);
    // Texture, Source, Destination, Flip, Transform, Layer
    void DrawTexture(ITexture2D*, const Rect2&, const Rect2&, const ETextureFlip, const Matrix4&, const float);
private:
    IGraphics* Graphics = nullptr;
    CResources* Resources = nullptr;
    IWindow* Window = nullptr;
    std::unique_ptr<IVertexBuffer> VertexBuffer;
    IShader* BasicShader = nullptr;
    IShader* BasicPrimitive = nullptr;
    IShader* TextureShader = nullptr;
    Matrix4 ViewProjection;
    EBlendMode BlendMode = EBlendMode::Alpha;
};
