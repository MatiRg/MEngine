#include "PostEffect.hpp"
#include "Graphics.hpp"
#include "../Resources/Resources.hpp"
#include "../System/MemoryManager.hpp"
#include "../Core/Log.hpp"

CPostEffectVariable::CPostEffectVariable(CPostEffect* aEffect, const std::string& aName):
    Effect(aEffect),
    Name(aName)
{
    ADD_MEMORY_RECORD(this);
}

CPostEffectVariable::~CPostEffectVariable()
{
    ERASE_MEMORY_RECORD(this);
}

void CPostEffectVariable::SetValue(const int x)
{
    Type = EPostEffectVariableType::Int;
    Value = x;
}

void CPostEffectVariable::SetValue(const float x)
{
    Type = EPostEffectVariableType::Float;
    Value = x;
}

void CPostEffectVariable::SetValue(const Vector2& x)
{
    Type = EPostEffectVariableType::Vector2;
    Value = x;
}

void CPostEffectVariable::SetValue(const Vector3& x)
{
    Type = EPostEffectVariableType::Vector3;
    Value = x;
}

void CPostEffectVariable::SetValue(const Vector4& x)
{
    Type = EPostEffectVariableType::Vector4;
    Value = x;
}

void CPostEffectVariable::SetValue(const Color& x)
{
    Type = EPostEffectVariableType::Color;
    Value = x;
}

void CPostEffectVariable::SetValue(ITexture2D* Texture)
{
    Type = EPostEffectVariableType::Texture;
    Value = Texture;
}

void CPostEffectVariable::Bind(IShader* Shader)
{
    if (Shader && Shader->HasUniform(Name))
    {
        switch (Type)
        {
        case EPostEffectVariableType::Int:
            Shader->SetInteger(Name, std::get<int>(Value));
            break;
        case EPostEffectVariableType::Float:
            Shader->SetFloat(Name, std::get<float>(Value));
            break;
        case EPostEffectVariableType::Vector2:
            Shader->SetVector2(Name, std::get<Vector2>(Value));
            break;
        case EPostEffectVariableType::Vector3:
            Shader->SetVector3(Name, std::get<Vector3>(Value));
            break;
        case EPostEffectVariableType::Vector4:
            Shader->SetVector4(Name, std::get<Vector4>(Value));
            break;
        case EPostEffectVariableType::Color:
            Shader->SetColor(Name, std::get<Color>(Value));
            break;
        case EPostEffectVariableType::Texture:
            Shader->SetTexture(Name, std::get<ITexture2D*>(Value), Effect->GetNextTextureIndex());
            break;
        case EPostEffectVariableType::None:
        default:
            break;
        }
    }
}

//

CPostEffect::CPostEffect(IGraphics* aGraphics, CResources* aResources, const int aOrder):
	Graphics(aGraphics),
	Resources(aResources),
	Order(aOrder)
{
	ADD_MEMORY_RECORD(this);
}
	
CPostEffect::~CPostEffect()
{
    for (const auto& i : Variables)
    {
        delete i;
    }
	ERASE_MEMORY_RECORD(this);
}

bool CPostEffect::Create(const std::string& ShaderName)
{
    FrameBuffer = Graphics->CreateFrameBuffer();
    if (!FrameBuffer)
    {
        LOG(ESeverity::Error) << "Post Effect: Invalid Frame Buffer\n";
        return false;
    }

    Shader = Resources->CreateResource<IShader>(ShaderName);
    if (!Shader)
    {
        LOG(ESeverity::Error) << "Post Effect: Invalid '" << ShaderName << "' Shader\n";
        return false;
    }

	return true;
}

void CPostEffect::Bind(IFrameBuffer* OldFrameBuffer)
{
    FrameBuffer->Bind();
    Graphics->Clear();
    Shader->Bind();
    //
    TextureIndexPool = 0;
    if (Shader->HasUniform("BackBufferColor"))
    {
        Shader->SetTexture("BackBufferColor", OldFrameBuffer->GetColorAttachment(), TextureIndexPool++);
    }
    if (Shader->HasUniform("BackBufferDepth"))
    {
        Shader->SetTexture("BackBufferDepth", OldFrameBuffer->GetDepthAttachment(), TextureIndexPool++);
    }
    //
    for (const auto& i : Variables)
    {
        i->Bind(Shader);
    }
}

void CPostEffect::UnBind()
{
    Shader->UnBind();
    FrameBuffer->UnBind();
}

bool CPostEffect::HasVariable(const std::string& Name)
{
    return GetVariable(Name);
}

CPostEffectVariable* CPostEffect::GetVariable(const std::string& Name)
{
    for (const auto& i : Variables)
    {
        if (i->GetName() == Name)
        {
            return i;
        }
    }
    return nullptr;
}

void CPostEffect::SetInt(const std::string& Name, const int x)
{
    CreateOrGetVariable(Name)->SetValue(x);
}

void CPostEffect::SetFloat(const std::string& Name, const float x)
{
    CreateOrGetVariable(Name)->SetValue(x);
}

void CPostEffect::SetVector2(const std::string& Name, const Vector2& x)
{
    CreateOrGetVariable(Name)->SetValue(x);
}

void CPostEffect::SetVector3(const std::string& Name, const Vector3& x)
{
    CreateOrGetVariable(Name)->SetValue(x);
}

void CPostEffect::SetVector4(const std::string& Name, const Vector4& x)
{
    CreateOrGetVariable(Name)->SetValue(x);
}

void CPostEffect::SetColor(const std::string& Name, const Color& x)
{
    CreateOrGetVariable(Name)->SetValue(x);
}

void CPostEffect::SetTexture(const std::string& Name, ITexture2D* x)
{
    CreateOrGetVariable(Name)->SetValue(x);
}

CPostEffectVariable* CPostEffect::CreateOrGetVariable(const std::string& Name)
{
    CPostEffectVariable* Tmp = GetVariable(Name);
    if (!Tmp)
    {
        Tmp = new CPostEffectVariable(this, Name);
        Variables.push_back(Tmp);
    }
    return Tmp;
}

int CPostEffect::GetNextTextureIndex()
{
    return TextureIndexPool++;
}