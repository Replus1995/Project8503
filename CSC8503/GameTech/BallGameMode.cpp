#include "BallGameMode.h"

using namespace NCL;
using namespace CSC8503;

BallGameMode::~BallGameMode()
{
}

void BallGameMode::SetupScene()
{
	//Add floor
	gameInst->AddFloorToWorld(Vector3(0, -2, 0), Vector3(100,2,100), Vector4(1,1,1,0), "DefaultFloor")->GetPhysicsObject()->UpdatePhysicsScales(0.0,0.0,1);
	//Add air wall
	//gameInst->AddAirWallToWorld(Vector3(0, 12, 0), Vector3(100, 2, 100), "AirWall_Top");


	//Add wall
	gameInst->AddWallToWorld(Vector3(100, 5, 0), Vector3(2, 5, 100), Vector4(0.8f, 0.8f, 0.8f, 1), "Wall_0");


	gameInst->AddSphereToWorld(Vector3(0, 20, 0), 1.0f, 10.0f, "TestBall")->GetPhysicsObject()->AddPhysicsChannel(PhysCh_RayCast);


}

void BallGameMode::Update(float dt)
{

}
