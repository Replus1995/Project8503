#pragma once
#include "../CSC8503Common/GameObject.h"
#include "CheckPointObject.h"

namespace NCL {
	namespace CSC8503 {
		class MainBallObject : public GameObject
		{
		public:
			MainBallObject(const std::string& name)
				: GameObject(name)
			{
				restorePoint = Vector3();
			};
			virtual ~MainBallObject() {};

			void SetRestorePoint(const Vector3& point)
			{
				restorePoint = point;
			};
			void RestorePosition()
			{
				transform.SetPosition(restorePoint).SetOrientation(Quaternion());
				physicsObject->SetAngularVelocity(Vector3());
				physicsObject->SetLinearVelocity(Vector3());
			}

			virtual void OnCollisionBegin(GameObject* otherObject) {
				CheckPointObject* cPoint = dynamic_cast<CheckPointObject*>(otherObject);
				if (cPoint)
				{
					restorePoint = cPoint->GetTransform().GetPosition();
				}
			}

		protected:
			Vector3 restorePoint;
		};
	}
}

