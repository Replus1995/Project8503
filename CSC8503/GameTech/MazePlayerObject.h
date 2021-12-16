#pragma once
#include "../CSC8503Common/GameObject.h"
#include "../../Common/Window.h"

namespace NCL {
	namespace CSC8503 {
		class MazePlayerObject : public GameObject
		{
		public:
			MazePlayerObject(const std::string& name)
				: GameObject(name)
			{
				restorePoint = Vector3();
			};
			virtual ~MazePlayerObject() {};

			void SetRestorePoint(const Vector3& point) { restorePoint = point; };
			void RestorePosition()
			{
				transform.SetPosition(restorePoint).SetOrientation(Quaternion());
				physicsObject->SetAngularVelocity(Vector3());
				physicsObject->SetLinearVelocity(Vector3());
			}
			void SetMoveForce(float force) { moveForce = force; };
			void SetPowerUpCount(float count) { powerUpCount = count; };

			virtual void Update(float dt) 
			{
				if (powerUpCount > 0.0f)
				{
					powerUpCount -= dt;
					if (powerUpCount <= 0.0f)
					{
						moveForce = 100.0f;
					}
				}

				if (Window::GetKeyboard()->KeyDown(KeyboardKeys::LEFT)) {
					physicsObject->AddForce(Vector3(-1,0,0) * moveForce);
				}

				if (Window::GetKeyboard()->KeyDown(KeyboardKeys::RIGHT)) {
					physicsObject->AddForce(Vector3(1, 0, 0) * moveForce);
				}

				if (Window::GetKeyboard()->KeyDown(KeyboardKeys::UP)) {
					physicsObject->AddForce(Vector3(0, 0, -1) * moveForce);
				}

				if (Window::GetKeyboard()->KeyDown(KeyboardKeys::DOWN)) {
					physicsObject->AddForce(Vector3(0, 0, 1) * moveForce);
				}

				if (Window::GetKeyboard()->KeyDown(KeyboardKeys::O)) {
					physicsObject->AddTorque(Vector3(0, -30, 0));
				}

				if (Window::GetKeyboard()->KeyDown(KeyboardKeys::P)) {
					physicsObject->AddTorque(Vector3(0, 30, 0));
				}
			};

		protected:
			Vector3 restorePoint;
			float moveForce = 100.0f;
			float powerUpCount = 0.0f;
		};
	}
}
