#pragma once
#include "GameMenu.h"

namespace NCL {
	namespace CSC8503 {
		class PauseMenu : public GameMenu
		{
		public:
			PauseMenu(){};
			virtual ~PauseMenu() {};

			virtual void Draw() override;
		
			bool ballLevel = false;
			bool mazeLevel = false;
			bool menuClose = false;
			bool quitGame = false;
		};

	}
}



