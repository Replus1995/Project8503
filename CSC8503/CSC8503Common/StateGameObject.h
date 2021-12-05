#pragma once
#include "GameObject.h"

namespace NCL {
	namespace CSC8503 {
		class StateMachine;
		class StateGameObject : public GameObject
		{
		public:
			StateGameObject(const string& name = "");
			virtual ~StateGameObject();

			virtual void Update(float dt);

		protected:
			void MoveLeft(float dt);
			void MoveRight(float dt);

			StateMachine* stateMachine;
			float counter;
		};
	}
}