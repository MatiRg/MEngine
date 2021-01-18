#pragma once
#include "../Engine/EngineModule.hpp"
#include "../Math/Color.hpp"
#include "Renderable3D.hpp"
#include "Light.hpp"
#include <vector>

class IGraphics;

class CRenderer3D: public IEngineModule
{
public:
    CRenderer3D(IGraphics*);
    ~CRenderer3D();

    ENGINE_MODULE(CRenderer3D)

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

    bool Init(const SEngineParams&) override;
    void Exit() override;

    void Render();
private:
    IGraphics* Graphics = nullptr;
    Matrix4 ViewMatrix;
    Matrix4 ProjectionMatrix;
    Vector3 CameraPosition;
    std::vector<CRenderable3D*> Renderables;
    std::vector<CLight*> Lights;
    //
    Color AmbientColor = Color(0.1f, 0.1f, 0.1f);
};
