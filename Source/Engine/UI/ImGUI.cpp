#include "ImGUI.hpp"
#include "../Input/Event.hpp"
#include "../Input/EventTypes.hpp"
#include "../Input/Input.hpp"
#include "../Graphics/Graphics.hpp"
#include "../Core/Log.hpp"
#include "../Engine//Config.hpp"
#include <array>
#include <imgui/imgui.h>
#include <imgui/imgui_stdlib.h>

namespace
{
    const char* ImGui_GetClipboardText(void* Ptr)
    {
        IInput* Input = static_cast<IInput*>(Ptr);
        return Input->GetClipBoardText().c_str();
    }

    void ImGui_SetClipboardText(void* Ptr, const char* Text)
    {
        IInput* Input = static_cast<IInput*>(Ptr);
        Input->SetClipBoardText(Text);
    }
}

CImGUI::CImGUI(IInput* aInput, IGraphics* aGraphics):
    IEngineModule( "ImGUI" ),
    Input(aInput),
    Graphics(aGraphics)
{
}

CImGUI::~CImGUI()
{
    Graphics->GetImGUIRenderer()->Destroy();
    if( ImGui::GetCurrentContext() )
    {
        ImGui::DestroyContext();
    }
}

bool CImGUI::Init(const SEngineParams& Params)
{
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    ImGui::StyleColorsClassic();

    io.DisplaySize.x = static_cast<float>(Params.Width);
    io.DisplaySize.y = static_cast<float>(Params.Height);
    io.IniFilename = nullptr;
    io.BackendPlatformName = "MyEngine";

    io.KeyMap[ImGuiKey_Tab] = static_cast<int>(EKey::Tab);
    io.KeyMap[ImGuiKey_LeftArrow] = static_cast<int>(EKey::Left);
    io.KeyMap[ImGuiKey_RightArrow] = static_cast<int>(EKey::Right);
    io.KeyMap[ImGuiKey_UpArrow] = static_cast<int>(EKey::Up);
    io.KeyMap[ImGuiKey_DownArrow] = static_cast<int>(EKey::Down);
    io.KeyMap[ImGuiKey_PageUp] = static_cast<int>(EKey::PageUp);
    io.KeyMap[ImGuiKey_PageDown] = static_cast<int>(EKey::PageDown);
    io.KeyMap[ImGuiKey_Home] = static_cast<int>(EKey::Home);
    io.KeyMap[ImGuiKey_End] = static_cast<int>(EKey::End);
    io.KeyMap[ImGuiKey_Insert] = static_cast<int>(EKey::Insert);
    io.KeyMap[ImGuiKey_Delete] = static_cast<int>(EKey::Delete);
    io.KeyMap[ImGuiKey_Backspace] = static_cast<int>(EKey::BackSpace);
    io.KeyMap[ImGuiKey_Enter] = static_cast<int>(EKey::Return);
    io.KeyMap[ImGuiKey_Escape] = static_cast<int>(EKey::Escape);
    io.KeyMap[ImGuiKey_A] = static_cast<int>(EKey::A);
    io.KeyMap[ImGuiKey_C] = static_cast<int>(EKey::C);
    io.KeyMap[ImGuiKey_V] = static_cast<int>(EKey::V);
    io.KeyMap[ImGuiKey_X] = static_cast<int>(EKey::X);
    io.KeyMap[ImGuiKey_Y] = static_cast<int>(EKey::Y);
    io.KeyMap[ImGuiKey_Z] = static_cast<int>(EKey::Z);

    io.GetClipboardTextFn = ImGui_GetClipboardText;
    io.SetClipboardTextFn = ImGui_SetClipboardText;
    io.ClipboardUserData = Input;

    if( !Graphics->GetImGUIRenderer()->Init() )
    {
        LOG( ESeverity::Fatal ) << "Unable to create ImGUI Renderer\n";
        return false;
    }

    io.Fonts->AddFontDefault();

    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowRounding = 0.0f;
    style.ChildRounding = 0.0f;
    style.FrameRounding = 0.0f;
    style.PopupRounding = 0.0f;
    style.ScrollbarRounding = 0.0f;
    style.GrabRounding = 0.0f;
    style.TabRounding = 0.0f;
    //
	style.AntiAliasedFill = false;
	style.AntiAliasedLines = false;

    LOG( ESeverity::Info ) << "ImGUI - Init\n";
    return true;
}

void CImGUI::OnInit()
{
}

