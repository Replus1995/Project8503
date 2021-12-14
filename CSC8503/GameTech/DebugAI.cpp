#include "DebugAI.h"

#include <iostream>

#include "../CSC8503Common/Debug.h"
#include "../CSC8503Common/State.h"
#include "../CSC8503Common/StateTransition.h"
#include "../CSC8503Common/StateMachine.h"
#include "../CSC8503Common/NavigationGrid.h"

#include "../CSC8503Common/BehaviourAction.h"
#include "../CSC8503Common/BehaviourSelector.h"
#include "../CSC8503Common/BehaviourSequence.h"

#include "TestPushdown.h"

using namespace NCL;
using namespace CSC8503;

void DebugAI::TestStateMachine()
{
	StateMachine* testMachine = new StateMachine();
	int data = 0;

	State* A = new State([&](float dt)->void
	{
		std::cout << "I'm in state A!\n";
		data++;
	}
	);

	State* B = new State([&](float dt)->void
	{
		std::cout << "I'm in state B!\n";
		data--;
	}
	);

	StateTransition* stateAB = new StateTransition(A, B, [&](void)->bool
	{
		return data > 10;
	}
	);

	StateTransition* stateBA = new StateTransition(B, A, [&](void)->bool
	{
		return data < 0;
	}
	);

	testMachine->AddState(A);
	testMachine->AddState(B);
	testMachine->AddTransition(stateAB);
	testMachine->AddTransition(stateBA);

	for (int i = 0; i < 100; i++)
	{
		testMachine->Update(1.0f);
	}

	delete testMachine;
}

void DebugAI::TestPathFinding(std::vector<NCL::Maths::Vector3>& outNodes)
{
	NavigationGrid grid("TestGrid1.txt");
	NavigationPath outPath;

	Vector3 startPos(80, 0, 10);
	Vector3 endPos(80, 0, 80);

	bool found = grid.FindPath(startPos, endPos, outPath);

	Vector3 pos;
	while (outPath.PopWaypoint(pos))
	{
		outNodes.push_back(pos);
	}
}

void DebugAI::DisplayPathFinding(const std::vector<NCL::Maths::Vector3>& inNodes, const NCL::Maths::Vector4& colour)
{
	for (size_t i = 1; i < inNodes.size(); i++)
	{
		const Vector3& a = inNodes[i - 1];
		const Vector3& b = inNodes[i];

		Debug::DrawLine(a, b, colour);
	}
}

void DebugAI::TestAndDisplayPathFinding()
{
	std::vector<Vector3> testNodes;
	TestPathFinding(testNodes);
	DisplayPathFinding(testNodes, Vector4(0, 1, 0, 1));
}

void DebugAI::TestBehaviourTree()
{
	float behaviourTimer = 0.0f;
	float distanceToTarget = 0.0f;
	BehaviourAction* findKey = new BehaviourAction("Find Key", [&](float dt, BehaviourState state)->BehaviourState {
		if (state == Initialise)
		{
			std::cout << "Looking for a key!\n";
			behaviourTimer = rand() % 100;
			state = Ongoing;
		}
		else if (state == Ongoing)
		{
			behaviourTimer -= dt;
			if (behaviourTimer <= 0.0f)
			{
				std::cout << "Found a key!\n";
				return Success;
			}
		}
		return state;
	});
		
	BehaviourAction* goToRoom = new BehaviourAction("Go To Room", [&](float dt, BehaviourState state)->BehaviourState {
		if (state == Initialise)
		{
			std::cout << "Going to the loot room!\n";
			state = Ongoing;
		}
		else if (state == Ongoing)
		{
			distanceToTarget -= dt;
			if (distanceToTarget <= 0.0f)
			{
				std::cout << "Reached room!\n";
				return Success;
			}
		}
		return state;
	});

	BehaviourAction* openDoor = new BehaviourAction("Open Door", [&](float dt, BehaviourState state)->BehaviourState {
		if (state == Initialise)
		{
			std::cout << "Opening door!\n";
			return Success;
		}
		return state;
	});

	BehaviourAction* LookForTreasure = new BehaviourAction("Look For Treasure", [&](float dt, BehaviourState state)->BehaviourState {
		if (state == Initialise)
		{
			std::cout << "Looking for treasure!\n";
			state = Ongoing;
		}
		else if (state == Ongoing)
		{
			bool found = rand() % 2;
			if (found)
			{
				std::cout << "I found some treasure!\n";
				return Success;
			}
			std::cout << "No treasure in here...\n";
			return Failure;
		}
		return state;
	});

	BehaviourAction* LookForItems = new BehaviourAction("Look For Items", [&](float dt, BehaviourState state)->BehaviourState {
		if (state == Initialise)
		{
			std::cout << "Looking for items!\n";
			state = Ongoing;
		}
		else if (state == Ongoing)
		{
			bool found = rand() % 2;
			if (found)
			{
				std::cout << "I found some items!\n";
				return Success;
			}
			std::cout << "No items in here...\n";
			return Failure;
		}
		return state;
	});

	BehaviourSequence* sequence = new BehaviourSequence("Room Sequence");
	sequence->AddChild(findKey);
	sequence->AddChild(goToRoom);
	sequence->AddChild(openDoor);

	BehaviourSelector* selection = new BehaviourSelector("Loot Selection");
	selection->AddChild(LookForTreasure);
	selection->AddChild(LookForItems);

	BehaviourSequence* rootSequence = new BehaviourSequence("Root Sequence");
	rootSequence->AddChild(sequence);
	rootSequence->AddChild(selection);

	for (int i = 0; i < 5; i++)
	{
		rootSequence->Reset();
		behaviourTimer = 0.0f;
		distanceToTarget = rand() % 250;
		BehaviourState state = Ongoing;
		std::cout << "We're going on an adventure!\n";
		while (state == Ongoing)
		{
			state = rootSequence->Execute(1.0f);
		}
		if (state == Success)
		{
			std::cout << "What a successful adventure!\n";
		}
		else if (state == Failure)
		{
			std::cout << "What a waste of time!\n";
		}
	}
	std::cout << "All done!\n";

	delete rootSequence;
}

void DebugAI::TestPushdownAutomata(NCL::Window* w)
{
	PushdownMachine machine(new IntroScreen());
	while (w->UpdateWindow())
	{
		float dt = w->GetTimer()->GetTimeDeltaSeconds();
		if (!machine.Update(dt))
		{
			return;
		}
	}
}
