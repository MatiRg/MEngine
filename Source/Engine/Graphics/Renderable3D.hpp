#pragma once
#include "../Math/Matrix4.hpp"
#include "Material.hpp"
#include "VertexBuffer.hpp"
#include <memory>
#include <vector>

enum class ERenderableType
{
    Solid,
    Transparent,
    Light // ?
};

class IRenderable3D
{
public:
    IRenderable3D(ERenderableType);
    virtual ~IRenderable3D();

    ERenderableType GetType() const { return Type; }
	
    bool HasMaterial() const { return GetMaterial(); }
    virtual CMaterial* GetMaterial() const = 0;

    bool HasVertexBuffer() const { return GetVertexBuffer(); }
    virtual IVertexBuffer* GetVertexBuffer() const = 0;

    void SetMatrix(const Matrix4& aMatrix) { Matrix = aMatrix; }
    const Matrix4& GetMatrix() const { return Matrix; }
protected:
    Matrix4 Matrix;
private:
    ERenderableType Type;
};