void CImGUI::OnEvent(const CEvent& Event)
{
    ImGuiIO& io = ImGui::GetIO();
    switch(Event.GetType())
    {
        case EEventType::MouseButton:
        {
            SEventMouseButton* Data = Event.GetData<SEventMouseButton>();
            if( Data )
            {
                if( Data->Key == EMouseKey::LButton ) io.MouseDown[0] = Data->Down;
                else if( Data->Key == EMouseKey::RButton ) io.MouseDown[1] = Data->Down;
                else if( Data->Key == EMouseKey::MButton ) io.MouseDown[2] = Data->Down;
            }
            break;
        }
        case EEventType::MouseMotion:
        {
            SEventMouseMotion* Data = Event.GetData<SEventMouseMotion>();
            if( Data )
            {
                Vector2& MousePosition = Data->Position;
                io.MousePos = ImVec2(MousePosition.x, MousePosition.y);
            }
            break;
        }
        case EEventType::MouseWheel:
        {
            SEventMouseWheel* Data = Event.GetData<SEventMouseWheel>();
            if( Data )
            {
                if( Data->x > 0.0f ) io.MouseWheelH += 1;
                if( Data->x < 0.0f ) io.MouseWheelH -= 1;
                if( Data->y > 0.0f ) io.MouseWheel += 1;
                if( Data->y < 0.0f ) io.MouseWheel -= 1;
            }
            break;
        }
        case EEventType::TextInput:
        {
            SEventTextInput* Data = Event.GetData<SEventTextInput>();
            if( Data )
            {
                io.AddInputCharactersUTF8( Data->Data.c_str() );
            }
            break;
        }
        case EEventType::Keyboard:
        {
            SEventKeyboard* Data = Event.GetData<SEventKeyboard>();
            if( Data )
            {
                int Key = static_cast<int>(Data->Key);
                io.KeysDown[Key] = Data->Down;
                io.KeyShift = ((Data->Mod & EKeyMod::Shift) != 0 );
                io.KeyCtrl = ((Data->Mod & EKeyMod::Ctrl) != 0 );
                io.KeyAlt = ((Data->Mod & EKeyMod::Alt) != 0 );
                if( Data->Down && Data->Key == EKey::F11 )
                {
                    DemoWindow = !DemoWindow;
                }
            }
            break;
        }
        case EEventType::WindowSize:
        {
            SEventWindowSize* Data = Event.GetData<SEventWindowSize>();
            if( Data )
            {
                io.DisplaySize = ImVec2(static_cast<float>(Data->x), static_cast<float>(Data->y));
            }
            break;
        }
        default:
            break;
    }
}

void CImGUI::OnUpdate(const float)
{
}

void CImGUI::OnLateUpdate(const float DT)
{
    Graphics->GetImGUIRenderer()->Update(DT);
    ImGuiIO& io = ImGui::GetIO();
    io.DeltaTime = DT;
    ImGui::NewFrame();
}

void CImGUI::OnGUI()
{
    #ifdef DEV_BUILD
    if( DemoWindow )
    {
        ShowDemoWindow();
    }
    #endif // DEV_BUILD
}

void CImGUI::Draw()
{
    ImGui::Render();
    Graphics->GetImGUIRenderer()->Draw( ImGui::GetDrawData() );
}

void CImGUI::Begin(const std::string& Caption)
{
    ImGui::Begin( Caption.c_str() );
}

void CImGUI::Begin(const std::string& Caption, const EImGUIWindowFlags Flg)
{
    ImGuiWindowFlags Flags = ImGuiWindowFlags_None;
    if( Flg & EImGUIWindowFlags::NoTitleBar ) Flags |= ImGuiWindowFlags_NoTitleBar;
    if( Flg & EImGUIWindowFlags::NoResize ) Flags |= ImGuiWindowFlags_NoResize;
    if( Flg & EImGUIWindowFlags::NoMove ) Flags |= ImGuiWindowFlags_NoMove;
    if( Flg & EImGUIWindowFlags::NoCollapse ) Flags |= ImGuiWindowFlags_NoCollapse;

    ImGui::Begin( Caption.c_str(), nullptr, Flags );
}

void CImGUI::Begin(const std::string& Caption, const Vector2& Pos, const Vector2& Size)
{
    ImGui::SetNextWindowPos( {Pos.x, Pos.y} );
    ImGui::SetNextWindowSize( {Size.x, Size.y} );
    ImGui::Begin( Caption.c_str() );
}

void CImGUI::Begin(const std::string& Caption, const Vector2& Pos, const Vector2& Size, const EImGUIWindowFlags Flg)
{
    ImGuiWindowFlags Flags = ImGuiWindowFlags_None;
    if( Flg & EImGUIWindowFlags::NoTitleBar ) Flags |= ImGuiWindowFlags_NoTitleBar;
    if( Flg & EImGUIWindowFlags::NoResize ) Flags |= ImGuiWindowFlags_NoResize;
    if( Flg & EImGUIWindowFlags::NoMove ) Flags |= ImGuiWindowFlags_NoMove;
    if( Flg & EImGUIWindowFlags::NoCollapse ) Flags |= ImGuiWindowFlags_NoCollapse;

    ImGui::SetNextWindowPos( {Pos.x, Pos.y} );
    ImGui::SetNextWindowSize( {Size.x, Size.y} );
    ImGui::Begin( Caption.c_str(), nullptr, Flags );
}

