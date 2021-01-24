#pragma once
#include "Shader.hpp"
#include "Texture2D.hpp"
#include "FrameBuffer.hpp"
#include "../Core/NonCopyable.hpp"
#include "../Math/Color.hpp"
#include "../Math/Vector3.hpp"
#include <memory>
#include <variant>

class IGraphics;
class CResources;
class CPostEffect;

enum class EPostEffectVariableType
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

class CPostEffectVariable
{
public:
    CPostEffectVariable(CPostEffect*, const std::string&);
    ~CPostEffectVariable();

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
    CPostEffect* Effect = nullptr;
    std::string Name; // Name in Shader Uniform
    EPostEffectVariableType Type = EPostEffectVariableType::None;
    std::variant<int, float, Vector2, Vector3, Vector4, Color, ITexture2D*> Value;
};

class CPostEffect: public NonCopyableMovable
{
    friend CPostEffectVariable;
public:
    CPostEffect(IGraphics*, CResources*, const int);
    ~CPostEffect();

    // Shader Name
    bool Create(const std::string&);

    // Last Frame Buffer
    void Bind(IFrameBuffer*);
    void UnBind();

    void SetEnabled(const bool x) { Enabled = x; }
    bool IsEnabled() const { return Enabled; }

    int GetOrder() const { return Order; }
    void SetOrder(const int n) { Order = n; }

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

    IFrameBuffer* GetFrameBuffer() const { return FrameBuffer.get(); }
    IShader* GetShader() const { return Shader; }
    ITexture2D* GetColorAttachment() const { return FrameBuffer->GetColorAttachment(); }
private:
    int GetNextTextureIndex();
    CPostEffectVariable* CreateOrGetVariable(const std::string&);
    CPostEffectVariable* GetVariable(const std::string&);
private:
    IGraphics* Graphics = nullptr;
    CResources* Resources = nullptr;
    IShader* Shader = nullptr;
    std::unique_ptr<IFrameBuffer> FrameBuffer;
    int Order = 0;
    int TextureIndexPool = 0;
    std::vector<CPostEffectVariable*> Variables;
    bool Enabled = true;
};

using PostEffectPtr = std::unique_ptr<CPostEffect>;