#include "XML.hpp"
#include "Utils.hpp"
#include "../System/MemoryManager.hpp"
#include <cstdio>
#include <array>
#include <algorithm>
#include <tinyxml2.h>

CXMLObject::CXMLObject(CXMLDocument* aDocument):
    Document(aDocument)
{
    ADD_MEMORY_RECORD(this);
}

CXMLObject::~CXMLObject()
{
    ERASE_MEMORY_RECORD(this);
}

void CXMLObject::SetInt(const int Arg)
{
    std::array<char, 256> Buffor;
    Buffor.fill('\0');
    std::snprintf(Buffor.data(), 255, "%d", Arg);
    SetValue(Buffor.data());
}

void CXMLObject::SetBool(const bool Arg)
{
    SetValue(Arg ? "True" : "False");
}

void CXMLObject::SetFloat(const float Arg)
{
    std::array<char, 256> Buffor;
    Buffor.fill('\0');
    std::snprintf(Buffor.data(), 255, "%.3f", Arg);
    SetValue(Buffor.data());
}

void CXMLObject::SetString(const std::string& Arg)
{
    SetValue(Arg);
}

void CXMLObject::SetUnsignedInt(const std::size_t Arg)
{
    std::array<char, 256> Buffor;
    Buffor.fill('\0');
    std::snprintf(Buffor.data(), 255, "%zu", Arg);
    SetValue(Buffor.data());
}

void CXMLObject::SetVector2(const Vector2& Arg)
{
    std::array<char, 256> Buffor;
    Buffor.fill('\0');
    std::snprintf(Buffor.data(), 255, "%.3f %.3f", Arg.x, Arg.y);
    SetValue(Buffor.data());
}

void CXMLObject::SetVector3(const Vector3& Arg)
{
    std::array<char, 256> Buffor;
    Buffor.fill('\0');
    std::snprintf(Buffor.data(), 255, "%.3f %.3f %.3f", Arg.x, Arg.y, Arg.z);
    SetValue(Buffor.data());
}

void CXMLObject::SetVector4(const Vector4& Arg)
{
    std::array<char, 256> Buffor;
    Buffor.fill('\0');
    std::snprintf(Buffor.data(), 255, "%.3f %.3f %.3f %.3f", Arg.x, Arg.y, Arg.z, Arg.w);
    SetValue(Buffor.data());
}

void CXMLObject::SetRect2(const Rect2& Arg)
{
    std::array<char, 256> Buffor;
    Buffor.fill('\0');
    std::snprintf(Buffor.data(), 255, "%.3f %.3f %.3f %.3f", Arg.GetX(), Arg.GetY(), Arg.GetWidth(), Arg.GetHeight());
    SetValue(Buffor.data());
}

void CXMLObject::SetIntRect2(const IntRect2& Arg)
{
    std::array<char, 256> Buffor;
    Buffor.fill('\0');
    std::snprintf(Buffor.data(), 255, "%d %d %d %d", Arg.GetX(), Arg.GetY(), Arg.GetWidth(), Arg.GetHeight());
    SetValue(Buffor.data());
}

void CXMLObject::SetColor(const Color& Arg)
{
    std::array<char, 256> Buffor;
    Buffor.fill('\0');
    std::snprintf(Buffor.data(), 255, "%.3f %.3f %.3f %.3f", Arg.GetR(), Arg.GetG(), Arg.GetB(), Arg.GetA());
    SetValue(Buffor.data());
}

void CXMLObject::SetMatrix3(const Matrix3& Arg)
{
    std::array<char, 256> Buffor;
    Buffor.fill('\0');
    std::snprintf(Buffor.data(), 255, "%.3f %.3f %.3f %.3f %.3f %.3f %.3f %.3f %.3f",
Arg[0], Arg[1], Arg[2], Arg[3], Arg[4], Arg[5], Arg[6], Arg[7], Arg[8] );
    SetValue(Buffor.data());
}

void CXMLObject::SetMatrix4(const Matrix4& Arg)
{
    std::array<char, 256> Buffor;
    Buffor.fill('\0');
    std::snprintf(Buffor.data(), 255, "%.3f %.3f %.3f %.3f %.3f %.3f %.3f %.3f %.3f %.3f %.3f %.3f %.3f %.3f %.3f %.3f",
Arg[0], Arg[1], Arg[2], Arg[3], Arg[4], Arg[5], Arg[6], Arg[7], Arg[8], Arg[9], Arg[10], Arg[11], Arg[12], Arg[13], Arg[14], Arg[15] );
    SetValue(Buffor.data());
}

