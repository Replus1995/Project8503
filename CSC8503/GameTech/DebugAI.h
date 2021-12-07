#pragma once
#include <vector>
#include "../../Common/Window.h"
#include "../../Common/Vector3.h"


class DebugAI
{
public:
	static void TestStateMachine();
	
	static void TestPathFinding(std::vector<NCL::Maths::Vector3>& outNodes);
	static void DisplayPathFinding(const std::vector<NCL::Maths::Vector3>& inNodes);
	static void TestAndDisplayPathFinding();

	static void TestBehaviourTree();

	static void TestPushdownAutomata(NCL::Window* w);

};