#include "BallGameMode.h"
#include "../CSC8503Common/SliderConstraint.h"
#include "ShootableObject.h"
#include "AutoRotateObject.h"

using namespace NCL;
using namespace CSC8503;

BallGameMode::~BallGameMode()
{
}

void BallGameMode::SetupScene()
{
	//Add floor
	//GameObject* floor = gameInst->AddFloorToWorld(Vector3(0, -2, 0), Vector3(100,2,100), Vector4(1,1,1,0), "DefaultFloor");
	//floor->GetPhysicsObject()->SetFriction(0.6);
	GameObject* floor_01 = gameInst->AddFloorToWorld(Vector3(80, -2, 0), Vector3(20, 2, 100), Vector4(1, 1, 1, 0), "Floor_01");
	floor_01->GetPhysicsObject()->SetFriction(0.6);
	floor_01->GetPhysicsObject()->SetElasticity(0.6);

	GameObject* floor_ice = gameInst->AddFloorToWorld(Vector3(37, -2, 0), Vector3(23, 2, 100), Vector4(.8, .8, 1, 0), "Floor_Ice");
	floor_ice->GetPhysicsObject()->SetFriction(0.05);
	floor_ice->GetPhysicsObject()->SetElasticity(0.2);

	GameObject* floor_sand = gameInst->AddFloorToWorld(Vector3(-8, -2, 0), Vector3(22, 2, 56), Vector4(.9, .7, .3, 0), "Floor_Sand");
	floor_ice->GetPhysicsObject()->SetFriction(10);
	floor_ice->GetPhysicsObject()->SetElasticity(0);
	GameObject* floor_02 = gameInst->AddFloorToWorld(Vector3(-8, -2, 78), Vector3(22, 2, 22), Vector4(1, 1, 1, 0), "Floor_02");
	floor_01->GetPhysicsObject()->SetFriction(0.6);
	floor_01->GetPhysicsObject()->SetElasticity(0.6);
	GameObject* floor_03 = gameInst->AddFloorToWorld(Vector3(-8, -2, -78), Vector3(22, 2, 22), Vector4(1, 1, 1, 0), "Floor_03");
	floor_01->GetPhysicsObject()->SetFriction(0.6);
	floor_01->GetPhysicsObject()->SetElasticity(0.6);

	GameObject* floor_04 = gameInst->AddFloorToWorld(Vector3(-65, -2, 0), Vector3(35, 2, 100), Vector4(1, 1, 1, 0), "Floor_04");
	floor_01->GetPhysicsObject()->SetFriction(0.6);
	floor_01->GetPhysicsObject()->SetElasticity(0.6);



	//Add air wall
	gameInst->AddAirWallToWorld(Vector3(0, 22, 0), Vector3(100, 2, 100), "AirWall_Top");


	//Add wall around
	gameInst->AddWallToWorld(Vector3(-100, 10, 0), Vector3(1, 10, 99), Vector4(0.8f, 0.8f, 0.8f, 1), "Wall_LM");
	gameInst->AddWallToWorld(Vector3(100, 10, 0), Vector3(1, 10, 99), Vector4(0.8f, 0.8f, 0.8f, 1), "Wall_RM");
	gameInst->AddWallToWorld(Vector3(0, 10, -100), Vector3(101, 10, 1), Vector4(0.8f, 0.8f, 0.8f, 1), "Wall_FM");
	gameInst->AddWallToWorld(Vector3(0, 10, 100), Vector3(101, 10, 1), Vector4(0.8f, 0.8f, 0.8f, 1), "Wall_BM");
	//Add column walls
	GameObject* wall_01 = gameInst->AddWallToWorld(Vector3(60, 10, 30), Vector3(1, 10, 70), Vector4(0.8f, 0.8f, 0.8f, 1), "Wall_01");
	GameObject* wall_angle_01 = gameInst->AddWallToWorld(Vector3(75, 10, -50), Vector3(1, 10, 35), Vector4(0.8f, 0.8f, 0.8f, 1), "Wall_Angle_01");
	wall_angle_01->GetTransform().SetOrientation(Quaternion::EulerAnglesToQuaternion(0, 45, 0));
	wall_angle_01->GetPhysicsObject()->SetElasticity(2);
	GameObject* wall_02 = gameInst->AddWallToWorld(Vector3(14, 10, -20), Vector3(1, 10, 80), Vector4(0.8f, 0.8f, 0.8f, 1), "Wall_02");

	GameObject* wall_angle_02 = gameInst->AddWallToWorld(Vector3(36, 10, 70), Vector3(1, 10, 31), Vector4(0.8f, 0.8f, 0.8f, 1), "Wall_Angle_02");
	wall_angle_02->GetTransform().SetOrientation(Quaternion::EulerAnglesToQuaternion(0, -50, 0));
	wall_angle_02->GetPhysicsObject()->SetElasticity(2);

	GameObject* wall_up_01 = gameInst->AddWallToWorld(Vector3(-8, 1, 50), Vector3(22, 1, 12), Vector4(0.3f, 0.3f, 0.3f, 1), "Wall_Up_01");
	wall_up_01->GetTransform().SetOrientation(Quaternion::EulerAnglesToQuaternion(10, 0, 0));
	wall_up_01->GetPhysicsObject()->SetElasticity(2);

	GameObject* wall_03 = gameInst->AddWallToWorld(Vector3(-30, 10, 20), Vector3(1, 10, 80), Vector4(0.8f, 0.8f, 0.8f, 1), "Wall_03");
	
	AddAutoRotateCapsule(Vector3(25, 4, -20), 6, 2, 0.0f, 120.0f, "Capsule_AR_01");
	AddAutoRotateCapsule(Vector3(45, 4, 0), 6, 2, 0.0f, -240.0f, "Capsule_AR_02");
	AddAutoRotateCapsule(Vector3(25, 4, 20), 6, 2, 0.0f, 180.0f, "Capsule_AR_03");
	
	
	//GameObject* up_01 = gameInst->AddWallToWorld(Vector3(32, 5, -40), Vector3(18, 1, 60), Vector4(0.3f, 0.3f, 0.3f, 1), "up_01");
	//up_01->GetTransform().SetOrientation(Quaternion::EulerAnglesToQuaternion(15, 0, 0));
	


	////Add slider board
	//GameObject* silder_0_A = gameInst->AddCubeToWorld(Vector3(80, 5, 90), Vector3(5, 5, 1), 0.0f);
	//GameObject* silder_0_B = AddShootableCube(Vector3(80, 5, 86), Vector3(18, 5, 1), 0.5f, Vector3(0,0,-1));
	//silder_0_B->GetPhysicsObject()->SetPhysicsChannel(PhysCh_RayCast | PhysCh_Static | PhysCh_Dynamic);
	//silder_0_B->GetPhysicsObject()->SetElasticity(1);
	//SliderConstraint* silder_0_con = new SliderConstraint(silder_0_A, silder_0_B, Vector3(0, 0, -1), 2, 32);
	//gameInst->world->AddConstraint(silder_0_con);

	//Add shoot board
	AddShootBoard(Vector3(80, 10, 90), Vector3(18, 10, 1), 0.5f, Vector3(0, 0, -1), 3, 30, "ShootBoard_01");
	AddShootBoard(Vector3(32, 10, -75), Vector3(18, 10, 1), 0.5f, Vector3(0, 0, 1), 3, 30, "ShootBoard_02");
	AddShootBoard(Vector3(-8, 10, 90), Vector3(20, 10, 1), 0.5f, Vector3(0, 0, -1), 3, 30, "ShootBoard_03");

	mainBall = gameInst->AddSphereToWorld(Vector3(80, 5, 70), 2.0f, 10.0f, "MainBall");
	mainBall->GetRenderObject()->SetColour(Vector4(1, 1, 0, 1));
	mainBall->GetPhysicsObject()->AddPhysicsChannel(PhysCh_RayCast);
	mainBall->GetPhysicsObject()->AddPhysicsChannel(PhysCh_AirWall);
	//mainBall->GetPhysicsObject()->SetElasticity(1);
	



}