void CXMLObject::SetQuaternion(const Quaternion& Arg)
{
    std::array<char, 256> Buffor;
    Buffor.fill('\0');
    std::snprintf(Buffor.data(), 255, "%.3f %.3f %.3f %.3f", Arg.x, Arg.y, Arg.z, Arg.w);
    SetValue(Buffor.data());
}

int CXMLObject::GetInt(const int Default)
{
    int Arg;
    if( std::sscanf(GetValue().c_str(), "%d", &Arg) != 1 )
    {
        LOG(ESeverity::Warning) << "Invalid Value in XML File - " << Document->GetFileName() << " - Param/Attribute Name - " << Name << "\n";
        return Default;
    }
    return Arg;
}

bool CXMLObject::GetBool(const bool Default)
{
    std::string tmp = Utils::ToLower(GetValue());
    if( tmp == "true" || tmp == "yes" || tmp == "1")
    {
        return true;
    }
    if( tmp == "false" || tmp == "no" || tmp == "0")
    {
        return false;
    }
    LOG(ESeverity::Warning) << "Invalid Value in XML File - " << Document->GetFileName() << " - Param/Attribute Name - " << Name << "\n";
    return Default;
}

float CXMLObject::GetFloat(const float Default)
{
    float Arg;
    if( std::sscanf(GetValue().c_str(), "%f", &Arg) != 1 )
    {
        LOG(ESeverity::Warning) << "Invalid Value in XML File - " << Document->GetFileName() << " - Param/Attribute Name - " << Name << "\n";
        return Default;
    }
    return Arg;
}

std::string CXMLObject::GetString()
{
    return GetValue();
}

std::size_t CXMLObject::GetUnsignedInt(const std::size_t Default)
{
    std::size_t Arg;
    if( std::sscanf(GetValue().c_str(), "%zu", &Arg) != 1 )
    {
        LOG(ESeverity::Warning) << "Invalid Value in XML File - " << Document->GetFileName() << " - Param/Attribute Name - " << Name << "\n";
        return Default;
    }
    return Arg;
}

Vector2 CXMLObject::GetVector2(const Vector2& Default)
{
    Vector2 Arg;
    if( std::sscanf(GetValue().c_str(), "%f %f", &Arg.x, &Arg.y) != 2 )
    {
        LOG(ESeverity::Warning) << "Invalid Value in XML File - " << Document->GetFileName() << " - Param/Attribute Name - " << Name << "\n";
        return Default;
    }
    return Arg;
}

Vector3 CXMLObject::GetVector3(const Vector3& Default)
{
    Vector3 Arg;
    if( std::sscanf(GetValue().c_str(), "%f %f %f", &Arg.x, &Arg.y, &Arg.z) != 3 )
    {
        LOG(ESeverity::Warning) << "Invalid Value in XML File - " << Document->GetFileName() << " - Param/Attribute Name - " << Name << "\n";
        return Default;
    }
    return Arg;
}

Vector4 CXMLObject::GetVector4(const Vector4& Default)
{
    Vector4 Arg;
    if( std::sscanf(GetValue().c_str(), "%f %f %f %f", &Arg.x, &Arg.y, &Arg.z, &Arg.w) != 4 )
    {
        LOG(ESeverity::Warning) << "Invalid Value in XML File - " << Document->GetFileName() << " - Param/Attribute Name - " << Name << "\n";
        return Default;
    }
    return Arg;
}

Rect2 CXMLObject::GetRect2(const Rect2& Default)
{
    float x, y, w, h;
    if( std::sscanf(GetValue().c_str(), "%f %f %f %f", &x, &y, &w, &h) != 4 )
    {
        LOG(ESeverity::Warning) << "Invalid Value in XML File - " << Document->GetFileName() << " - Param/Attribute Name - " << Name << "\n";
        return Default;
    }
    return Rect2(x, y, w, h);
}

