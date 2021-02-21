#include "Material.hpp"
#include "../Core/XML.hpp"
#include "../Core/Utils.hpp"
#include "../Core/Types.hpp"
#include "../Resources/Resources.hpp"
#include <unordered_map>

CMaterialVariable::CMaterialVariable(CMaterial* aMaterial, const std::string& aName):
    Material(aMaterial),
    Name(aName)
{
    ADD_MEMORY_RECORD(this);
}

CMaterialVariable::~CMaterialVariable()
{
    ERASE_MEMORY_RECORD(this);
}

void CMaterialVariable::SetValue(const int x)
{
    Type = EMaterialVariableType::Int;
    Value = x;
}

void CMaterialVariable::SetValue(const float x)
{
    Type = EMaterialVariableType::Float;
    Value = x;
}

void CMaterialVariable::SetValue(const Vector2& x)
{
    Type = EMaterialVariableType::Vector2;
    Value = x;
}

void CMaterialVariable::SetValue(const Vector3& x)
{
    Type = EMaterialVariableType::Vector3;
    Value = x;
}

void CMaterialVariable::SetValue(const Vector4& x)
{
    Type = EMaterialVariableType::Vector4;
    Value = x;
}

void CMaterialVariable::SetValue(const Color& x)
{
    Type = EMaterialVariableType::Color;
    Value = x;
}

void CMaterialVariable::SetValue(ITexture2D* Texture)
{
    Type = EMaterialVariableType::Texture;
    Value = Texture;
}

void CMaterialVariable::Bind(IShader* Shader)
{
    if (Shader && Shader->HasUniform(Name))
    {
        switch (Type)
        {
        case EMaterialVariableType::Int:
            Shader->SetInteger(Name, std::get<int>(Value));
            break;
        case EMaterialVariableType::Float:
            Shader->SetFloat(Name, std::get<float>(Value));
            break;
        case EMaterialVariableType::Vector2:
            Shader->SetVector2(Name, std::get<Vector2>(Value));
            break;
        case EMaterialVariableType::Vector3:
            Shader->SetVector3(Name, std::get<Vector3>(Value));
            break;
        case EMaterialVariableType::Vector4:
            Shader->SetVector4(Name, std::get<Vector4>(Value));
            break;
        case EMaterialVariableType::Color:
            Shader->SetColor(Name, std::get<Color>(Value));
            break;
        case EMaterialVariableType::Texture:
            Shader->SetTexture(Name, std::get<ITexture2D*>(Value), Material->GetNextTextureIndex());
            break;
        case EMaterialVariableType::None:
        default:
            break;
        }
    }
}

//
namespace Utils
{

    std::string MaterialVariableTypeToString(EMaterialVariableType x)
    {
        switch (x)
        {
        case EMaterialVariableType::Int:
            return "Int";
        case EMaterialVariableType::Float:
            return "Float";
        case EMaterialVariableType::Vector2:
            return "Vector2";
        case EMaterialVariableType::Vector3:
            return "Vector3";
        case EMaterialVariableType::Vector4:
            return "Vector4";
        case EMaterialVariableType::Color:
            return "Color";
        case EMaterialVariableType::Texture:
            return "Texture";
        case EMaterialVariableType::None:
        default:
            return "";
        }
    }

    EMaterialVariableType MaterialVariableTypeFromString(const std::string& x)
    {
        static std::unordered_map<std::string, EMaterialVariableType> LookUp = {
            {"int", EMaterialVariableType::Int},
            {"float", EMaterialVariableType::Float},
            {"vector2", EMaterialVariableType::Vector2},
            {"vector3", EMaterialVariableType::Vector3},
            {"vector4", EMaterialVariableType::Vector4},
            {"color", EMaterialVariableType::Color},
            {"texture", EMaterialVariableType::Texture}
        };
        std::string Tmp = Utils::ToLower(x);

        if (LookUp.count(Tmp))
        {
            return LookUp.at(Tmp);
        }
        else
        {
            return EMaterialVariableType::None;
        }
    }
}

//

CMaterial::CMaterial():
    IResource("")
{
    // Not Loaded from File
    Valid = true;
}
	
CMaterial::CMaterial(const std::string& Name):
    IResource(Name)
{
}

CMaterial::~CMaterial()
{
    for (const auto& i : Variables)
    {
        delete i;
    }
}