void CImGUI::End()
{
    ImGui::End();
}

void CImGUI::Combo(const std::string& Caption, int& Selection, const std::vector<std::string>& Options)
{
    std::vector<const char*> Items;
    for(const auto& i: Options)
    {
        Items.push_back( i.c_str() );
    }
    ImGui::Combo( Caption.c_str(), &Selection, Items.data(), static_cast<int>(Options.size()) );
}

void CImGUI::CheckBox(const std::string& Caption, bool& Value)
{
    ImGui::Checkbox( Caption.c_str(), &Value );
}

void CImGUI::Text(const std::string& Caption)
{
    ImGui::Text( Caption.c_str() );
}

void CImGUI::TextColor(const std::string& Caption, const Color& Col)
{
    ImGui::TextColored( ImColor(Col.GetR(), Col.GetG(), Col.GetB(), Col.GetA() ), Caption.c_str() );
}

void CImGUI::InputText(const std::string& Caption, std::string& Value)
{
    ImGui::InputText( Caption.c_str(), &Value );
}

void CImGUI::InputFloat(const std::string& Caption, float& Value)
{
    ImGui::InputFloat( Caption.c_str(), &Value );
}

void CImGUI::InputVector2(const std::string& Caption, Vector2& Value)
{
    ImGui::InputFloat2( Caption.c_str(), &Value.x );
}

void CImGUI::InputVector3(const std::string& Caption, Vector3& Value)
{
    ImGui::InputFloat3( Caption.c_str(), &Value.x );
}

void CImGUI::InputVector4(const std::string& Caption, Vector4& Value)
{
    ImGui::InputFloat4( Caption.c_str(), &Value.x );
}

void CImGUI::InputRect2(const std::string& Caption, Rect2& Value)
{
    std::array<float, 4> Tmp = {Value.GetX(), Value.GetY(), Value.GetWidth(), Value.GetHeight()};
    ImGui::InputFloat4( Caption.c_str(), Tmp.data() );
    Value.SetX( Tmp[0] );
    Value.SetY( Tmp[1] );
    Value.SetWidth( Tmp[2] );
    Value.SetHeight( Tmp[3] );
}

bool CImGUI::Button(const std::string& Caption)
{
    return ImGui::Button( Caption.c_str() );
}

void CImGUI::SameLine()
{
    ImGui::SameLine();
}

bool CImGUI::Header(const std::string& Name)
{
    return ImGui::CollapsingHeader( Name.c_str() );
}

bool CImGUI::BeginItemContextMenu(const std::string& ID)
{
    return ImGui::BeginPopupContextItem( ID.empty() ? nullptr : ID.c_str() );
}

bool CImGUI::BeginWindowContextMenu(const std::string& ID)
{
    return ImGui::BeginPopupContextWindow( ID.c_str(), 1, false );
}

void CImGUI::OpenContextMenu(const std::string& ID)
{
    ImGui::OpenPopupOnItemClick( ID.empty() ? nullptr : ID.c_str(), 1 );
}

void CImGUI::EndContextMenu()
{
    ImGui::EndPopup();
}

void CImGUI::OpenPopup(const std::string& Name)
{
    ImGui::OpenPopup( Name.c_str() );
}

void CImGUI::ClosePopup()
{
    ImGui::CloseCurrentPopup();
}

bool CImGUI::BeginPopup(const std::string& Name)
{
    return ImGui::BeginPopupModal( Name.c_str(), nullptr, ImGuiWindowFlags_AlwaysAutoResize );
}

void CImGUI::EndPopup()
{
    ImGui::EndPopup();
}

bool CImGUI::BeginMenu(const std::string& Caption)
{
    return ImGui::BeginMenu( Caption.c_str() );
}

bool CImGUI::BeginMenu(const std::string& Caption, const bool Enabled)
{
    return ImGui::BeginMenu( Caption.c_str(), Enabled );
}

bool CImGUI::MenuItem(const std::string& Caption)
{
    return ImGui::MenuItem( Caption.c_str() );
}

bool CImGUI::MenuItem(const std::string& Caption, const bool Enabled)
{
    return ImGui::MenuItem( Caption.c_str(), nullptr, false, Enabled );
}

void CImGUI::Separator()
{
    ImGui::Separator();
}

void CImGUI::EndMenu()
{
    ImGui::EndMenu();
}

bool CImGUI::BeginMainMenuBar()
{
    return ImGui::BeginMainMenuBar();
}

void CImGUI::EndMainMenuBar()
{
    ImGui::EndMainMenuBar();
}

void CImGUI::ShowDemoWindow()
{
    ImGui::ShowDemoWindow();
}
