#include "BallGameMode.h"
#include "../CSC8503Common/SliderConstraint.h"
#include "ShootableObject.h"
#include "AutoRotateObject.h"
#include "LineMoveObject.h"
#include "TriggerObject.h"
#include "GameUI.h"

using namespace NCL;
using namespace CSC8503;

BallGameMode::BallGameMode(TutorialGame* game)
	: GameMode(game)
{
	menu.reset(new BallLevelMenu());
	gameInst->gameUI->PushMenu(menu);
}

BallGameMode::~BallGameMode()
{
	gameInst->gameUI->RemoveMenu(menu);
}

void BallGameMode::SetupScene()
{
	gameInst->camInitPos = Vector3(0, 100, 200);
	gameInst->camInitRot = Vector3(0, 0, -30);

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
	floor_sand->GetPhysicsObject()->SetFriction(1.0f);
	floor_sand->GetPhysicsObject()->SetElasticity(0);
	GameObject* floor_02 = gameInst->AddFloorToWorld(Vector3(-8, -2, 78), Vector3(22, 2, 22), Vector4(1, 1, 1, 0), "Floor_02");
	floor_02->GetPhysicsObject()->SetFriction(0.6);
	floor_02->GetPhysicsObject()->SetElasticity(0.6);
	GameObject* floor_03 = gameInst->AddFloorToWorld(Vector3(-8, -2, -78), Vector3(22, 2, 22), Vector4(1, 1, 1, 0), "Floor_03");
	floor_03->GetPhysicsObject()->SetFriction(0.6);
	floor_03->GetPhysicsObject()->SetElasticity(0.6);

	GameObject* floor_04 = gameInst->AddFloorToWorld(Vector3(-65, -2, 0), Vector3(35, 2, 100), Vector4(1, 1, 1, 0), "Floor_04");
	floor_04->GetPhysicsObject()->SetFriction(0.6);
	floor_04->GetPhysicsObject()->SetElasticity(0.6);

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

	GameObject* wall_02 = gameInst->AddWallToWorld(Vector3(14, 10, -22), Vector3(1, 10, 78), Vector4(0.8f, 0.8f, 0.8f, 1), "Wall_02");
	GameObject* wall_angle_02 = gameInst->AddWallToWorld(Vector3(36, 10, 70), Vector3(1, 10, 31), Vector4(0.8f, 0.8f, 0.8f, 1), "Wall_Angle_02");
	wall_angle_02->GetTransform().SetOrientation(Quaternion::EulerAnglesToQuaternion(0, -50, 0));
	wall_angle_02->GetPhysicsObject()->SetElasticity(2);

	GameObject* wall_03 = gameInst->AddWallToWorld(Vector3(-30, 10, 20), Vector3(1, 10, 80), Vector4(0.8f, 0.8f, 0.8f, 1), "Wall_03");
	GameObject* wall_up_01 = gameInst->AddWallToWorld(Vector3(-8, 1, 50), Vector3(22, 1, 12), Vector4(0.3f, 0.3f, 0.3f, 1), "Wall_Up_01");
	wall_up_01->GetTransform().SetOrientation(Quaternion::EulerAnglesToQuaternion(10, 0, 0));
	wall_up_01->GetPhysicsObject()->SetElasticity(0.6);
	wall_up_01->GetPhysicsObject()->SetFriction(0.3);

	GameObject* wall_up_02 = gameInst->AddWallToWorld(Vector3(-8, 1, -45), Vector3(22, 1, 12), Vector4(0.3f, 0.3f, 0.3f, 1), "Wall_Up_02");
	wall_up_02->GetTransform().SetOrientation(Quaternion::EulerAnglesToQuaternion(-10, 0, 0));
	wall_up_02->GetPhysicsObject()->SetElasticity(0.6);
	wall_up_02->GetPhysicsObject()->SetFriction(0.3);

	GameObject* wall_angle_03 = gameInst->AddWallToWorld(Vector3(-8, 10, -70), Vector3(1, 10, 29), Vector4(0.8f, 0.8f, 0.8f, 1), "Wall_Angle_03");
	wall_angle_03->GetTransform().SetOrientation(Quaternion::EulerAnglesToQuaternion(0, 50, 0));
	wall_angle_03->GetPhysicsObject()->SetElasticity(2);

	GameObject* wall_down_01 = gameInst->AddWallToWorld(Vector3(-85, 10, 20), Vector3(16, 1, 30), Vector4(0.3f, 0.3f, 0.3f, 1), "Wall_Down_01");
	wall_down_01->GetTransform().SetOrientation(Quaternion::EulerAnglesToQuaternion(0, 0, -30));
	wall_down_01->GetPhysicsObject()->SetFriction(0.8);
	GameObject* wall_down_02 = gameInst->AddWallToWorld(Vector3(-45, 10, 20), Vector3(16, 1, 30), Vector4(0.3f, 0.3f, 0.3f, 1), "Wall_Down_02");
	wall_down_02->GetTransform().SetOrientation(Quaternion::EulerAnglesToQuaternion(0, 0, 30));
	wall_down_02->GetPhysicsObject()->SetFriction(0.8);
	GameObject* wall_down_03 = gameInst->AddWallToWorld(Vector3(-65, 2, 20), Vector3(16, 1, 30), Vector4(0.3f, 0.3f, 0.3f, 1), "Wall_Down_03");
	wall_down_03->GetPhysicsObject()->SetFriction(0.8);

	GameObject* wall_up_03 = gameInst->AddWallToWorld(Vector3(-65, 2.5, -35), Vector3(34, 1, 20), Vector4(0.3f, 0.3f, 0.3f, 1), "Wall_Up_03");
	wall_up_03->GetTransform().SetOrientation(Quaternion::EulerAnglesToQuaternion(-15, 0, 0));
	wall_up_03->GetPhysicsObject()->SetElasticity(0.6);
	wall_up_03->GetPhysicsObject()->SetFriction(0.3);

	
	//Level2
	AddAutoRotateCapsule(Vector3(25, 4, -20), 6, 2, 0.0f, 120.0f, "Capsule_AR_01");
	AddAutoRotateCapsule(Vector3(45, 4, -5), 6, 2, 0.0f, -240.0f, "Capsule_AR_02");
	AddBonusLR(Vector3(22, 4, 15), Vector3(35, 4, 15), 2.0f, "Bonus_LR_01");
	AddBonusLR(Vector3(48, 4, 25), Vector3(35, 4, 25), 2.0f, "Bonus_LR_02");
	
	//Add shoot board
	AddShootBoard(Vector3(80, 10, 90), Vector3(18, 10, 1), 0.5f, Vector3(0, 0, -1), 3, 30, "ShootBoard_01");
	AddShootBoard(Vector3(32, 10, -75), Vector3(17, 10, 1), 0.5f, Vector3(0, 0, 1), 3, 30, "ShootBoard_02");
	AddShootBoard(Vector3(-8, 10, 90), Vector3(20, 10, 1), 0.5f, Vector3(0, 0, -1), 3, 30, "ShootBoard_03");
	AddShootBoard(Vector3(-65, 10, -90), Vector3(34, 10, 1), 0.5f, Vector3(0, 0, 1), 3, 30, "ShootBoard_04");

	CheckPointObject* ckPoint_01 = AddCheckPoint(Vector3(37, 10, -55), Vector3(22, 10, 15), "CheckPoint_01");
	ckPoint_01->AddTriggerCallback([&](GameObject* a) {menu->SetStatusString("Check point 1 reached."); menu->extraScore += 20; });
	CheckPointObject* ckPoint_02 = AddCheckPoint(Vector3(-8, 10, 75), Vector3(21, 10, 14), "CheckPoint_02");
	ckPoint_02->AddTriggerCallback([&](GameObject* a) {menu->SetStatusString("Check point 2 reached."); menu->extraScore += 20; });
	CheckPointObject* ckPoint_03 = AddCheckPoint(Vector3(-65, 10, -75), Vector3(35, 10, 14), "CheckPoint_03");
	ckPoint_03->AddTriggerCallback([&](GameObject* a) {menu->SetStatusString("Check point 3 reached."); menu->extraScore += 20; });
	//AddMixedGrid(Vector3(-65, 50, 20), 8, 8, 6, 6);

	TriggerObject* gridTrigger = AddTriggerVolume(Vector3(-65, 10, -30), Vector3(34, 10, 2), nullptr, "GridTrigger");
	gridTrigger->AddTriggerCallback([&](GameObject* a) { AddMixedGrid(Vector3(-65, 50, 20), 8, 8, 6, 6); });

	TriggerObject* goalTrigger = AddTriggerVolume(Vector3(-65, 10, 80), Vector3(10, 10, 10), gameInst->cubeMesh, "GoalTrigger");
	//goalTrigger->GetRenderObject()->SetColour(Vector4(1., .3, .3, 1));
	goalTrigger->AddTriggerCallback([&](GameObject* a) { menu->SetReachGoal(true); menu->SetStatusString("Goal reached. You win!"); menu->extraScore += 50; });

	mainBall = AddMainBall(Vector3(80, 5, 70), 2.0f, 10.0f, "MainBall");




	GameObject* floor_extra = gameInst->AddFloorToWorld(Vector3(130, -2, 0), Vector3(30, 2, 100), Vector4(1, 1, 1, 0), "Floor_Extra");
	GameObject* cube_projection = gameInst->AddCubeToWorld(Vector3(120, 20, 0), Vector3(2, 2, 2), 10.0f, "Cube_Projection");
	cube_projection->GetPhysicsObject()->SetCollisionMethod(CM_Projection);
	cube_projection->GetPhysicsObject()->AddPhysicsChannel(PhysCh_RayCast);
	GameObject* sphere_penalty = gameInst->AddSphereToWorld(Vector3(140, 20, 0), 2, 10.0f, "Sphere_Panelty");
	sphere_penalty->GetPhysicsObject()->SetCollisionMethod(CM_Penalty);
	sphere_penalty->GetPhysicsObject()->AddPhysicsChannel(PhysCh_RayCast);
	gameInst->AddCubeToWorld(Vector3(130, 20, 20), Vector3(2, 2, 2), 10.0f, "Cube_Impulse")->GetPhysicsObject()->AddPhysicsChannel(PhysCh_RayCast);


	gameInst->AddPlaneToWorld(Vector3(180, 0, 0), Vector2(20, 20), "AirPlane_Bot");
	GameObject* sphere_plane = gameInst->AddSphereToWorld(Vector3(180, 20, 0), 2, 10.0f, "Sphere_Plane");
	sphere_plane->GetPhysicsObject()->AddPhysicsChannel(PhysCh_RayCast);
	sphere_plane->GetPhysicsObject()->AddPhysicsChannel(PhysCh_AirWall);

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
	if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::R))
	{
		mainBall->RestorePosition();
	}

	menu->Update(dt);
}

