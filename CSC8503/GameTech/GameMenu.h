#pragma once
#include <memory>
#include "imgui.h"

class GameMenu
{
public:
	GameMenu() {};
	virtual ~GameMenu() {};

	virtual void Draw() const = 0;
};

typedef std::shared_ptr<GameMenu> GameMenuPtr;


class GameMenuDemo : public GameMenu
{
	virtual void Draw() const override
	{
		bool showDemo = true;
		ImGui::ShowDemoWindow(&showDemo);
	};
};