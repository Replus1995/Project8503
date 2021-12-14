#pragma once
#include "../CSC8503Common/GameObject.h"
#include <functional>

namespace NCL {
	namespace CSC8503 {
		typedef std::function<void(GameObject*)> TriggerCallback;
		class TriggerObject : public GameObject
		{
		public:
			TriggerObject(const std::string& name)
				: GameObject(name)
			{};
			virtual ~TriggerObject() {};
			virtual void OnCollisionBegin(GameObject* otherObject) {
				isActive = false;
				for  (TriggerCallback& func : triggerFuncs)
				{
					func(otherObject);
				}
			}

			void AddTriggerCallback(TriggerCallback func)
			{
				triggerFuncs.push_back(func);
			}
			void ClearTriggerCallback()
			{
				triggerFuncs.clear();
			}

		protected:
			std::vector<TriggerCallback>  triggerFuncs;
		};
	}
}
