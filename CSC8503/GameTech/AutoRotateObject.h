#pragma once
#include "../CSC8503Common/GameObject.h"
#include "../CSC8503Common/State.h"
#include "../CSC8503Common/StateTransition.h"
#include "../CSC8503Common/StateMachine.h"

namespace NCL {
	namespace CSC8503 {
		class AutoRotateObject : public GameObject
		{
		public:
			AutoRotateObject(const std::string& name)
				: GameObject(name)
			{
				speed = 1.0f;
				dir = 1.0f;
				counter = 0.0f;

				State* stateA = new State([&](float dt)->void
				{
					SpeedUp(dt);
				}
				);

				State* stateB = new State([&](float dt)->void
				{
					SlowDown(dt);
				}
				);

				StateTransition* transitionAB = new StateTransition(stateA, stateB, [&](void)->bool
				{
					return this->counter > 10.0f;
				}
				);

				StateTransition* transitionBA = new StateTransition(stateB, stateA, [&](void)->bool
				{
					return this->counter < 0.0f;
				}
				);


				stateMachine.AddState(stateA);
				stateMachine.AddState(stateB);
				stateMachine.AddTransition(transitionAB);
				stateMachine.AddTransition(transitionBA);
			};
			virtual ~AutoRotateObject() {};

			virtual void Update(float dt)
			{
				Quaternion rotation = Quaternion::EulerAnglesToQuaternion(0, dt * speed, 0) * transform.GetOrientation();
				transform.SetOrientation(rotation);
				stateMachine.Update(dt);
			};

			void SetRotateSpeed(float rotateSpeed)
			{
				speed = rotateSpeed;
				dir = speed >= 0 ? 1.0f : -1.0f;
			};

			void SpeedUp(float dt)
			{
				speed += 60 * dt * dir;
				counter += dt;
			}

			void SlowDown(float dt)
			{
				speed -= 60 * dt * dir;
				counter -= dt;
			}

		protected:
			float speed;
			float dir;
			float counter;
			StateMachine stateMachine;
		};
	}
}

