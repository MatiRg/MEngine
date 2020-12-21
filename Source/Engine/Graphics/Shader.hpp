#pragma once
#include "../Resources/ResourceManager.hpp"
#include "../Math/Rect2.hpp"
#include "../Math/Color.hpp"
#include "../Math/Vector3.hpp"
#include "../Math/Vector4.hpp"
#include "../Math/Matrix3.hpp"
#include "../Math/Matrix4.hpp"

class IGraphics;
class ITexture2D;
class CResources;

enum class EUniformType
{
    Int,
    Float,
    Vector2,
    Vector3,
    Vector4,
    Matrix3,
    Matrix4,
    Texture,
    Unknown
};

struct SUniform
{
    std::string Name;
    EUniformType Type;
    int Count;
    bool IsArray;

    SUniform(const std::string& aName, const EUniformType aType, const int aCount, const bool aIsArray):
        Name(aName),
        Type(aType),
        Count(aCount),
        IsArray(aIsArray)
    {
    }
};

using UniformArray = std::vector<SUniform>;
using ShaderDefines = std::vector<std::string>;

// For Resource Var Map
static constexpr char RESOURCES_VAR_DEFINES[] = "__DEFINES";

// Shader takes no file extension
class IShader: public IResource
{
public:
    IShader(const std::string& Name):
        IResource( Name )
    {
    }
    virtual ~IShader() = default;

    RESOURCE(IShader)

    bool HasUniform(const std::string&) const;
    const UniformArray& GetUniforms() const { return Uniforms; }

    virtual void Bind() = 0;
    virtual void UnBind() = 0;

    // Max 8 Textures [0, 7]; SamplerName, Texture, Index
    virtual void SetTexture(const std::string&, ITexture2D*, const int) = 0;
    virtual void SetInteger(const std::string&, const int) = 0;
    virtual void SetFloat(const std::string&, const float) = 0;
    virtual void SetVector2(const std::string&, const Vector2&) = 0;
    virtual void SetVector3(const std::string&, const Vector3&) = 0;
    virtual void SetVector4(const std::string&, const Vector4&) = 0;
    virtual void SetColor(const std::string&, const Color&) = 0; // Interpreted as Vector4
    virtual void SetMatrix3(const std::string&, const Matrix3&) = 0;
    virtual void SetMatrix4(const std::string&, const Matrix4&) = 0;
protected:
    UniformArray Uniforms;
};

//

class CShaderManager : public TResourceManager<IShader>
{
public:
    CShaderManager(IGraphics*);

    RESOURCE_MANAGER(CShaderManager)
protected:
    std::unique_ptr<IResource> MakeResource(const std::string&, const ResourceCreateMap&) override;
private:
    IGraphics* Graphics = nullptr;
};