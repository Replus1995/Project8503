#include "MazeMap.h"
#include "TutorialGame.h"

using namespace NCL;
using namespace CSC8503;

MazeMap::MazeMap(const std::string& gridFile)
	: NavigationGrid(gridFile)
{

	//bonusObj->SetActive(false);
	for (int y = 0; y < gridHeight; ++y) {
		for (int x = 0; x < gridWidth; ++x) {
			GridNode& n = allNodes[(gridWidth * y) + x];
			if (n.type == '.')
			{
				nodeIds.push_back((gridWidth * y) + x);
			}
		}
	}

}

MazeMap::~MazeMap()
{
}

bool MazeMap::FindNode(const Vector3& pos, GridNode& node)
{
	int posX = ((int)(pos.x + gridWidth / 2 * nodeSize) / nodeSize);
	int posZ = ((int)(pos.z + gridHeight / 2 * nodeSize) / nodeSize);

	if (posX < 0 || posX > gridWidth - 1 ||
		posZ < 0 || posZ > gridHeight - 1) {
		return false; //outside of map region!
	}

	GridNode* posNode = &allNodes[(posZ * gridWidth) + posX];
	if (posNode->type == 'x')
	{
		return false;
	}
	node = *posNode;
	return true;
}

size_t MazeMap::FindRandomPos(Vector3& outPos)
{
	int arrayIndex = rand() % nodeIds.size();
	size_t nodeId = nodeIds[arrayIndex];
	nodeIds.erase(nodeIds.begin() + arrayIndex);
	outPos = allNodes[nodeId].position;
	return nodeId;
}

void MazeMap::SetPosAvailable(size_t nodeId)
{
	nodeIds.push_back(nodeId);
}
