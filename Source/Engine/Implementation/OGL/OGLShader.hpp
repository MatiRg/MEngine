#pragma once
#include "../../Graphics/Shader.hpp"
#include "OGLUtils.hpp"

class COGLShader final: public IShader
{
public:
    COGLShader(const std::string&, CResources*);
    ~COGLShader();

    // x no file ext, Will Load:
    // x.vs - vertex shader
    // x.fs - fragment shader
    bool Load() override;

    bool Find() override;
    //
    bool Exist() override;

    bool IsValid() const override { return Valid; }

    void Bind() override;
    void UnBind() override;

    void SetTexture(ITexture2D*, const std::string&, const int) override;
    void SetInteger(const std::string&, const int) override;
    void SetFloat(const std::string&, const float) override;
    void SetVector2(const std::string&, const Vector2&) override;
    void SetVector3(const std::string&, const Vector3&) override;
    void SetVector4(const std::string&, const Vector4&) override;
    void SetColor(const std::string&, const Color&) override;
    void SetMatrix3(const std::string&, const Matrix3&) override;
    void SetMatrix4(const std::string&, const Matrix4&) override;
private:
    GLuint Handle = 0u;
    std::string FileName;
    bool Valid = false;
};