MainBallObject* BallGameMode::AddMainBall(const Vector3& position, float radius, float inverseMass, const std::string& name)
{
	MainBallObject* ball = new MainBallObject(name);

	Vector3 sphereSize = Vector3(radius, radius, radius);
	SphereVolume* volume = new SphereVolume(radius);
	ball->SetBoundingVolume((CollisionVolume*)volume);

	ball->GetTransform()
		.SetScale(sphereSize)
		.SetPosition(position);

	ball->SetRenderObject(new RenderObject(&ball->GetTransform(), gameInst->sphereMesh, gameInst->basicTex, gameInst->basicShader));
	ball->GetRenderObject()->SetColour(Vector4(0.3, 1, 0.3, 1));
	ball->SetPhysicsObject(new PhysicsObject(&ball->GetTransform(), ball->GetBoundingVolume()));
	ball->GetPhysicsObject()->SetInverseMass(inverseMass);
	ball->GetPhysicsObject()->InitSphereInertia();
	ball->GetPhysicsObject()->AddPhysicsChannel(PhysCh_AirWall);

	ball->SetRestorePoint(position);

	gameInst->world->AddGameObject(ball);

	return ball;
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

CheckPointObject* BallGameMode::AddCheckPoint(const Vector3& position, Vector3 dimensions, const std::string& name)
{
	CheckPointObject* cube = new CheckPointObject(name);

	AABBVolume* volume = new AABBVolume(dimensions);

	cube->SetBoundingVolume((CollisionVolume*)volume);

	cube->GetTransform()
		.SetPosition(position)
		.SetScale(dimensions * 2);

	cube->SetRenderObject(nullptr);
	//cube->SetRenderObject(new RenderObject(&cube->GetTransform(), gameInst->cubeMesh, gameInst->basicTex, gameInst->basicShader));
	cube->SetPhysicsObject(new PhysicsObject(&cube->GetTransform(), cube->GetBoundingVolume()));
	cube->GetPhysicsObject()->SetInverseMass(0.0f);
	cube->GetPhysicsObject()->InitCubeInertia();
	cube->GetPhysicsObject()->SetPhysicsChannel(PhysCh_Static | PhysCh_Dynamic | PhysCh_EventOnly);

	gameInst->world->AddGameObject(cube);

	return cube;
}

GameObject* BallGameMode::AddBonusLR(const Vector3& position, const Vector3& center, float scale, const std::string& name)
{
	LineMoveObject* bonus = new LineMoveObject(name);

	SphereVolume* volume = new SphereVolume(scale);
	bonus->SetBoundingVolume((CollisionVolume*)volume);
	bonus->GetTransform()
		.SetScale(Vector3(scale * .25f, scale * .25f, scale * .25f))
		.SetPosition(position);

	bonus->SetRenderObject(new RenderObject(&bonus->GetTransform(), gameInst->bonusMesh, nullptr, gameInst->basicShader));
	bonus->SetPhysicsObject(new PhysicsObject(&bonus->GetTransform(), bonus->GetBoundingVolume()));
	bonus->GetPhysicsObject()->SetInverseMass(0.0f);
	bonus->GetPhysicsObject()->InitSphereInertia();
	bonus->GetPhysicsObject()->SetFriction(0.0f);
	bonus->GetPhysicsObject()->AddPhysicsChannel(PhysCh_Static);
	bonus->GetPhysicsObject()->AddPhysicsChannel(PhysCh_NoForce);

	bonus->SetCenterPoint(center);

	gameInst->world->AddGameObject(bonus);

	return bonus;
}

void BallGameMode::AddMixedGrid(const Vector3& center, int numRows, int numCols, float rowSpacing, float colSpacing)
{
	float sphereRadius = 1.0f;
	Vector3 cubeDims = Vector3(1, 1, 1);

	for (int x = 0; x < numCols; ++x) {
		for (int z = 0; z < numRows; ++z) {
			Vector3 position = Vector3((x-numCols/2) * colSpacing, 0.0f, (z-numRows/2) * rowSpacing) + center;

			if (rand() % 2) {
				gameInst->AddCubeToWorld_OBB(position, cubeDims, 10.0f, "MixGrid_Cube_" + std::to_string(x * numRows + z));
			}
			else {
				gameInst->AddSphereToWorld(position, sphereRadius, 10.0f, "MixGrid_Sphere_" + std::to_string(x * numRows + z));
			}
		}
	}
}

TriggerObject* BallGameMode::AddTriggerVolume(const Vector3& position, const Vector3& dimensions, MeshGeometry* mesh, const std::string& name)
{
	TriggerObject* cube = new TriggerObject(name);

	AABBVolume* volume = new AABBVolume(dimensions);

	cube->SetBoundingVolume((CollisionVolume*)volume);

	cube->GetTransform()
		.SetPosition(position)
		.SetScale(dimensions * 2);

	mesh ? cube->SetRenderObject(new RenderObject(&cube->GetTransform(), mesh, gameInst->dogeTex, gameInst->basicShader)) : cube->SetRenderObject(nullptr);
	cube->SetPhysicsObject(new PhysicsObject(&cube->GetTransform(), cube->GetBoundingVolume()));
	cube->GetPhysicsObject()->SetInverseMass(0.0f);
	cube->GetPhysicsObject()->InitCubeInertia();
	cube->GetPhysicsObject()->SetPhysicsChannel(PhysCh_Static | PhysCh_Dynamic | PhysCh_EventOnly);

	gameInst->world->AddGameObject(cube);

	return cube;
}
