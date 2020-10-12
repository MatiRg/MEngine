#pragma once
#include "../Engine/EngineModule.hpp"
#include "../Math/Matrix4.hpp"
#include "../Math/Rect2.hpp"
#include "../Math/Color.hpp"
#include "../Scene/Transform2D.hpp"
#include "Texture2D.hpp"
#include "Vertex2D.hpp"
#include "VertexBuffer.hpp"
#include <map>
#include <memory>
#include <vector>

class IRenderable2D;
class IFont;
class ITexture2D;
class CEntity2D;
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

    void SetCamera(CEntity2D*);
    bool HasCamera() const;

    // Vertexes, Type, Transform
    IRenderable2D* DrawPolygon(const std::vector<SVertexPC>&, const EPrimitiveMode, const CTransform2D&);

    // Position, Color, Transform
    IRenderable2D* DrawPoint(const Vector2&, const Color&, const CTransform2D&);

    // Start, Stop, Color, Transform
    IRenderable2D* DrawLine(const Vector2&, const Vector2&, const Color&, const CTransform2D&);

    // Rectangle, Filled, Color, Transform
    IRenderable2D* DrawRect(const Rect2&, const bool, const Color&, const CTransform2D&);

    // Font, Text, Position, Color, Size, Transform
    IRenderable2D* DrawText(IFont*, const std::string&, const Vector2&, const Color&, const int, const CTransform2D&);

    // Texture, SrcRect, DestRect, Flip, Transform
    IRenderable2D* DrawTexture(ITexture2D*, const Rect2&, const Rect2&, const ETextureFlip, const CTransform2D&);
private:
    CDrawer2D* Drawer2D = nullptr;
    Matrix4 ViewProjection;
    std::vector<std::unique_ptr<IRenderable2D>> Renderables;
    CEntity2D* Camera = nullptr;
};
