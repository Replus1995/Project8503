#pragma once
#include "TriggerObject.h"

namespace NCL {
	namespace CSC8503 {
		class CheckPointObject : public TriggerObject
		{
		public:
			CheckPointObject(const std::string& name)
				: TriggerObject(name)
			{};
			virtual ~CheckPointObject() {};
		};
	}
}

