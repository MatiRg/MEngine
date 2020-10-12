#pragma once
#include "../Core/XML.hpp"
#include <string>

/*
Configuration File:
Structure is static and it is:
<Category>
    <Name> Value </Name>
    ...
</Category>
...
*/
class CConfig final: public NonCopyableMovable
{
public:
    // File is not loaded
    CConfig(const std::string&);
    ~CConfig();

    bool Load();
    bool Save();

    // Category, Name, Value
    void SetInt(const std::string&, const std::string&, const int);
	// Category, Name, Value
	void SetFloat(const std::string&, const std::string&, const float);
	// Category, Name, Value
	void SetBool(const std::string&, const std::string&, const bool);
	// Category, Name, Value
	void SetString(const std::string&, const std::string&, const std::string&);
	// Category, Name, Value
	void SetVector2(const std::string&, const std::string&, const Vector2&);
	// Category, Name, Value
	void SetVector3(const std::string&, const std::string&, const Vector3&);
	// Category, Name, Value
	void SetColor(const std::string&, const std::string&, const Color&);
	// Category, Name, Value
	void SetRect2(const std::string&, const std::string&, const Rect2&);

    // Category, Name, Default
    int GetInt(const std::string&, const std::string&, const int);
	// Category, Name, Default
	float GetFloat(const std::string&, const std::string&, const float);
	// Category, Name, Default
	bool GetBool(const std::string&, const std::string&, const bool);
	// Category, Name, Default
	std::string GetString(const std::string&, const std::string&, const std::string&);
	// Category, Name, Default
	Vector2 GetVector2(const std::string&, const std::string&, const Vector2&);
	// Category, Name, Default
	Vector3 GetVector3(const std::string&, const std::string&, const Vector3&);
	// Category, Name, Default
	Color GetColor(const std::string&, const std::string&, const Color&);
	// Category, Name, Default
	Rect2 GetRect2(const std::string&, const std::string&, const Rect2&);

	const CXMLDocument& GetXMLDocument() const { return Document; }
private:
	CXMLElement* GetElement(const std::string&, const std::string&, const bool);
private:
    std::string File;
    CXMLDocument Document;
};