#pragma once
#include "GameMode.h"

namespace NCL {
	namespace CSC8503 {
		class ShootableObject;

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
			ShootableObject* AddShootableCube(const Vector3& position, const Vector3& dimensions, float inverseMass, const Vector3& shootDir, const std::string& name = "");
			void AddShootBoard(const Vector3& position, const Vector3& dimensions, float inverseMass, const Vector3& shootDir, float min, float max, const std::string& name = "");
			GameObject* AddAutoRotateCapsule(const Vector3& position, float halfHeight, float radius, float inverseMass, float rotateSpeed, const std::string& name = "");
		protected:
			GameObject* mainBall = nullptr;
		};
	}
}
