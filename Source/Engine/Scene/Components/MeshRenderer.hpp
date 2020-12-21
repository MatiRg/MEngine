#pragma once
#include "../Component.hpp"
#include "../../Graphics/Renderable3D.hpp"
#include "../../Graphics/Material.hpp"
#include "../../Graphics/Model.hpp"

class CMeshRenderer: public IComponent, public IRenderable3D
{
public:
    CMeshRenderer(CEngine*);

    // IComponent

    COMPONENT(CMeshRenderer)

    bool OnLoad(CXMLElement*) override;
    bool OnSave(CXMLElement*) override;

    void OnCreate() override;

    void OnLateUpdate(const float) override;
    void OnRender() override;

    // IRenderable3D

    CMaterial* GetMaterial() const override { return Material; }
    IVertexBuffer* GetVertexBuffer() const override;

    // Self

    void SetMaterial(CMaterial* aMaterial) { Material = aMaterial; }

    void SetMesh(CMesh* aMesh) { Mesh = aMesh; }
    CMesh* GetMesh() const { return Mesh; }
private:
    CMaterial* Material = nullptr;
    CMesh* Mesh = nullptr;
};
