#pragma once
#include "NonCopyable.hpp"
#include "Log.hpp"
#include "../Math/Vector2.hpp"
#include "../Math/Vector3.hpp"
#include "../Math/Rect2.hpp"
#include "../Math/Color.hpp"
#include "../Math/Matrix3.hpp"
#include "../Math/Matrix4.hpp"
#include "../Math/Vector4.hpp"
#include "../Math/Quaternion.hpp"
#include <vector>
#include <string>
#include <type_traits>

class CXMLElement;
class CXMLDocument;

class CXMLObject: public NonCopyableMovable
{
public:
    CXMLObject(CXMLDocument*);
    virtual ~CXMLObject();

    void SetName(const std::string& aName) { Name = aName; }
    const std::string& GetName() const { return Name; }

    void SetValue(const std::string& aValue) { Value = aValue; }
    const std::string& GetValue() const { return Value; }
    void ClearValue() 
    { 
        Value.clear(); 
        Value.shrink_to_fit();
    }

    void SetString(const std::string&);
    void SetInt(const int);
    void SetBool(const bool);
    void SetFloat(const float);
    void SetUnsignedInt(const std::size_t);
    void SetVector2(const Vector2&);
    void SetVector3(const Vector3&);
    void SetVector4(const Vector4&);
    void SetRect2(const Rect2&);
    void SetColor(const Color&);
    void SetMatrix3(const Matrix3&);
    void SetMatrix4(const Matrix4&);
    void SetQuaternion(const Quaternion&);

    //
    std::string GetString();
    // Default Value
    int GetInt(const int);
    // Default Value
    bool GetBool(const bool);
    // Default Value
    float GetFloat(const float);
    // Default Value
    std::size_t GetUnsignedInt(const std::size_t);
    // Default Value
    Vector2 GetVector2(const Vector2&);
    // Default Value
    Vector3 GetVector3(const Vector3&);
    // Default Value
    Vector4 GetVector4(const Vector4&);
    // Default Value
    Rect2 GetRect2(const Rect2&);
    // Default Value
    Color GetColor(const Color&);
    // Default Value
    Matrix3 GetMatrix3(const Matrix3&);
    // Default Value
    Matrix4 GetMatrix4(const Matrix4&);
    // Default Value
    Quaternion GetQuaternion(const Quaternion&);
protected:
    CXMLDocument* Document = nullptr;
private:
    std::string Name;
    std::string Value;
};

using CXMLAttribute = CXMLObject;
using XMLElementArray = std::vector<CXMLElement*>;
using XMLAttributeArray = std::vector<CXMLAttribute*>;

class CXMLElement: public CXMLObject
{
public:
    CXMLElement(CXMLDocument*);
    virtual ~CXMLElement();

    CXMLElement* NewElement(const std::string&);

    bool HasElement(const std::string&) const;
    CXMLElement* GetElement(const std::string&) const;

    XMLElementArray GetElements(const std::string&) const;
    const XMLElementArray& GetElements() const { return Children; }

    // if attribute with same name exist, it will return it
    CXMLAttribute* NewAttribute(const std::string&);

    bool HasAttribute(const std::string&) const;
    CXMLAttribute* GetAttribute(const std::string&) const;
    const XMLAttributeArray& GetAttributes() const { return Attributes; }

    // Name, Value
    void SetStringAttribute(const std::string&, const std::string&);
    // Name, Value
    void SetIntAttribute(const std::string&, const int);
    // Name, Value
    void SetFloatAttribute(const std::string&, const float);
    // Name, Value
    void SetUnsignedIntAttribute(const std::string&, const std::size_t);
    // Name, Value
    void SetBoolAttribute(const std::string&, const bool);
    // Name, Value
    void SetVector2Attribute(const std::string&, const Vector2&);
    // Name, Value
    void SetVector3Attribute(const std::string&, const Vector3&);
    // Name, Value
    void SetVector4Attribute(const std::string&, const Vector4&);
    // Name, Value
    void SetRect2Attribute(const std::string&, const Rect2&);
    // Name, Value
    void SetColorAttribute(const std::string&, const Color&);
    // Name, Value
    void SetMatrix3Attribute(const std::string&, const Matrix3&);
    // Name, Value
    void SetMatrix4Attribute(const std::string&, const Matrix4&);
    // Name, Value
    void SetQuaternionAttribute(const std::string&, const Quaternion&);

