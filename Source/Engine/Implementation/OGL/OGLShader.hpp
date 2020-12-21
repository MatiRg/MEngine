#pragma once
#include "../../Graphics/Shader.hpp"
#include "OGLUtils.hpp"
#include <unordered_map>

class COGLShader final: public IShader
{
public:
    COGLShader(const std::string&);
    ~COGLShader();

    bool Load(CResources*, const ResourceCreateMap&) override;

    void Bind() override;
    void UnBind() override;

    void SetTexture(const std::string&, ITexture2D*, const int) override;
    void SetInteger(const std::string&, const int) override;
    void SetFloat(const std::string&, const float) override;
    void SetVector2(const std::string&, const Vector2&) override;
    void SetVector3(const std::string&, const Vector3&) override;
    void SetVector4(const std::string&, const Vector4&) override;
    void SetColor(const std::string&, const Color&) override;
    void SetMatrix3(const std::string&, const Matrix3&) override;
    void SetMatrix4(const std::string&, const Matrix4&) override;
private:
    int GetUniformIndex(const std::string&);
    bool ProcessFile(CResources*, std::string&, std::stringstream&);
    bool LoadProgram(CResources*, GLuint, const StringVec&);
private:
    GLuint Handle = 0u;
    std::unordered_map<std::string, GLint> UniformCache;
};
