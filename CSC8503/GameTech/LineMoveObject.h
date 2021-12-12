#pragma once
#include "../CSC8503Common/GameObject.h"

namespace NCL {
	namespace CSC8503 {
		class LineMoveObject : public GameObject
		{
		public:
			LineMoveObject(const std::string& name)
				: GameObject(name)
			{
				centerPoint = Vector3();
			};
			virtual ~LineMoveObject() {};

			virtual void Update(float dt)
			{
				Vector3 forceDir = centerPoint - transform.GetPosition();
				if (forceDir.Length() > 0)
				{
					forceDir.Normalise();
				
					Vector3 newVelocity = physicsObject->GetLinearVelocity() + forceDir * 2;
					physicsObject->SetLinearVelocity(newVelocity);

					transform.SetPosition(transform.GetPosition() + physicsObject->GetLinearVelocity() * dt);
				}
			};
			
			void SetCenterPoint(const Vector3& point)
			{
				centerPoint = point;
			}

		protected:
			Vector3 centerPoint;
		};
	}
}

