#pragma once
#include "GameMode.h"
#include "MainBallObject.h"
#include "BallLevelMenu.h"

namespace NCL {
	namespace CSC8503 {
		class ShootableObject;
		class TriggerObject;
		class CheckPointObject;

		class BallGameMode : public GameMode
		{
		public:
			BallGameMode(TutorialGame* game);
			virtual ~BallGameMode();

			virtual void SetupScene() override;
			virtual void Update(float dt) override;

		protected:
			MainBallObject* AddMainBall(const Vector3& position, float radius, float inverseMass, const std::string& name = "");
			ShootableObject* AddShootableCube(const Vector3& position, const Vector3& dimensions, float inverseMass, const Vector3& shootDir, const std::string& name = "");
			void AddShootBoard(const Vector3& position, const Vector3& dimensions, float inverseMass, const Vector3& shootDir, float min, float max, const std::string& name = "");
			GameObject* AddAutoRotateCapsule(const Vector3& position, float halfHeight, float radius, float inverseMass, float rotateSpeed, const std::string& name = "");
			CheckPointObject* AddCheckPoint(const Vector3& position, Vector3 dimensions, const std::string& name = "");
			GameObject* AddBonusLR(const Vector3& position, const Vector3& center, float scale, const std::string& name = "");
			void AddMixedGrid(const Vector3& center, int numRows, int numCols, float rowSpacing, float colSpacing);
			TriggerObject* AddTriggerVolume(const Vector3& position, const Vector3& dimensions, MeshGeometry* mesh = nullptr, const std::string& name = "");

		protected:
			MainBallObject* mainBall = nullptr;
			std::shared_ptr<BallLevelMenu> menu = nullptr;
		};
	}
}