IntRect2 CXMLObject::GetIntRect2(const IntRect2& Default)
{
    int x, y, w, h;
    if( std::sscanf(GetValue().c_str(), "%d %d %d %d", &x, &y, &w, &h) != 4 )
    {
        LOG(ESeverity::Warning) << "Invalid Value in XML File - " << Document->GetFileName() << " - Param/Attribute Name - " << Name << "\n";
        return Default;
    }
    return IntRect2(x, y, w, h);
}

Color CXMLObject::GetColor(const Color& Default)
{
    float r, g, b, a;
    if( std::sscanf(GetValue().c_str(), "%f %f %f %f", &r, &g, &b, &a) != 4 )
    {
        LOG(ESeverity::Warning) << "Invalid Value in XML File - " << Document->GetFileName() << " - Param/Attribute Name - " << Name << "\n";
        return Default;
    }
    return Color(r, g, b, a);
}

Matrix3 CXMLObject::GetMatrix3(const Matrix3& Default)
{
    Matrix3 Arg;
    if( std::sscanf(GetValue().c_str(), "%f %f %f %f %f %f %f %f %f",
&Arg[0], &Arg[1], &Arg[2], &Arg[3], &Arg[4], &Arg[5], &Arg[6], &Arg[7], &Arg[8] ) != 9 )
    {
        LOG(ESeverity::Warning) << "Invalid Value in XML File - " << Document->GetFileName() << " - Param/Attribute Name - " << Name << "\n";
        return Default;
    }
    return Arg;
}

Matrix4 CXMLObject::GetMatrix4(const Matrix4& Default)
{
    Matrix4 Arg;
    if( std::sscanf(GetValue().c_str(), "%f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f",
&Arg[0], &Arg[1], &Arg[2], &Arg[3], &Arg[4], &Arg[5], &Arg[6], &Arg[7], &Arg[8], &Arg[9], &Arg[10], &Arg[11], &Arg[12], &Arg[13], &Arg[14], &Arg[15] ) != 16 )
    {
        LOG(ESeverity::Warning) << "Invalid Value in XML File - " << Document->GetFileName() << " - Param/Attribute Name - " << Name << "\n";
        return Default;
    }
    return Arg;
}

Quaternion CXMLObject::GetQuaternion(const Quaternion& Default)
{
    Quaternion Arg;
    if (std::sscanf(GetValue().c_str(), "%f %f %f %f", &Arg.x, &Arg.y, &Arg.z, &Arg.w) != 4)
    {
        LOG(ESeverity::Warning) << "Invalid Value in XML File - " << Document->GetFileName() << " - Param/Attribute Name - " << Name << "\n";
        return Default;
    }
    return Arg;
}

//

CXMLElement::CXMLElement(CXMLDocument* aDocument):
    CXMLObject(aDocument)
{
}

CXMLElement::~CXMLElement()
{
    ClearAttributes();
    ClearElements();
}

CXMLElement* CXMLElement::NewElement(const std::string& ElementName)
{
    CXMLElement* Tmp = new CXMLElement(Document);
    Tmp->SetName(ElementName);
    Tmp->SetParent(this);
    return Tmp;
}

bool CXMLElement::HasElement(const std::string& ElementName) const
{
    return GetElement(ElementName);
}

CXMLElement* CXMLElement::GetElement(const std::string& ElementName) const
{
    auto Iterator = std::find_if(Children.begin(), Children.end(), [&](CXMLElement* Object)
    {
        return Object->GetName() == ElementName;
    });
    if( Iterator != Children.end() )
    {
        return *Iterator;
    }
    return nullptr;
}

XMLElementArray CXMLElement::GetElements(const std::string& ElementName) const
{
    XMLElementArray array;
    for(const auto& i: Children)
    {
        if( i->GetName() == ElementName )
        {
            array.push_back(i);
        }
    }
    return array;
}

CXMLAttribute* CXMLElement::NewAttribute(const std::string& AttributeName)
{
    CXMLAttribute* Tmp = GetAttribute(AttributeName);
    if( !Tmp )
    {
        Tmp = new CXMLAttribute(Document);
        Tmp->SetName(AttributeName);
        AddAttribute(Tmp);
    }
    return Tmp;
}

bool CXMLElement::HasAttribute(const std::string& AttributeName) const
{
    return GetAttribute(AttributeName);
}

