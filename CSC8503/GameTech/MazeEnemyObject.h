#pragma once
#include "../CSC8503Common/GameObject.h"
#include "../CSC8503Common/NavigationPath.h"
#include "MazeMap.h"
#include "MazePlayerObject.h"
#include "BonusObject.h"


class BehaviourParallel;

namespace NCL {
	namespace CSC8503 {
		class MazeEnemyObject : public GameObject
		{
		public:
			MazeEnemyObject(const std::string& name, MazeMap* map, MazePlayerObject* player);
			virtual ~MazeEnemyObject();
			virtual void Update(float dt);

			virtual void OnCollisionBegin(GameObject* otherObject);

			void SetPowerUps(BonusObject* object_01, BonusObject* object_02)
			{
				powerUp_01 = object_01;
				powerUp_02 = object_02;
			}

			void RandomSpawn();
			void SetFreeze(float time);

		protected:
			MazeMap* mazeMap;
			MazePlayerObject* mazePlayer;
			BonusObject* powerUp_01 = nullptr;
			BonusObject* powerUp_02 = nullptr;

			Vector3 destPowerUpPos;
			NavigationPath pathToPowerUp;
			Vector3 puNextPos;
			BehaviourParallel* puFindBehaviour;

			float FreezeCount;
		};
	}
}
