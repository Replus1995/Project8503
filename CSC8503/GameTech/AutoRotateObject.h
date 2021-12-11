#pragma once
#include "../CSC8503Common/GameObject.h"

namespace NCL {
	namespace CSC8503 {
		class AutoRotateObject : public GameObject
		{
		public:
			AutoRotateObject(const std::string& name)
				: GameObject(name)
			{
				speed = 1.0f;
			};
			virtual ~AutoRotateObject() {};

			virtual void Update(float dt)
			{
				Quaternion rotation = Quaternion::EulerAnglesToQuaternion(0, dt * speed, 0) * transform.GetOrientation();
				transform.SetOrientation(rotation);

			};

			void SetRotateSpeed(float rotateSpeed)
			{
				speed = rotateSpeed;
			};

		protected:
			float speed;
		};
	}
}

