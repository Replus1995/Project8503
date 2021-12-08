#pragma once
#include "TutorialGame.h"

namespace NCL {
	namespace CSC8503 {
		class GameMode
		{
		public:
			GameMode(TutorialGame* game)
				: gameInst(game)
			{};
			virtual ~GameMode() {};

			virtual void SetupScene() = 0;
			virtual void Update(float dt) = 0;
			
		protected:
			TutorialGame* gameInst;
		};
	}
}