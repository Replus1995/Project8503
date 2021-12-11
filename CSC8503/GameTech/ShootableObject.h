#pragma once
#include "../CSC8503Common/GameObject.h"

namespace NCL {
	namespace CSC8503
	{
		class ShootableObject : public GameObject
		{
		public:
			ShootableObject(const std::string& name)
				: GameObject(name)
			{};
			virtual ~ShootableObject() {};

			void SetShootDir(const Vector3& dir) { shootDir = dir; };
			void TriggerShoot(float force) { GetPhysicsObject()->AddForce(shootDir * force); };

		protected:
			Vector3 shootDir = Vector3(0, 0, -1);
		};
	}
}