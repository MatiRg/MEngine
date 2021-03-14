#pragma once
#include "../Engine/EngineModule.hpp"
#include "../Math/Color.hpp"
#include "../Math/Vector2.hpp"
#include "../Math/Vector3.hpp"
#include "../Math/Vector4.hpp"
#include "../Math/Rect2.hpp"
#include "ImGUIFlags.hpp"
#include <vector>

class IInput;
class IGraphics;

/**
  \class CImGUI
  \brief Debug UI system, use in OnGUI() callback
*/
class CImGUI final: public IEngineModule
{
public:
    CImGUI(IInput*, IGraphics*);
    ~CImGUI();

    ENGINE_MODULE(CImGUI)

    bool Init(const SEngineParams&) override;
    void OnInit() override;
    void OnEvent(const CEvent&) override;
    void OnUpdate(const float) override;
    void OnLateUpdate(const float) override;
    void OnGUI() override;

    void Draw();

    //! Title
    void Begin(const std::string&);
    //! Title, Flags
    void Begin(const std::string&, const EImGUIWindowFlags);
    //! Title, Position, Size
    void Begin(const std::string&, const IntVector2&, const IntVector2&);
    //! Title, Position, Size, Flags
    void Begin(const std::string&, const IntVector2&, const IntVector2&, const EImGUIWindowFlags);
    void End();

    //! Name, Selected, Values
    void Combo(const std::string&, int&, const std::vector<std::string>&);

    //! Name, Value
    void CheckBox(const std::string&, bool&);

    //! Text
    void Text(const std::string&);
    //! Text, Color
    void TextColor(const std::string&, const Color&);
    //! Name, Value
    void InputText(const std::string&, std::string&);
    //! Name, Value
    void InputFloat(const std::string&, float&);
    //! Name, Value
    void InputVector2(const std::string&, Vector2&);
    //! Name, Value
    void InputVector3(const std::string&, Vector3&);
    //! Name, Value
    void InputVector4(const std::string&, Vector4&);
    //! Name, Value
    void InputColor(const std::string&, Color&);
    //! Name, Value
    void InputRect2(const std::string&, Rect2&);
    //! Name
    bool Button(const std::string&);
    //1 Next widget is in same line
    void SameLine();
    //! Collapsible Header, Name. Return true if Opened
    bool Header(const std::string&);

    //! Name/ID
    bool BeginItemContextMenu(const std::string&);
    bool BeginWindowContextMenu(const std::string&);
    //! Name/ID
    void OpenContextMenu(const std::string&);
    void EndContextMenu();

    void OpenPopup(const std::string&);
    void ClosePopup();
    bool BeginPopup(const std::string&);
    void EndPopup();
    //! Name
    bool BeginMenu(const std::string&);
    //! Name, Enabled
    bool BeginMenu(const std::string&, const bool); 
    //! Name
    bool MenuItem(const std::string&);
    //! Name, Enabled
    bool MenuItem(const std::string&, const bool);
    //! Horizontal Line
    void Separator();
    void EndMenu();

    bool BeginMainMenuBar();
    void EndMainMenuBar();

    void ShowDemoWindow();
private:
    IInput* Input = nullptr;
    IGraphics* Graphics = nullptr;
    bool DemoWindow = false;
};
