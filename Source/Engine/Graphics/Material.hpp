#pragma once
#include "../Resources/ResourceManager.hpp"
#include "Shader.hpp"
#include "Texture2D.hpp"
#include "GraphicsTypes.hpp"
#include <vector>
#include <variant>

class CResources;
class CMaterial;

enum class EMaterialVariableType
{
    None,
    Int,
    Float,
    Vector2,
    Vector3,
    Vector4,
    Color,
    Texture
};

enum class EPassType
{
    Solid,
    Transparent
};

class CMaterialVariable
{
public:
    CMaterialVariable(CMaterial*, const std::string&);
    ~CMaterialVariable();

    void Bind(IShader*);

    void SetValue(const int);
    void SetValue(const float);
    void SetValue(const Vector2&);
    void SetValue(const Vector3&);
    void SetValue(const Vector4&);
    void SetValue(const Color&);
    void SetValue(ITexture2D*);

    const std::string& GetName() const { return Name; }
protected:
    CMaterial* Material = nullptr;
    std::string Name; // Name in Shader Uniform
    EMaterialVariableType Type = EMaterialVariableType::None;
    std::variant<int, float, Vector2, Vector3, Vector4, Color, ITexture2D*> Value;
};

class CMaterial: public IResource
{
    friend CMaterialVariable;
public:
    CMaterial();
    CMaterial(const std::string&);
    ~CMaterial();

    RESOURCE(CMaterial)

    bool Load(CResources*, const ResourceCreateMap&) override;

    void Bind();
    void UnBind();

    bool HasVariable(const std::string&);

    // Set or Modify Variable
    void SetInt(const std::string&, const int);
    // Set or Modify Variable
    void SetFloat(const std::string&, const float);
    // Set or Modify Variable
    void SetVector2(const std::string&, const Vector2&);
    // Set or Modify Variable
    void SetVector3(const std::string&, const Vector3&);
    // Set or Modify Variable
    void SetVector4(const std::string&, const Vector4&);
    // Set or Modify Variable
    void SetColor(const std::string&, const Color&);
    // Set or Modify Variable
    void SetTexture(const std::string&, ITexture2D*);

    void SetShader(IShader* aShader) { Shader = aShader; }
    bool HasShader() const { return Shader; }
    IShader* GetShader() const { return Shader; }

    void SetPassType(const EPassType aPassType) { PassType = aPassType; }
    EPassType GetPassType() const { return PassType; }
private:
    int GetNextTextureIndex();
    CMaterialVariable* CreateOrGetVariable(const std::string&);
    CMaterialVariable* GetVariable(const std::string&);
private:
    int TextureIndexPool = 0;
    IShader* Shader = nullptr;
    std::vector<CMaterialVariable*> Variables;
    EPassType PassType = EPassType::Solid;
};

class CMaterialManager: public TResourceManager<CMaterial>
{
public:
    RESOURCE_MANAGER(CMaterialManager)
protected:
    std::unique_ptr<IResource> MakeResource(const std::string&, const ResourceCreateMap&) override;
};