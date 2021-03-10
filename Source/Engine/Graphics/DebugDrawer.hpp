#pragma once
#include "../Engine/EngineModule.hpp"
#include "../Math/Vector3.hpp"
#include "../Math/Color.hpp"
#include "Renderable3D.hpp"
#include "Shader.hpp"
#include <vector>

class CRenderer3D;
class IGraphics;
class CResources;

class CDebugDrawer: public IEngineModule
{
public:
    CDebugDrawer(CRenderer3D*, IGraphics*, CResources*);
    ~CDebugDrawer();

    ENGINE_MODULE(CDebugDrawer)

    void AddLine(const Vector3& Start, const Vector3& End, const Color& DrawColor, const float Time = 0.0f);

    bool Init(const SEngineParams&) override;
    void OnBeginFrame() override;
    void OnUpdate(const float) override;
    void OnRender() override;
    void Exit() override;
private:
    struct SLine
    {
        Vector3 Start;
        Vector3 End;
        Color DrawColor;
        float Time;

        SLine(const Vector3& aStart, const Vector3& aEnd, const Color& aDrawColor, const float aTime):
            Start(aStart),
            End(aEnd),
            DrawColor(aDrawColor),
            Time(aTime)
        {
        }
    };
private:
    CRenderer3D* Renderer3D = nullptr;
    IGraphics* Graphics = nullptr;
    CResources* Resources = nullptr;
    std::vector<SLine> Lines;
    std::unique_ptr<IVertexBuffer> Buffer;
    IShader* Shader = nullptr;
    std::unique_ptr<CMaterial> Material;
    CRenderable3D LineRenderable;
};