CXMLAttribute* CXMLElement::GetAttribute(const std::string& AttributeName) const
{
    auto Iterator = std::find_if(Attributes.begin(), Attributes.end(), [&](CXMLAttribute* Object)
    {
        return Object->GetName() == AttributeName;
    });
    if( Iterator != Attributes.end() )
    {
        return *Iterator;
    }
    return nullptr;
}

void CXMLElement::SetStringAttribute(const std::string& Name, const std::string& Arg)
{
    CXMLAttribute* Attribute = NewAttribute(Name);
    Attribute->SetString(Arg);
}

void CXMLElement::SetIntAttribute(const std::string& Name, const int Arg)
{
    CXMLAttribute* Attribute = NewAttribute(Name);
    Attribute->SetInt(Arg);
}

void CXMLElement::SetFloatAttribute(const std::string& Name, const float Arg)
{
    CXMLAttribute* Attribute = NewAttribute(Name);
    Attribute->SetFloat(Arg);
}

void CXMLElement::SetUnsignedIntAttribute(const std::string& Name, const std::size_t Arg)
{
    CXMLAttribute* Attribute = NewAttribute(Name);
    Attribute->SetUnsignedInt(Arg);
}

void CXMLElement::SetBoolAttribute(const std::string& Name, const bool Arg)
{
    CXMLAttribute* Attribute = NewAttribute(Name);
    Attribute->SetBool(Arg);
}

void CXMLElement::SetVector2Attribute(const std::string& Name, const Vector2& Arg)
{
    CXMLAttribute* Attribute = NewAttribute(Name);
    Attribute->SetVector2(Arg);
}

void CXMLElement::SetVector3Attribute(const std::string& Name, const Vector3& Arg)
{
    CXMLAttribute* Attribute = NewAttribute(Name);
    Attribute->SetVector3(Arg);
}

void CXMLElement::SetVector4Attribute(const std::string& Name, const Vector4& Arg)
{
    CXMLAttribute* Attribute = NewAttribute(Name);
    Attribute->SetVector4(Arg);
}

void CXMLElement::SetRect2Attribute(const std::string& Name, const Rect2& Arg)
{
    CXMLAttribute* Attribute = NewAttribute(Name);
    Attribute->SetRect2(Arg);
}

void CXMLElement::SetIntRect2Attribute(const std::string& Name, const IntRect2& Arg)
{
    CXMLAttribute* Attribute = NewAttribute(Name);
    Attribute->SetIntRect2(Arg);
}

void CXMLElement::SetColorAttribute(const std::string& Name, const Color& Arg)
{
    CXMLAttribute* Attribute = NewAttribute(Name);
    Attribute->SetColor(Arg);
}

void CXMLElement::SetMatrix3Attribute(const std::string& Name, const Matrix3& Arg)
{
    CXMLAttribute* Attribute = NewAttribute(Name);
    Attribute->SetMatrix3(Arg);
}

void CXMLElement::SetMatrix4Attribute(const std::string& Name, const Matrix4& Arg)
{
    CXMLAttribute* Attribute = NewAttribute(Name);
    Attribute->SetMatrix4(Arg);
}

void CXMLElement::SetQuaternionAttribute(const std::string& Name, const Quaternion& Arg)
{
    CXMLAttribute* Attribute = NewAttribute(Name);
    Attribute->SetQuaternion(Arg);
}

std::string CXMLElement::GetStringAttribute(const std::string& Name, const std::string& Default)
{
    CXMLAttribute* Attribute = GetAttribute(Name);
    if( !Attribute )
    {
        return Default;
    }
    return Attribute->GetString();
}

int CXMLElement::GetIntAttribute(const std::string& Name, const int Default)
{
    CXMLAttribute* Attribute = GetAttribute(Name);
    if( !Attribute )
    {
        return Default;
    }
    return Attribute->GetInt(Default);
}

float CXMLElement::GetFloatAttribute(const std::string& Name, const float Default)
{
    CXMLAttribute* Attribute = GetAttribute(Name);
    if( !Attribute )
    {
        return Default;
    }
    return Attribute->GetFloat(Default);
}

std::size_t CXMLElement::GetUnsignedIntAttribute(const std::string& Name, const std::size_t Default)
{
    CXMLAttribute* Attribute = GetAttribute(Name);
    if( !Attribute )
    {
        return Default;
    }
    return Attribute->GetUnsignedInt(Default);
}

