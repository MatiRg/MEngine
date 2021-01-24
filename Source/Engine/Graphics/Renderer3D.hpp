#pragma once
#include "Renderable3D.hpp"
#include "VertexBuffer.hpp"
#include "Light.hpp"
#include "PostEffect.hpp"
#include "../Engine/EngineModule.hpp"
#include <vector>

class IGraphics;
class CResources;

class CRenderer3D: public IEngineModule
{
public:
    CRenderer3D(IGraphics*, CResources*);
    ~CRenderer3D();

    ENGINE_MODULE(CRenderer3D)

    CPostEffect* CreatePostEffect(const std::string& ShaderName, const int Order);

    // Non Ownership
    void AddRenderable(CRenderable3D*);

    void SetViewMatrix(const Matrix4& aMtx) { ViewMatrix = aMtx; }
    const Matrix4& GetViewMatrix() const { return ViewMatrix; }

    void SetProjectionMatrix(const Matrix4& aMtx) { ProjectionMatrix = aMtx; }
    const Matrix4& GetProjectionMatrix() const { return ProjectionMatrix; }

    void SetCameraPosition(const Vector3& aVec) { CameraPosition = aVec; }
    const Vector3& GetCameraPosition() const { return CameraPosition; }

    void SetAmbientColor(const Color& v) { AmbientColor = v; }
    const Color& GetAmbientColor() const { return AmbientColor; }

    void SetGammaCorrection(const float v) { GammaCorrection = v; }
    float GetGammaCorrection() const { return GammaCorrection; }

    bool Init(const SEngineParams&) override;
    void Exit() override;

    void Render();
private:
    IGraphics* Graphics = nullptr;
    CResources* Resources = nullptr;
    Matrix4 ViewMatrix;
    Matrix4 ProjectionMatrix;
    Vector3 CameraPosition;
    std::vector<CRenderable3D*> Renderables;
    std::vector<CLight*> Lights;
    std::unique_ptr<IFrameBuffer> DefaultFrameBuffer;
    std::unique_ptr<IVertexBuffer> QuadVertexBuffer;
    IShader* ScreenShader = nullptr;
    std::vector<PostEffectPtr> Effects;
    //
    Color AmbientColor = Color(0.1f, 0.1f, 0.1f);
    float GammaCorrection = 1.4f;
};
