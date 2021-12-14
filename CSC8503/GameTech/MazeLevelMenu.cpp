#include "MazeLevelMenu.h"

using namespace NCL;
using namespace CSC8503;

void MazeLevelMenu::Draw()
{
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings;
    const ImGuiViewport* main_viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(ImVec2(main_viewport->WorkPos.x, main_viewport->WorkPos.y + 280), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(360, 200), ImGuiCond_Always);

    if (!ImGui::Begin("Ball Level Menu", NULL, window_flags))
    {
        // Early out if the window is collapsed, as an optimization.
        ImGui::End();
        return;
    }
    ImGui::PushItemWidth(ImGui::GetFontSize() * -12);

    ImGui::Text("Game Info:");
    std::string TimeStr = "Time used: " + std::to_string(timeCount);
    ImGui::BulletText(TimeStr.c_str());
    std::string ScoreStr = "Score: " + std::to_string(score);
    ImGui::BulletText(ScoreStr.c_str());
    ImGui::BulletText(statusStr.c_str());
    ImGui::Separator();
    ImGui::Text("Control Guide:");
    ImGui::BulletText("Use Up|Down|Left|Right to control the player ball");
    ImGui::BulletText("Yellow coin = 50pts");
    ImGui::BulletText("Blue capsule = 20pts & freeze enemy for 3 seconds & power up player");
    ImGui::Separator();


    ImGui::End();
}

void MazeLevelMenu::Update(float dt)
{
	timeCount += dt;
}
