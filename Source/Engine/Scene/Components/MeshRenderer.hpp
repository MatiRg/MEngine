#pragma once
#include "../Component.hpp"
#include "../../Graphics/Renderable3D.hpp"
#include "../../Graphics/Material.hpp"
#include "../../Graphics/Model.hpp"

class CMeshRenderer: public IComponent
{
public:
    CMeshRenderer(CEngine*);

    COMPONENT(CMeshRenderer)

    bool OnLoad(CXMLElement*) override;
    bool OnSave(CXMLElement*) override;

    void OnCreate() override;

    void OnRender() override;

    void SetMesh(CMesh* aMesh);
    CMesh* GetMesh() const { return Mesh; }
private:
    CRenderable3D Renderable;
    CMesh* Mesh = nullptr;
};
