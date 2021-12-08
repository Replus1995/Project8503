#include "GameUI.h"
#include "../../Common/Win32Window.h"
#include "../../Common/Window.h"
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_opengl3.h"

using namespace NCL;

GameUI* GameUI::GamesUI_Instance = nullptr;

GameUI::GameUI()
{
    Win32Code::Win32Window* win32_w = dynamic_cast<Win32Code::Win32Window*>(Window::GetWindow());
    if (!win32_w) return;
    win32_w->SetExtraMsgFunc(GameUI::ProcessMsg);

    IsValid = true;

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.WantCaptureMouse = true;
    //io.ConfigFlags = ImGuiConfigFlags_NoMouseCursorChange;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();

    // Setup Platform/Renderer backends
    ImGui_ImplWin32_Init(win32_w->GetHandle());
    ImGui_ImplOpenGL3_Init("#version 400");

}

GameUI::~GameUI()
{
    if (IsValid)
    {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplWin32_Shutdown();
        ImGui::DestroyContext();
    }
    
    if (GamesUI_Instance == this) GamesUI_Instance = nullptr;
}

void GameUI::PushMenu(const GameMenuPtr& inMenu)
{
    menus.push_back(inMenu);
}

GameMenuPtr GameUI::PopMenu()
{
    GameMenuPtr menuToPop = menus.back();
    menus.pop_back();
    return menuToPop;
}

void GameUI::ClearMenus()
{
    menus.clear();
}

void GameUI::UpdateUI() const
{
    if (!IsValid) return;

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    for (const auto& menu : menus)
    {
        menu->Draw();
    }

    ImGui::Render();
}

void GameUI::DrawUI() const
{
    if (!IsValid) return;

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

GameUI* GameUI::GetInstance()
{
    return GamesUI_Instance;
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
void GameUI::ProcessMsg(void* inMsg)
{
    MSG& msg = *(MSG*)inMsg;
    ImGui_ImplWin32_WndProcHandler(msg.hwnd, msg.message, msg.wParam, msg.lParam);
}