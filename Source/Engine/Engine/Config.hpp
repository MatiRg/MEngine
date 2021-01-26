#pragma once
#include "../Core/XML.hpp"
#include <string>

/*
*/
/**
  \class CConfig
  \brief Encapsulates XML File.Can be used to Load/Save config files

  Configuration File Format:
  Structure is static and it is:
  \code
  <Category>
	<Name> Value </Name>
	...
  </Category>
  ...
  \endcode
*/
class CConfig final: public NonCopyableMovable
{
public:
    // File is not loaded
    /**
     * \brief Construct CConfig with give File Name, File is not loaded
     * \param aFile File Name
     */
    CConfig(const std::string& aFile);
    ~CConfig();

	//! Load Config File
    bool Load();
	//! Save Config File
    bool Save();

    //! Category, Name, Value
    void SetInt(const std::string&, const std::string&, const int);
	//! Category, Name, Value
	void SetFloat(const std::string&, const std::string&, const float);
	//! Category, Name, Value
	void SetBool(const std::string&, const std::string&, const bool);
	//! Category, Name, Value
	void SetString(const std::string&, const std::string&, const std::string&);
	//! Category, Name, Value
	void SetVector2(const std::string&, const std::string&, const Vector2&);
	//1 Category, Name, Value
	void SetVector3(const std::string&, const std::string&, const Vector3&);
	//! Category, Name, Value
	void SetColor(const std::string&, const std::string&, const Color&);
	//! Category, Name, Value
	void SetRect2(const std::string&, const std::string&, const Rect2&);

    //! Category, Name, Default which is returned in case of error
    int GetInt(const std::string&, const std::string&, const int);
	//! Category, Name, Default which is returned in case of error
	float GetFloat(const std::string&, const std::string&, const float);
	//! Category, Name, Default which is returned in case of error
	bool GetBool(const std::string&, const std::string&, const bool);
	//! Category, Name, Default which is returned in case of error
	std::string GetString(const std::string&, const std::string&, const std::string&);
	//! Category, Name, Default which is returned in case of error
	Vector2 GetVector2(const std::string&, const std::string&, const Vector2&);
	//! Category, Name, Default which is returned in case of error
	Vector3 GetVector3(const std::string&, const std::string&, const Vector3&);
	//! Category, Name, Default which is returned in case of error
	Color GetColor(const std::string&, const std::string&, const Color&);
	//! Category, Name, Default which is returned in case of error
	Rect2 GetRect2(const std::string&, const std::string&, const Rect2&);

	const CXMLDocument& GetXMLDocument() const { return Document; }
private:
	CXMLElement* GetElement(const std::string&, const std::string&, const bool);
private:
    std::string File;
    CXMLDocument Document;
};