bool CXMLElement::GetBoolAttribute(const std::string& Name, const bool Default)
{
    CXMLAttribute* Attribute = GetAttribute(Name);
    if( !Attribute )
    {
        return Default;
    }
    return Attribute->GetBool(Default);
}

Vector2 CXMLElement::GetVector2Attribute(const std::string& Name, const Vector2& Default)
{
    CXMLAttribute* Attribute = GetAttribute(Name);
    if( !Attribute )
    {
        return Default;
    }
    return Attribute->GetVector2(Default);
}

Vector3 CXMLElement::GetVector3Attribute(const std::string& Name, const Vector3& Default)
{
    CXMLAttribute* Attribute = GetAttribute(Name);
    if( !Attribute )
    {
        return Default;
    }
    return Attribute->GetVector3(Default);
}

Vector4 CXMLElement::GetVector4Attribute(const std::string& Name, const Vector4& Default)
{
    CXMLAttribute* Attribute = GetAttribute(Name);
    if( !Attribute )
    {
        return Default;
    }
    return Attribute->GetVector4(Default);
}

Rect2 CXMLElement::GetRect2Attribute(const std::string& Name, const Rect2& Default)
{
    CXMLAttribute* Attribute = GetAttribute(Name);
    if( !Attribute )
    {
        return Default;
    }
    return Attribute->GetRect2(Default);
}

IntRect2 CXMLElement::GetIntRect2Attribute(const std::string& Name, const IntRect2& Default)
{
    CXMLAttribute* Attribute = GetAttribute(Name);
    if( !Attribute )
    {
        return Default;
    }
    return Attribute->GetIntRect2(Default);
}

Color CXMLElement::GetColorAttribute(const std::string& Name, const Color& Default)
{
    CXMLAttribute* Attribute = GetAttribute(Name);
    if( !Attribute )
    {
        return Default;
    }
    return Attribute->GetColor(Default);
}

Matrix3 CXMLElement::GetMatrix3Attribute(const std::string& Name, const Matrix3& Default)
{
    CXMLAttribute* Attribute = GetAttribute(Name);
    if( !Attribute )
    {
        return Default;
    }
    return Attribute->GetMatrix3(Default);
}

Matrix4 CXMLElement::GetMatrix4Attribute(const std::string& Name, const Matrix4& Default)
{
    CXMLAttribute* Attribute = GetAttribute(Name);
    if( !Attribute )
    {
        return Default;
    }
    return Attribute->GetMatrix4(Default);
}

Quaternion CXMLElement::GetQuaternionAttribute(const std::string& Name, const Quaternion& Default)
{
    CXMLAttribute* Attribute = GetAttribute(Name);
    if (!Attribute)
    {
        return Default;
    }
    return Attribute->GetQuaternion(Default);
}

void CXMLElement::SetParent(CXMLElement* NewParent)
{
    if( NewParent == Parent || this == NewParent )
    {
        return;
    }

    if( Parent )
    {
        Parent->RemoveChild(this);
    }
    Parent = NewParent;
    if( Parent )
    {
        Parent->AddChild(this);
    }
}

void CXMLElement::AddChild(CXMLElement* Child)
{
    Children.push_back(Child);
}

void CXMLElement::RemoveChild(CXMLElement* Child)
{
    auto Iterator = std::find_if(Children.begin(), Children.end(), [&](CXMLElement* Object)
    {
        return Object == Child;
    });
    if( Iterator != Children.end() )
    {
        Children.erase(Iterator);
    }
}

void CXMLElement::AddAttribute(CXMLAttribute* Tmp)
{
    Attributes.push_back(Tmp);
}

void CXMLElement::ClearAttributes()
{
    for(auto& i: Attributes)
    {
        delete i;
    }
    Attributes.clear();
}

void CXMLElement::ClearElements()
{
    for(auto& i: Children)
    {
        delete i;
    }
    Children.clear();
}

//

CXMLDocument::CXMLDocument():
    CXMLElement(this)
{
}

CXMLDocument::~CXMLDocument()
{
}

