#pragma once
#include "../Engine/EngineModule.hpp"
#include "Renderable3D.hpp"
#include "Shader.hpp"
#include "VertexBuffer.hpp"
#include <vector>

class CRenderer3D;
class CResources;

class CDebugDrawer: public IEngineModule
{
public:
    CDebugDrawer(CRenderer3D*, CResources*);
    ~CDebugDrawer();

    ENGINE_MODULE(CDebugDrawer)

    bool Init(const SEngineParams&) override;
    void Exit() override;
private:
    CRenderer3D* Renderer3D = nullptr;
    CResources* Resources = nullptr;
};
