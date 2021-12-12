#pragma once	
#include "GameMenu.h"
#include <string>

namespace NCL {
	namespace CSC8503 {
		class BallLevelMenu : public GameMenu
		{
		public:
			BallLevelMenu()
			{
				reachGoal = false;
				timeCount = 0.0f;
				score = 1000;
				statusStr = "Start!";
			
			};
			virtual ~BallLevelMenu() {};
			virtual void Draw() override;
			void SetReachGoal(bool goal)
			{
				reachGoal = goal;
			};
			void SetStatusString(const std::string& str)
			{
				statusStr = str;
			};
			void Update(float dt);
			
		protected:
			bool reachGoal;
			float timeCount;
			int score;
			std::string statusStr;

		};

	}
}