bool CMaterial::Load(CResources* Resources, const ResourceCreateMap&)
{
    CXMLDocument Document;
    if( !Document.Load(GetPath()) )
    {
        LOG(ESeverity::Error) << "Unable to Load Material File - " << GetPath() << "\n";
        return false;
    }

    CXMLElement* Root = Document.GetElement("Material");
    if (!Root)
    {
        LOG(ESeverity::Error) << "No Root Element in Material File - " << GetPath() << "\n";
        return false;
    }

    CXMLElement* ShaderElement = Root->GetElement("Shader");
    if (!ShaderElement)
    {
        LOG(ESeverity::Error) << "No Shader Element in Material File - " << GetPath() << "\n";
        return false;
    }

    std::string ShaderName = ShaderElement->GetStringAttribute("Name", "Default.shader");

    StringVec Defines;
    XMLElementArray DefinesElements = ShaderElement->GetElements("Define");
    for (const auto& i : DefinesElements)
    {
        std::string Tmp = i->GetStringAttribute("Value", "");
        if (Tmp.empty())
        {
            continue;
        }
        Defines.push_back(Tmp);
    }

    ResourceCreateMap ResVars;
    ResVars[RESOURCES_VAR_DEFINES] = Defines;

    IShader* Tmp = Resources->CreateResource<IShader>(ShaderName, ResVars );
    if (!Tmp)
    {
        return false;
    }
    SetShader( Tmp );

    //  Variables
    XMLElementArray VariableElements = Root->GetElements("Variable");
    for (const auto& i : VariableElements)
    {
        std::string TmpName = i->GetStringAttribute("Name", "");
        if (TmpName.empty())
        {
            LOG(ESeverity::Warning) << "Empty or Missing Material Variable Name in Material File - " << GetPath() << "\n";
            continue;
        }
    
        std::string TmpType = i->GetStringAttribute("Type", "");
        if (TmpType.empty())
        {
            LOG(ESeverity::Warning) << "Empty or Missing EMaterialVariableType in Material File - " << GetPath() << "\n";
            continue;
        }

        EMaterialVariableType VarType = Utils::MaterialVariableTypeFromString(TmpType);
        if (VarType == EMaterialVariableType::None)
        {
            LOG(ESeverity::Warning) << "Invalid Converted EMaterialVariableType in Material File - " << GetPath() << "\n";
            continue;
        }

        switch (VarType)
        {
        case EMaterialVariableType::Int:
            SetInt( TmpName, i->GetIntAttribute("Value", 0) );
            break;
        case EMaterialVariableType::Float:
            SetFloat(TmpName, i->GetFloatAttribute("Value", 0.0f));
            break;
        case EMaterialVariableType::Vector2:
            SetVector2(TmpName, i->GetVector2Attribute("Value", VECTOR2_ZERO));
            break;
        case EMaterialVariableType::Vector3:
            SetVector3(TmpName, i->GetVector3Attribute("Value", VECTOR3_ZERO));
            break;
        case EMaterialVariableType::Vector4:
            SetVector4(TmpName, i->GetVector4Attribute("Value", VECTOR4_ZERO));
            break;
        case EMaterialVariableType::Color:
            SetColor(TmpName, i->GetColorAttribute("Value", COLOR_GREY) );
            break;
        case EMaterialVariableType::Texture:
            {
                ITexture2D* Texture = Resources->CreateResource<ITexture2D>( i->GetStringAttribute("Value", "") );
                if( !Texture )
                {
                    return false;
                }
                SetTexture(TmpName, Texture);
            }
            break;
        case EMaterialVariableType::None:
        default:
            break;
        }
    }

    Valid = true;
    return true;
}

void CMaterial::Bind()
{
    if( HasShader() )
    {
        GetShader()->Bind();

        TextureIndexPool = 0;
        for (const auto& i : Variables)
        {
            i->Bind(Shader);
        }
    }
}

void CMaterial::UnBind()
{
    if (HasShader())
    {
        GetShader()->UnBind();
    }
}

bool CMaterial::HasVariable(const std::string& Name)
{
    return GetVariable(Name);
}

CMaterialVariable* CMaterial::GetVariable(const std::string& Name)
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

void CMaterial::SetInt(const std::string& Name, const int x)
{
    CreateOrGetVariable(Name)->SetValue(x);
}

void CMaterial::SetFloat(const std::string& Name, const float x)
{
    CreateOrGetVariable(Name)->SetValue(x);
}

void CMaterial::SetVector2(const std::string& Name, const Vector2& x)
{
    CreateOrGetVariable(Name)->SetValue(x);
}

void CMaterial::SetVector3(const std::string& Name, const Vector3& x)
{
    CreateOrGetVariable(Name)->SetValue(x);
}

void CMaterial::SetVector4(const std::string& Name, const Vector4& x)
{
    CreateOrGetVariable(Name)->SetValue(x);
}

void CMaterial::SetColor(const std::string& Name, const Color& x)
{
    CreateOrGetVariable(Name)->SetValue(x);
}

void CMaterial::SetTexture(const std::string& Name, ITexture2D* x)
{
    CreateOrGetVariable(Name)->SetValue(x);
}

CMaterialVariable* CMaterial::CreateOrGetVariable(const std::string& Name)
{
    CMaterialVariable* Tmp = GetVariable(Name);
    if (!Tmp)
    {
        Tmp = new CMaterialVariable(this, Name);
        Variables.push_back(Tmp);
    }
    return Tmp;
}

int CMaterial::GetNextTextureIndex()
{
    return TextureIndexPool++;
}

//

std::unique_ptr<IResource> CMaterialManager::MakeResource(const std::string& Name, const ResourceCreateMap&)
{
    return std::make_unique<CMaterial>(Name);
}