void BallGameMode::Update(float dt)
{
	if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::E))
	{
		if (gameInst->selectionObject)
		{
			ShootableObject* sObj = dynamic_cast<ShootableObject*>(gameInst->selectionObject);
			if (sObj)
			{
				sObj->TriggerShoot(gameInst->forceMagnitude);
			}
		}
	}
}

ShootableObject* BallGameMode::AddShootableCube(const Vector3& position, const Vector3& dimensions, float inverseMass, const Vector3& shootDir, const std::string& name)
{
	ShootableObject* cube = new ShootableObject(name);

	AABBVolume* volume = new AABBVolume(dimensions);

	cube->SetBoundingVolume((CollisionVolume*)volume);

	cube->GetTransform()
		.SetPosition(position)
		.SetScale(dimensions * 2);

	cube->SetRenderObject(new RenderObject(&cube->GetTransform(), gameInst->cubeMesh, gameInst->basicTex, gameInst->basicShader));
	cube->SetPhysicsObject(new PhysicsObject(&cube->GetTransform(), cube->GetBoundingVolume()));
	cube->GetPhysicsObject()->SetInverseMass(inverseMass);
	cube->GetPhysicsObject()->InitCubeInertia();
	//cube->GetPhysicsObject()->SetPhysicsChannel(PhysCh_RayCast | PhysCh_Static | PhysCh_Dynamic);

	cube->SetShootDir(shootDir);

	gameInst->world->AddGameObject(cube);

	return cube;
}

