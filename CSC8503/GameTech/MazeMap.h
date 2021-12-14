#pragma once
#include <string>
#include "../../Common/Vector3.h"
#include "../CSC8503Common/NavigationGrid.h"

namespace NCL {
	using namespace Maths;
	namespace CSC8503 {
		class MazeMap : public NavigationGrid
		{
		public:
			MazeMap(const std::string& gridFile);
			~MazeMap();

			bool FindNode(const Vector3& pos, GridNode& node);
			size_t FindRandomPos(Vector3& outPos);
			void SetPosAvailable(size_t nodeId);

			int GetHalfSize() { return nodeSize / 2; };

		protected:
			std::vector<size_t> nodeIds;
		};
	}
}