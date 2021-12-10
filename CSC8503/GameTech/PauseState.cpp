#include "PauseState.h"
#include "TutorialGame.h"
#include "GameUI.h"

using namespace NCL;
using namespace CSC8503;


PushdownState::PushdownResult InGameState::OnUpdate(float dt, PushdownState** newState)
{
	if (Window::GetKeyboard()->KeyDown(KeyboardKeys::ESCAPE) || game->IsFreezed())
	{
		*newState = new PauseState(this);
		return PushdownResult::Push;
	}
	if (quit)
	{
		return PushdownResult::Pop;
	}
	return PushdownResult::NoChange;
}

void InGameState::OnAwake()
{
	//printf("Enter InGameState.\n");
}

void InGameState::OnSleep()
{
	game->SetFreeze(true);
}

PushdownState::PushdownResult PauseState::OnUpdate(float dt, PushdownState** newState)
{
	if (pauseMenu->menuClose)
	{
		return PushdownResult::Pop;
	}
	else if(pauseMenu->quitGame)
	{
		gameState->quit = true;
		return PushdownResult::Pop;
	}
	else if (pauseMenu->ballLevel)
	{
		gameState->game->SetBallLevel();
		return PushdownResult::Pop;
	}
	else if (pauseMenu->mazeLevel)
	{
		gameState->game->SetMazeLevel();
		return PushdownResult::Pop;
	}

	return PushdownResult::NoChange;
}

void PauseState::OnAwake()
{
	gameState->game->GetUI()->PushMenu(pauseMenu);
}

void PauseState::OnSleep()
{
	gameState->game->GetUI()->RemoveMenu(pauseMenu);
	gameState->game->SetFreeze(false);
}
