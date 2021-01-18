#pragma once
#include "../Core/NonCopyable.hpp"
#include "../Math/Matrix4.hpp"
#include "Material.hpp"
#include "VertexBuffer.hpp"
#include <memory>
#include <vector>

enum class ERenderableType
{
    Solid,
    Light
};

class CRenderable3D: public NonCopyableMovable
{
public:
    CRenderable3D(ERenderableType);
    virtual ~CRenderable3D();

    ERenderableType GetRenderableType() const { return Type; }
	
    bool HasMaterial() const { return GetMaterial(); }
    CMaterial* GetMaterial() const { return Material; };
    void SetMaterial(CMaterial* aMaterial) { Material = aMaterial; }

    bool HasVertexBuffer() const { return GetVertexBuffer(); }
    IVertexBuffer* GetVertexBuffer() const { return VertexBuffer; };
    void SetVertexBuffer(IVertexBuffer* aVertexBuffer) { VertexBuffer = aVertexBuffer; }

    void SetMatrix(const Matrix4& aMatrix) { Matrix = aMatrix; }
    const Matrix4& GetMatrix() const { return Matrix; }
protected:
    Matrix4 Matrix;
    CMaterial* Material = nullptr;
    IVertexBuffer* VertexBuffer = nullptr;
private:
    ERenderableType Type;
};