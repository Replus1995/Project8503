#pragma once
#include "../../Common/Window.h"
#include "../CSC8503Common/PushdownState.h"
#include "../CSC8503Common/PushdownMachine.h"

using namespace NCL;
using namespace CSC8503;


class PauseScreen : public PushdownState
{
public:
	PushdownResult OnUpdate(float dt, PushdownState** newState) override
	{
		if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::U))
		{
			return PushdownResult::Pop;
		}
		return PushdownResult::NoChange;
	};
	void OnAwake() override
	{
		std::cout << "Press U to unpause game!\n";
	};
};

class GameScreen : public PushdownState
{
public:
	PushdownResult OnUpdate(float dt, PushdownState** newState) override
	{
		pauseReminder -= dt;
		if (pauseReminder < 0)
		{
			std::cout << "Coins mined: " << coinsMined << "\n";
			std::cout << "Press P to pause game, or F1 to return to main menu!\n";
			pauseReminder += 1.0f;
		}
		if (Window::GetKeyboard()->KeyDown(KeyboardKeys::P))
		{
			*newState = new PauseScreen();
			return PushdownResult::Push;
		}
		if (Window::GetKeyboard()->KeyDown(KeyboardKeys::F1))
		{
			std::cout << "Returning to main menu!\n";
			return PushdownResult::Pop;
		}
		if (rand() % 7 == 0)
		{
			coinsMined++;
		}
		return PushdownResult::NoChange;
	};
	void OnAwake() override
	{
		std::cout << "Preparing to mine coins!\n";
	};

protected:
	int coinsMined = 0;
	float pauseReminder = 1;
};

class IntroScreen : public PushdownState
{
public:
	PushdownResult OnUpdate(float dt, PushdownState** newState) override
	{
		if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::SPACE))
		{
			*newState = new GameScreen();
			return PushdownResult::Push;
		}
		if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::ESCAPE))
		{
			return PushdownResult::Pop;
		}
		return PushdownResult::NoChange;
	};
	void OnAwake() override
	{
		std::cout << "Welcome to a really awesome game!\n";
		std::cout << "Press space to Begin ot escape to quit!\n";
	}
};