namespace
{
    void LoadXML(tinyxml2::XMLElement* From, CXMLElement* To)
    {
        CXMLElement* Child = To->NewElement( From->Name() );
        if( From->GetText() )
        {
            Child->SetValue( From->GetText() );
        }

        for(auto Atribute = From->FirstAttribute(); Atribute; Atribute = Atribute->Next())
        {
            CXMLAttribute* ChildAttribute = Child->NewAttribute( Atribute->Name() );
            ChildAttribute->SetValue( Atribute->Value() );
        }

        for(auto Element = From->FirstChildElement(); Element; Element = Element->NextSiblingElement())
        {
            LoadXML( Element, Child );
        }
    }

    void SaveXML(CXMLElement* From, tinyxml2::XMLElement* To, tinyxml2::XMLDocument* Doc)
    {
        tinyxml2::XMLElement* Tmp = Doc->NewElement( From->GetName().c_str() );
        if( To )
        {
            To->InsertEndChild(Tmp);
        }
        else
        {
            Doc->InsertEndChild(Tmp);
        }

        if( !From->GetValue().empty() )
        {
            Tmp->SetText( From->GetValue().c_str() );
        }

        for(const auto& Atribute: From->GetAttributes())
        {
            Tmp->SetAttribute(Atribute->GetName().c_str(), Atribute->GetValue().c_str());
        }

        for(const auto& Child: From->GetElements())
        {
            SaveXML(Child, Tmp, Doc);
        }
    }
}

bool CXMLDocument::Load(const std::string& aFile)
{
    ClearValue();
    ClearAttributes();
    ClearElements();

    File = aFile;

    tinyxml2::XMLDocument Doc(true, tinyxml2::COLLAPSE_WHITESPACE);
    tinyxml2::XMLError r = Doc.LoadFile( File.c_str() );
    if( r != tinyxml2::XML_SUCCESS )
    {
        LOG(ESeverity::Error) << "Unable to Load XML File - " << File << "\n";
        return false;
    }

    for(auto Element = Doc.FirstChildElement(); Element; Element = Element->NextSiblingElement())
    {
        LoadXML(Element, this);
    }

    return true;
}

bool CXMLDocument::Save()
{
    if( !File.empty() )
    {
        tinyxml2::XMLDocument Doc;

        for(const auto& Element: GetElements())
        {
            SaveXML(Element, nullptr, &Doc);
        }

        tinyxml2::XMLError r = Doc.SaveFile( File.c_str() );
        if( r != tinyxml2::XML_SUCCESS )
        {
            LOG(ESeverity::Error) << "Unable to Save XML File - " << File << " - " << Doc.ErrorStr() << "\n";
            return false;
        }
        return true;
    }
    return false;
}

bool CXMLDocument::Save(const std::string& aFile)
{
    File = aFile;
    return Save();
}

namespace XML
{
	void SaveFloat(CXMLElement* Root, const std::string& Name, const float Value)
	{
		if (!Root)
		{
			return;
		}
		CXMLElement* Container = Root->NewElement(Name);
		Container->SetFloat(Value);
	}

	float LoadFloat(CXMLElement* Root, const std::string& Name, const float DefaultValue)
	{
		if (!Root)
		{
			return DefaultValue;
		}
		CXMLElement* Container = Root->GetElement(Name);
		if (!Container)
		{
			return DefaultValue;
		}
		return Container->GetFloat(DefaultValue);
	}

	void SaveInt(CXMLElement* Root, const std::string& Name, const int Value)
	{
		if (!Root)
		{
			return;
		}
		CXMLElement* Container = Root->NewElement(Name);
		Container->SetInt(Value);
	}

	int LoadInt(CXMLElement* Root, const std::string& Name, const int DefaultValue)
	{
		if (!Root)
		{
			return DefaultValue;
		}
		CXMLElement* Container = Root->GetElement(Name);
		if (!Container)
		{
			return DefaultValue;
		}
		return Container->GetInt(DefaultValue);
	}

	void SaveBool(CXMLElement* Root, const std::string& Name, const bool Value)
	{
		if (!Root)
		{
			return;
		}
		CXMLElement* Container = Root->NewElement(Name);
		Container->SetBool(Value);
	}

	bool LoadBool(CXMLElement* Root, const std::string& Name, const bool DefaultValue)
	{
		if (!Root)
		{
			return DefaultValue;
		}
		CXMLElement* Container = Root->GetElement(Name);
		if (!Container)
		{
			return DefaultValue;
		}
		return Container->GetBool(DefaultValue);
	}

