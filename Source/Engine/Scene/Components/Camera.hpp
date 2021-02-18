#pragma once
#include "../Component.hpp"
#include "../../Math/Matrix4.hpp"

class CCamera: public IComponent
{
public:
    CCamera(CEngine*);

    COMPONENT(CCamera)

    bool OnLoad(CXMLElement*) override;
    bool OnSave(CXMLElement*) override;

    void OnCreate() override;
    void OnDestroy() override;

    void OnLateUpdate(const float) override;

    void SetFOV(const float);
    float GetFOV() const { return Fov; }

    void SetAspect(const float);
    float GetAspect() const { return Aspect; }

    void SetFarClipPlane(const float);
    float GetFarClipPlane() const { return Far; }

    void SetNearClipPlane(const float);
    float GetNearClipPlane() const { return Near; }

    const Matrix4& GetView() const;
    const Matrix4& GetProjection() const;
private:
    float Fov = 75.0f;
    float Aspect = 4.0f / 3.0f;
    float Far = 1000.0f;
    float Near = 0.03f;
    mutable bool ProjectionDirty = true;
    mutable Matrix4 ProjectionMatrix = Matrix4::IDENTITY;
    mutable bool ViewDirty = true;
    mutable Matrix4 ViewMatrix = Matrix4::IDENTITY;
};