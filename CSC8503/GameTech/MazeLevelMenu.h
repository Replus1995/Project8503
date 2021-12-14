#pragma once
#include "GameMenu.h"
#include <string>

namespace NCL {
	namespace CSC8503 {
		class MazeLevelMenu : public GameMenu
		{
		public:
			MazeLevelMenu(int& inScore)
				: score(inScore)
			{
				timeCount = 0.0f;
				statusStr = "Start!";

			};
			virtual ~MazeLevelMenu() {};
			virtual void Draw() override;
			void SetStatusString(const std::string& str)
			{
				statusStr = str;
			};
			void Update(float dt);

		protected:
			float timeCount;
			int& score;
			std::string statusStr;

		};
	}
}
