#pragma once
#include "../Core/NonCopyable.hpp"
#include "../Math/Matrix4.hpp"
#include "Material.hpp"
#include "VertexBuffer.hpp"
#include <memory>
#include <vector>

enum class ERenderableType
{
    Object,
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

    void SetPrimitiveType(const EPrimitiveType aPrimitiveType) { PrimitiveType = aPrimitiveType; }
    EPrimitiveType GetPrimitiveType() const { return PrimitiveType; }

    void SetMatrix(const Matrix4&);
    const Matrix4& GetMatrix() const { return Matrix; }

    const Vector3& GetPosition() const { return Position; }
protected:
    Matrix4 Matrix;
    CMaterial* Material = nullptr;
    IVertexBuffer* VertexBuffer = nullptr;
    EPrimitiveType PrimitiveType = EPrimitiveType::Triangles;
private:
    ERenderableType Type = ERenderableType::Object;
    Vector3 Position;
};

using Renderable3DVec = std::vector<CRenderable3D*>;