#pragma once
#include "GameMode.h"

namespace NCL {
	namespace CSC8503 {
		class BallGameMode : public GameMode
		{
		public:
			BallGameMode(TutorialGame* game)
				: GameMode(game)
			{};
			virtual ~BallGameMode();

			virtual void SetupScene() override;
			virtual void Update(float dt) override;

		protected:
			GameObject* mainBall = nullptr;
		};
	}
}
