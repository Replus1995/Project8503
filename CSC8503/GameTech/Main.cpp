#include "../../Common/Window.h"
#include "../../Common/Win32Window.h"

#include "../CSC8503Common/StateMachine.h"
#include "../CSC8503Common/StateTransition.h"
#include "../CSC8503Common/State.h"

#include "../CSC8503Common/NavigationGrid.h"

#include "TutorialGame.h"
#include "GameUI.h"

#include "DebugAI.h"

#include "PauseMenu.h"

	#pragma comment( linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"" ) 

using namespace NCL;
using namespace CSC8503;

/*

The main function should look pretty familar to you!
We make a window, and then go into a while loop that repeatedly
runs our 'game' until we press escape. Instead of making a 'renderer'
and updating it, we instead make a whole game, and repeatedly update that,
instead. 

This time, we've added some extra functionality to the window class - we can
hide or show the 

*/

//#define SimpleTestAI

int main() {

	srand(time(0));

#ifdef SimpleTestAI
	{
		//DebugAI::TestStateMachine();
		DebugAI::TestBehaviourTree();
		system("Pause");
		return 0;
	}
#endif //  SimpleTestAI

	Window*w = Window::CreateGameWindow("GameTech", 1920, 1080, false);
	if (!w->HasInitialised()) {
		return -1;
	}

	//DebugAI::TestPushdownAutomata(w);

	w->ShowOSPointer(true);
	w->LockMouseToWindow(true);

	TutorialGame* g = new TutorialGame();

	w->GetTimer()->GetTimeDeltaSeconds(); //Clear the timer so we don't get a larget first dt!
	while (w->UpdateWindow() && !g->ShouldQuit()) {
		float dt = w->GetTimer()->GetTimeDeltaSeconds();
		if (dt > 0.1f) {
			std::cout << "Skipping large time delta" << std::endl;
			continue; //must have hit a breakpoint or something to have a 1 second frame time!
		}

		/*if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::T)) {
			w->SetWindowPosition(0, 0);
		}*/

		//w->SetTitle("Gametech frame time:" + std::to_string(1000.0f * dt));
		g->UpdateGame(dt);
		g->UpdateRender(dt);
		//DebugAI::TestAndDisplayPathFinding(); //Test navigation
	}
	delete g;
	Window::DestroyGameWindow();
}