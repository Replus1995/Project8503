#pragma once
#include "GameMode.h"
#include "MazeLevelMenu.h"

namespace NCL {
	namespace CSC8503 {
		class MazeMap;
		class BonusObject;
		class MazePlayerObject;
		class MazeEnemyObject;
		class MazeGameMode : public GameMode
		{
		public:
			MazeGameMode(TutorialGame* game);
			virtual ~MazeGameMode();

			virtual void SetupScene() override;
			virtual void Update(float dt) override;

			MazePlayerObject* AddPlayer();
			MazeEnemyObject* AddEnemy();

			BonusObject* AddCoinObject();
			BonusObject* AddPowerUpObject(const std::string& name);

			void ReplaceBonusObject(BonusObject* object);

			GameObject* AddRotateGate(const Vector3& position, Vector3 dimensions, const std::string& name);

		protected:
			GameObject* mainBall = nullptr;
			MazeMap* map = nullptr;

			
			MazePlayerObject* player = nullptr;
			MazeEnemyObject* enemy = nullptr;
			BonusObject* coin = nullptr;
			BonusObject* powerUp_01 = nullptr;
			BonusObject* powerUp_02 = nullptr;
			int score = 0;

			std::shared_ptr<MazeLevelMenu> menu = nullptr;
		};
	}
}