void BallGameMode::AddShootBoard(const Vector3& position, const Vector3& dimensions, float inverseMass, const Vector3& shootDir, float min, float max, const std::string& name)
{
	GameObject* silder_0_A = gameInst->AddCubeToWorld(position + -shootDir * min, Vector3(5, 5, 1), 0.0f, name + "_A");
	silder_0_A->GetPhysicsObject()->AddPhysicsChannel(PhysCh_Static);
	GameObject* silder_0_B = AddShootableCube(position, dimensions, inverseMass, shootDir, name + "_B");
	silder_0_B->GetPhysicsObject()->SetPhysicsChannel(PhysCh_RayCast | PhysCh_Static | PhysCh_Dynamic);
	silder_0_B->GetPhysicsObject()->SetElasticity(1);
	SliderConstraint* silder_0_con = new SliderConstraint(silder_0_A, silder_0_B, shootDir, min, max);
	gameInst->world->AddConstraint(silder_0_con);
}

GameObject* BallGameMode::AddAutoRotateCapsule(const Vector3& position, float halfHeight, float radius, float inverseMass, float rotateSpeed, const std::string& name)
{
	AutoRotateObject* capsule = new AutoRotateObject(name);

	CapsuleVolume* volume = new CapsuleVolume(halfHeight, radius);
	capsule->SetBoundingVolume((CollisionVolume*)volume);

	capsule->GetTransform()
		.SetScale(Vector3(radius * 2, halfHeight, radius * 2))
		.SetPosition(position)
		.SetOrientation(Quaternion::EulerAnglesToQuaternion(0,0,90));

	capsule->SetRenderObject(new RenderObject(&capsule->GetTransform(), gameInst->capsuleMesh, gameInst->basicTex, gameInst->basicShader));
	capsule->SetPhysicsObject(new PhysicsObject(&capsule->GetTransform(), capsule->GetBoundingVolume()));

	capsule->GetPhysicsObject()->SetInverseMass(inverseMass);
	capsule->GetPhysicsObject()->InitCubeInertia();
	capsule->GetPhysicsObject()->SetPhysicsChannel(PhysCh_Static | PhysCh_Dynamic | PhysCh_RayCast);
	
	capsule->SetRotateSpeed(rotateSpeed);


	gameInst->world->AddGameObject(capsule);

	return capsule;
}