    // Name, Default
    std::string GetStringAttribute(const std::string&, const std::string&);
    // Name, Default
    int GetIntAttribute(const std::string&, const int);
    // Name, Default
    float GetFloatAttribute(const std::string&, const float);
    // Name, Default
    std::size_t GetUnsignedIntAttribute(const std::string&, const std::size_t);
    // Name, Default
    bool GetBoolAttribute(const std::string&, const bool);
    // Name, Default
    Vector2 GetVector2Attribute(const std::string&, const Vector2&);
    // Name, Default
    Vector3 GetVector3Attribute(const std::string&, const Vector3&);
    // Name, Default
    Vector4 GetVector4Attribute(const std::string&, const Vector4&);
    // Name, Default
    Rect2 GetRect2Attribute(const std::string&, const Rect2&);
    // Name, Default
    Color GetColorAttribute(const std::string&, const Color&);
    // Name, Default
    Matrix3 GetMatrix3Attribute(const std::string&, const Matrix3&);
    // Name, Default
    Matrix4 GetMatrix4Attribute(const std::string&, const Matrix4&);
    // Name, Default
    Quaternion GetQuaternionAttribute(const std::string&, const Quaternion&);

    CXMLElement* GetParent() { return Parent; }

    void ClearAttributes();
    void ClearElements();
private:
    void SetParent(CXMLElement*);

    void AddChild(CXMLElement*);
    void RemoveChild(CXMLElement*);

    void AddAttribute(CXMLAttribute*);
private:
    XMLElementArray Children;
    CXMLElement* Parent = nullptr;
    XMLAttributeArray Attributes;
};

class CXMLDocument final: public CXMLElement
{
public:
    CXMLDocument();
    ~CXMLDocument();

    bool Load(const std::string&);

    bool Save();
    bool Save(const std::string&);

    const std::string& GetFileName() const { return File; }
private:
    std::string File;
};

namespace XML
{
	// Root, Name, Value
	void SaveFloat(CXMLElement*, const std::string&, const float);
	// Root, Name, Default Value
	float LoadFloat(CXMLElement*, const std::string&, const float);

	// Root, Name, Value
	void SaveInt(CXMLElement*, const std::string&, const int);
	// Root, Name, Default Value
	int LoadInt(CXMLElement*, const std::string&, const int);

	// Root, Name, Value
	void SaveBool(CXMLElement*, const std::string&, const bool);
	// Root, Name, Default Value
	bool LoadBool(CXMLElement*, const std::string&, const bool);

	// Root, Name, Value
	void SaveString(CXMLElement*, const std::string&, const std::string&);
	// Root, Name, Default Value
	std::string LoadString(CXMLElement*, const std::string&, const std::string&);

	// Root, Name, Value
	void SaveVector2(CXMLElement*, const std::string&, const Vector2&);
	// Root, Name, Default Value
	Vector2 LoadVector2(CXMLElement*, const std::string&, const Vector2&);

	// Root, Name, Value
	void SaveVector3(CXMLElement*, const std::string&, const Vector3&);
	// Root, Name, Default Value
	Vector3 LoadVector3(CXMLElement*, const std::string&, const Vector3&);

	// Root, Name, Value
	void SaveRect2(CXMLElement*, const std::string&, const Rect2&);
	// Root, Name, Default Value
	Rect2 LoadRect2(CXMLElement*, const std::string&, const Rect2&);

	// Root, Name, Value
	void SaveColor(CXMLElement*, const std::string&, const Color&);
	// Root, Name, Default Value
	Color LoadColor(CXMLElement*, const std::string&, const Color&);

    // Root, Name, Value
    void SaveQuaternion(CXMLElement*, const std::string&, const Quaternion&);
    // Root, Name, Default Value
    Quaternion LoadQuaternion(CXMLElement*, const std::string&, const Quaternion&);

    // Root, Name, Value
    void SaveMatrix4(CXMLElement*, const std::string&, const Matrix4&);
    // Root, Name, Default Value
    Matrix4 LoadMatrix4(CXMLElement*, const std::string&, const Matrix4&);

	// Root, Name, Value
    template<class Enum,
        std::enable_if_t<std::is_enum<Enum>::value, int> = 0
    >
    void SaveEnum(CXMLElement* Root, const std::string& Name, const Enum Value)
    {
        SaveInt( Root, Name, static_cast<int>(Value) );
    }

	// Root, Name, Default Value
    template<class Enum,
        std::enable_if_t<std::is_enum<Enum>::value, int> = 0
    >
    Enum LoadEnum(CXMLElement* Root, const std::string& Name, const Enum DefaultValue)
    {
        int Tmp = LoadInt(Root, Name, static_cast<int>(DefaultValue));
        return static_cast<Enum>(Tmp);
    }
}
