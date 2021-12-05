#include "StateGameObject.h"
#include "State.h"
#include "StateTransition.h"
#include "StateMachine.h"

using namespace NCL;
using namespace CSC8503;

StateGameObject::StateGameObject(const string& name)
	: GameObject(name)
{
	counter = 0.0f;
	stateMachine = new StateMachine();

	State* stateA = new State([&](float dt)->void
	{
		this->MoveLeft(dt);
	}
	);

	State* stateB = new State([&](float dt)->void
	{
		this->MoveRight(dt);
	}
	);

	StateTransition* transitionAB = new StateTransition(stateA, stateB, [&](void)->bool
	{
		return this->counter > 3.0f;
	}
	);

	StateTransition* transitionBA = new StateTransition(stateB, stateA, [&](void)->bool
	{
		return this->counter < 0.0f;
	}
	);


	stateMachine->AddState(stateA);
	stateMachine->AddState(stateB);
	stateMachine->AddTransition(transitionAB);
	stateMachine->AddTransition(transitionBA);
}

StateGameObject::~StateGameObject()
{
	delete stateMachine;
}

void StateGameObject::Update(float dt)
{
	stateMachine->Update(dt);
}

void StateGameObject::MoveLeft(float dt)
{
	GetPhysicsObject()->AddForce({ -20,0,0 });
	counter += dt;
}

void StateGameObject::MoveRight(float dt)
{
	GetPhysicsObject()->AddForce({ 20,0,0 });
	counter -= dt;
}
