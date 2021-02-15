#pragma once
#include "../../Graphics/VertexBuffer.hpp"
#include "OGLUtils.hpp"

class COGLVertexBuffer: public IVertexBuffer
{
public:
    COGLVertexBuffer();
    ~COGLVertexBuffer();

    bool Create(const std::vector<SVertexElement>&, const bool) override;

    bool IsDynamic() const override { return Dynamic; }
    bool IsValid() const override { return Valid; }

    bool HasElement(const EVertexElement) const override;
    bool HasIndices() const override { return HasEBO; }

    void SetData(const EVertexElement, const std::vector<Vector3>&) override;
    void SetData(const EVertexElement, const std::vector<Vector2>&) override;
    void SetData(const EVertexElement, const std::vector<Color>&) override;

    void SetIndices(const std::vector<unsigned int>&) override;

    void Bind() override;
    void Draw(const EPrimitiveType) override;
    void Draw(const EPrimitiveType, const std::size_t) override;
    void UnBind() override;
private:
    struct SVBO
    {
        GLuint Index;
        SVertexElement Descriptor;

        SVBO(const GLuint aIndex, const SVertexElement& aDescriptor):
            Index(aIndex),
            Descriptor(aDescriptor)
        {
        }
    };
private:
    const SVBO* GetVBO(const EVertexElement);
private:
    bool Dynamic = false;
    std::size_t VertexCount = 0u;
    GLuint VAO = 0u;
    std::vector<SVBO> VBOs;
    bool HasEBO = false;
    GLuint EBO = 0u;
    std::size_t IndicesCount = 0u;
    bool Valid = false;
};
