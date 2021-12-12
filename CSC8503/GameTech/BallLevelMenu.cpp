#include "BallLevelMenu.h"
#include <algorithm>

using namespace NCL;
using namespace CSC8503;

void BallLevelMenu::Draw()
{
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings;
    const ImGuiViewport* main_viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(ImVec2(main_viewport->WorkPos.x, main_viewport->WorkPos.y + 260), ImGuiCond_Always);
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
    ImGui::BulletText("Use mouse to select which board you want to control");
    ImGui::BulletText("Press E to push the ball");
    ImGui::BulletText("Press R to return to last check point");
    ImGui::Separator();


    ImGui::End();
}

void BallLevelMenu::Update(float dt)
{
	if (!reachGoal)
	{
		timeCount += dt;
	}
	score = std::max(int(1000 - floor(timeCount / 5)), 0);
}
