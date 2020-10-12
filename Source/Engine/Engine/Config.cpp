#include "Config.hpp"
#include "../Core/Utils.hpp"
#include "../Core/Log.hpp"

CConfig::CConfig(const std::string& aFile):
    File(aFile)
{
    LOG( ESeverity::Debug ) << "Config File - " << File << " - Created\n";
}

CConfig::~CConfig()
{
    LOG( ESeverity::Debug ) << "Config File - " << File << " - Destroyed\n";
}

bool CConfig::Load()
{
    return Document.Load(File);
}

bool CConfig::Save()
{
    return Document.Save(File);
}

void CConfig::SetInt(const std::string& Category, const std::string& Name, const int Value)
{
	GetElement(Category, Name, true)->SetInt(Value);
}

void CConfig::SetFloat(const std::string& Category, const std::string& Name, const float Value)
{
	GetElement(Category, Name, true)->SetFloat(Value);
}

void CConfig::SetBool(const std::string& Category, const std::string& Name, const bool Value)
{
	GetElement(Category, Name, true)->SetBool(Value);
}

void CConfig::SetString(const std::string& Category, const std::string& Name, const std::string& Value)
{
	GetElement(Category, Name, true)->SetString(Value);
}

void CConfig::SetVector2(const std::string& Category, const std::string& Name, const Vector2& Value)
{
	GetElement(Category, Name, true)->SetVector2(Value);
}

void CConfig::SetVector3(const std::string& Category, const std::string& Name, const Vector3& Value)
{
	GetElement(Category, Name, true)->SetVector3(Value);
}

void CConfig::SetColor(const std::string& Category, const std::string& Name, const Color& Value)
{
	GetElement(Category, Name, true)->SetColor(Value);
}

void CConfig::SetRect2(const std::string& Category, const std::string& Name, const Rect2& Value)
{
	GetElement(Category, Name, true)->SetRect2(Value);
}

int CConfig::GetInt(const std::string& Category, const std::string& Name, const int Default)
{
	CXMLElement* Element = GetElement(Category, Name, false);
	if (!Element)
	{
		return Default;
	}
	return Element->GetInt(Default);
}

float CConfig::GetFloat(const std::string& Category, const std::string& Name, const float Default)
{
	CXMLElement* Element = GetElement(Category, Name, false);
	if (!Element)
	{
		return Default;
	}
	return Element->GetFloat(Default);
}

bool CConfig::GetBool(const std::string& Category, const std::string& Name, const bool Default)
{
	CXMLElement* Element = GetElement(Category, Name, false);
	if (!Element)
	{
		return Default;
	}
	return Element->GetBool(Default);
}

std::string CConfig::GetString(const std::string& Category, const std::string& Name, const std::string& Default)
{
	CXMLElement* Element = GetElement(Category, Name, false);
	if (!Element)
	{
		return Default;
	}
	return Element->GetString();
}

Vector2 CConfig::GetVector2(const std::string& Category, const std::string& Name, const Vector2& Default)
{
	CXMLElement* Element = GetElement(Category, Name, false);
	if (!Element)
	{
		return Default;
	}
	return Element->GetVector2(Default);
}

Vector3 CConfig::GetVector3(const std::string& Category, const std::string& Name, const Vector3& Default)
{
	CXMLElement* Element = GetElement(Category, Name, false);
	if (!Element)
	{
		return Default;
	}
	return Element->GetVector3(Default);
}

Color CConfig::GetColor(const std::string& Category, const std::string& Name, const Color& Default)
{
	CXMLElement* Element = GetElement(Category, Name, false);
	if (!Element)
	{
		return Default;
	}
	return Element->GetColor(Default);
}

Rect2 CConfig::GetRect2(const std::string& Category, const std::string& Name, const Rect2& Default)
{
	CXMLElement* Element = GetElement(Category, Name, false);
	if (!Element)
	{
		return Default;
	}
	return Element->GetRect2(Default);
}

CXMLElement* CConfig::GetElement(const std::string& Category, const std::string& Name, const bool Create)
{
	CXMLElement* CategoryElement = Document.GetElement(Category);
	if (!CategoryElement )
	{
		if (Create)
		{
			CategoryElement = Document.NewElement(Category);
		}
		else
		{
			return nullptr;
		}
	}
	CXMLElement* NameElment = CategoryElement->GetElement(Name);
	if (!NameElment && Create)
	{
		NameElment = CategoryElement->NewElement(Name);
	}
	return NameElment;
}