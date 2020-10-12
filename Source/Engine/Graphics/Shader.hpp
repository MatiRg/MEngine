#pragma once
#include "../Resources/Resource.hpp"
#include "../Resources/ResourceFactory.hpp"
#include "../Math/Rect2.hpp"
#include "../Math/Color.hpp"
#include "../Math/Vector3.hpp"
#include "../Math/Vector4.hpp"
#include "../Math/Matrix3.hpp"
#include "../Math/Matrix4.hpp"

class IGraphics;
class ITexture2D;

// Shader takes no file extension
class IShader: public IResource
{
public:
    IShader(const std::string& Name, CResources* Resources):
        IResource( Name, Resources )
    {
    }
    virtual ~IShader() = default;

    RESOURCE(IShader)

    virtual void Bind() = 0;
    virtual void UnBind() = 0;

    // Max 4 Textures [0, 3]; Texture, SamplerName, Index
    virtual void SetTexture(ITexture2D*, const std::string&, const int) = 0;
    virtual void SetInteger(const std::string&, const int) = 0;
    virtual void SetFloat(const std::string&, const float) = 0;
    virtual void SetVector2(const std::string&, const Vector2&) = 0;
    virtual void SetVector3(const std::string&, const Vector3&) = 0;
    virtual void SetVector4(const std::string&, const Vector4&) = 0;
    virtual void SetColor(const std::string&, const Color&) = 0;
    virtual void SetMatrix3(const std::string&, const Matrix3&) = 0;
    virtual void SetMatrix4(const std::string&, const Matrix4&) = 0;
};

//

class CShaderFactory: public TResourceFactory<IShader>
{
public:
    CShaderFactory(IGraphics*);

    std::unique_ptr<IResource> CreateResource(const std::string&, CResources*) override;
private:
    IGraphics* Graphics = nullptr;
};