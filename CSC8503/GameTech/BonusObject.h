#pragma once
#include "TriggerObject.h"

namespace NCL {
	namespace CSC8503 {
		class BonusObject : public TriggerObject
		{
		public:
			BonusObject(const std::string& name)
				: TriggerObject(name)
			{};
			virtual ~BonusObject() {};

			virtual void Update(float dt)
			{
				if (autoRotate)
				{
					Quaternion rotation = Quaternion::EulerAnglesToQuaternion(0, dt * 360, 0) * transform.GetOrientation();
					transform.SetOrientation(rotation);
				}
			};

			void SetActive(bool active) { isActive = active; };

			size_t mazeNodeId = 0;
			bool autoRotate = false;
		};
	}
}
