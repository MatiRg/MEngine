#pragma once
#include "../../Graphics/VertexBuffer.hpp"
#include "OGLUtils.hpp"

class COGLVertexBufferColorTexCoords: public IVertexBufferColorTexCoords
{
public:
    COGLVertexBufferColorTexCoords(const bool);
    ~COGLVertexBufferColorTexCoords();

    bool Create() override;

    bool IsValid() const override { return Valid; }

    void Bind() override;
    void Draw(const EPrimitiveMode, const std::size_t) override;
    void UnBind() override;

    //

    void FeedPositions(const std::vector<Vector3>&) override;
    void FeedColors(const std::vector<Color>&) override;
    void FeedTextureCoords(const std::vector<Vector2>&) override;
private:
    GLuint VAO = 0u;
    GLuint Position = 0u;
    GLuint Color = 0u;
    GLuint Coords = 0u;
    bool Valid = false;
};
