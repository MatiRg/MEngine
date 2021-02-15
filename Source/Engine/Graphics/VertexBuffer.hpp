#pragma once
#include "../Math/Color.hpp"
#include "../Math/Vector3.hpp"
#include "../Math/Vector2.hpp"
#include "../Core/NonCopyable.hpp"
#include "../System/MemoryManager.hpp"
#include "GraphicsTypes.hpp"
#include <vector>

// Index of Element is same in shader layout
enum class EVertexElement
{
    Position  = 0,
    Color     = 1,
    TexCoord0 = 2,
    Normal    = 3
};

enum class EVertexType
{
    Vector3 = 0,
    Vector2,
    Color
};

struct SVertexElement
{
    EVertexElement Element = EVertexElement::Position;
    EVertexType Type = EVertexType::Vector3;
    std::size_t Size = 0u;
    std::size_t ElementSize = 0u; // Number of elements -> Vector3 = 3 etc. 

    SVertexElement(const EVertexElement);
};

class IVertexBuffer: public NonCopyableMovable
{
public:
    IVertexBuffer()
    {
        ADD_MEMORY_RECORD(this);
    }

    virtual ~IVertexBuffer()
    {
        ERASE_MEMORY_RECORD(this);
    }

    // Vertex Elements, Dynamic
    virtual bool Create(const std::vector<SVertexElement>&, const bool) = 0;

    virtual bool IsDynamic() const = 0;
    virtual bool IsValid() const = 0;

    virtual bool HasElement(const EVertexElement) const = 0;
    virtual bool HasIndices() const = 0;

    virtual void SetData(const EVertexElement, const std::vector<Vector3>&) = 0;
    virtual void SetData(const EVertexElement, const std::vector<Vector2>&) = 0;
    virtual void SetData(const EVertexElement, const std::vector<Color>&) = 0;

    virtual void SetIndices(const std::vector<unsigned int>&) = 0;

    virtual void Bind() = 0;
    virtual void Draw(const EPrimitiveType) = 0;
    virtual void Draw(const EPrimitiveType, const std::size_t) = 0;
    virtual void UnBind() = 0;
};