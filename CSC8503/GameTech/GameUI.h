#pragma once
#include <vector>
#include "GameMenu.h"

struct ExInputResult;

class GameUI
{
public:
	GameUI();
	~GameUI();

	void PushMenu(const GameMenuPtr& inMenu);
	GameMenuPtr PopMenu();
	void ClearMenus();

	void UpdateUI() const;
	void DrawUI() const;

	static GameUI* GetInstance();
private:
	bool IsValid = false;
	std::vector<GameMenuPtr> menus;
	static GameUI* GamesUI_Instance;
};