#pragma once
#include "../Math/Color.hpp"
#include "../Math/Vector3.hpp"
#include "../Math/Vector2.hpp"
#include "../Core/NonCopyable.hpp"
#include <vector>

enum class EPrimitiveMode
{
    Triangles = 0,
    Lines,
    Points
};

class IVertexBuffer: public NonCopyableMovable
{
public:
    IVertexBuffer(const bool aDynamic): Dynamic(aDynamic) {}
    virtual ~IVertexBuffer() = default;

    virtual bool Create() = 0;

    bool IsDynamic() const { return Dynamic; }
    virtual bool IsValid() const = 0;

    virtual void Bind() = 0;
    virtual void Draw(const EPrimitiveMode, const std::size_t) = 0;
    virtual void UnBind() = 0;
private:
    bool Dynamic = false;
};

class IVertexBufferColorTexCoords: public IVertexBuffer
{
public:
    IVertexBufferColorTexCoords(const bool Dynamic): IVertexBuffer(Dynamic) {}
    virtual ~IVertexBufferColorTexCoords() = default;

    virtual void FeedPositions(const std::vector<Vector3>&) = 0;
    virtual void FeedColors(const std::vector<Color>&) = 0;
    virtual void FeedTextureCoords(const std::vector<Vector2>&) = 0;
};