	void SaveString(CXMLElement* Root, const std::string& Name, const std::string& Value)
	{
		if (!Root)
		{
			return;
		}
		CXMLElement* Container = Root->NewElement(Name);
		Container->SetString(Value);
	}

	std::string LoadString(CXMLElement* Root, const std::string& Name, const std::string& DefaultValue)
	{
		if (!Root)
		{
			return DefaultValue;
		}
		CXMLElement* Container = Root->GetElement(Name);
		if (!Container)
		{
			return DefaultValue;
		}
		return Container->GetString();
	}

	void SaveVector2(CXMLElement* Root, const std::string& Name, const Vector2& Value)
	{
		if (!Root)
		{
			return;
		}
		CXMLElement* Container = Root->NewElement(Name);
		Container->SetVector2(Value);
	}

	Vector2 LoadVector2(CXMLElement* Root, const std::string& Name, const Vector2& DefaultValue)
	{
		if (!Root)
		{
			return DefaultValue;
		}
		CXMLElement* Container = Root->GetElement(Name);
		if (!Container)
		{
			return DefaultValue;
		}
		return Container->GetVector2(DefaultValue);
	}

	void SaveVector3(CXMLElement* Root, const std::string& Name, const Vector3& Value)
	{
		if (!Root)
		{
			return;
		}
		CXMLElement* Container = Root->NewElement(Name);
		Container->SetVector3(Value);
	}

	Vector3 LoadVector3(CXMLElement* Root, const std::string& Name, const Vector3& DefaultValue)
	{
		if (!Root)
		{
			return DefaultValue;
		}
		CXMLElement* Container = Root->GetElement(Name);
		if (!Container)
		{
			return DefaultValue;
		}
		return Container->GetVector3(DefaultValue);
	}

	void SaveRect2(CXMLElement* Root, const std::string& Name, const Rect2& Value)
	{
		if (!Root)
		{
			return;
		}
		CXMLElement* Container = Root->NewElement(Name);
		Container->SetRect2(Value);
	}

	Rect2 LoadRect2(CXMLElement* Root, const std::string& Name, const Rect2& DefaultValue)
	{
		if (!Root)
		{
			return DefaultValue;
		}
		CXMLElement* Container = Root->GetElement(Name);
		if (!Container)
		{
			return DefaultValue;
		}
		return Container->GetRect2(DefaultValue);
	}

	void SaveColor(CXMLElement* Root, const std::string& Name, const Color& Value)
	{
		if (!Root)
		{
			return;
		}
		CXMLElement* Container = Root->NewElement(Name);
		Container->SetColor(Value);
	}

	Color LoadColor(CXMLElement* Root, const std::string& Name, const Color& DefaultValue)
	{
		if (!Root)
		{
			return DefaultValue;
		}
		CXMLElement* Container = Root->GetElement(Name);
		if (!Container)
		{
			return DefaultValue;
		}
		return Container->GetColor(DefaultValue);
	}

    void SaveQuaternion(CXMLElement* Root, const std::string& Name, const Quaternion& Value)
    {
        if (!Root)
        {
            return;
        }
        CXMLElement* Container = Root->NewElement(Name);
        Container->SetQuaternion(Value);
    }

    Quaternion LoadQuaternion(CXMLElement* Root, const std::string& Name, const Quaternion& DefaultValue)
    {
        if (!Root)
        {
            return DefaultValue;
        }
        CXMLElement* Container = Root->GetElement(Name);
        if (!Container)
        {
            return DefaultValue;
        }
        return Container->GetQuaternion(DefaultValue);
    }

    void SaveMatrix4(CXMLElement* Root, const std::string& Name, const Matrix4& Value)
    {
        if (!Root)
        {
            return;
        }
        CXMLElement* Container = Root->NewElement(Name);
        Container->SetMatrix4(Value);
    }

    Matrix4 LoadMatrix4(CXMLElement* Root, const std::string& Name, const Matrix4& DefaultValue)
    {
        if (!Root)
        {
            return DefaultValue;
        }
        CXMLElement* Container = Root->GetElement(Name);
        if (!Container)
        {
            return DefaultValue;
        }
        return Container->GetMatrix4(DefaultValue);
    }
}
