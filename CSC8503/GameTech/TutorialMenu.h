#pragma once	
#include "GameMenu.h"

namespace NCL {
	namespace CSC8503 {
		class TutorialGame;
		class TutorialMenu : public GameMenu
		{
		public:
			TutorialMenu(TutorialGame* inGame)
				: game(inGame)
			{};
			virtual ~TutorialMenu() {};

			virtual void Draw() const override;
		protected:
			TutorialGame* game;
		};

	}
}



