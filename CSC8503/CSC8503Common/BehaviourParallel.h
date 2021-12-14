#pragma once
#include "BehaviourNodeWithChildren.h"

class BehaviourParallel : public BehaviourNodeWithChildren
{
public:
	BehaviourParallel(const std::string& nodeName)
		: BehaviourNodeWithChildren(nodeName)
	{};
	virtual ~BehaviourParallel() {};

	BehaviourState Execute(float dt) override
	{
		currentState = Success;
		for (auto& i : childNodes)
		{
			BehaviourState nodeState = i->Execute(dt);
			switch (nodeState)
			{
			case Success:
			case Failure:
				continue;
			case Ongoing:
				currentState = Ongoing;
			}
		}
		return currentState;
	};
};
