#include "MazeGameMode.h"
#include "MazeMap.h"
#include "BonusObject.h"
#include "MazePlayerObject.h"
#include "MazeEnemyObject.h"
#include "GameUI.h"
#include "../CSC8503Common/PositionConstraint.h"
#include "../CSC8503Common/HingeConstraint.h"

using namespace NCL;
using namespace CSC8503;

MazeGameMode::MazeGameMode(TutorialGame* game)
	: GameMode(game)
{
	map = new MazeMap("MazeGrid.txt");
	menu.reset(new MazeLevelMenu(score));
	gameInst->gameUI->PushMenu(menu);
}

MazeGameMode::~MazeGameMode()
{
	delete map;
	map = nullptr;
	gameInst->gameUI->RemoveMenu(menu);
}

void MazeGameMode::SetupScene()
{
	gameInst->camInitPos = Vector3(0, 280, 0);
	gameInst->camInitRot = Vector3(0, 0, -90);

	GameObject* floor_01 = gameInst->AddFloorToWorld(Vector3(0, -2, 0), Vector3(100, 2, 100), Vector4(1, 1, 1, 0), "Floor_01");
	floor_01->GetPhysicsObject()->SetFriction(0.8);
	floor_01->GetPhysicsObject()->SetElasticity(0.6);

	gameInst->AddWallToWorld(Vector3(-95, 10, 0), Vector3(5, 10, 90), Vector4(0.8f, 0.8f, 0.8f, 1), "Wall_RM");
	gameInst->AddWallToWorld(Vector3(95, 10, 0), Vector3(5, 10, 90), Vector4(0.8f, 0.8f, 0.8f, 1), "Wall_RM");
	gameInst->AddWallToWorld(Vector3(0, 10, -95), Vector3(100, 10, 5), Vector4(0.8f, 0.8f, 0.8f, 1), "Wall_FM");
	gameInst->AddWallToWorld(Vector3(0, 10, 95), Vector3(100, 10, 5), Vector4(0.8f, 0.8f, 0.8f, 1), "Wall_BM");

	gameInst->AddWallToWorld(Vector3(0, 10, -75), Vector3(80, 10, 5), Vector4(0.8f, 0.8f, 0.8f, 1), "Wall_R_01");
	gameInst->AddWallToWorld(Vector3(30, 10, -55), Vector3(50, 10, 5), Vector4(0.8f, 0.8f, 0.8f, 1), "Wall_R_02");
	gameInst->AddWallToWorld(Vector3(-20, 10, -35), Vector3(30, 10, 5), Vector4(0.8f, 0.8f, 0.8f, 1), "Wall_R_03");
	gameInst->AddWallToWorld(Vector3(-60, 10, -15), Vector3(20, 10, 5), Vector4(0.8f, 0.8f, 0.8f, 1), "Wall_R_04");
	gameInst->AddWallToWorld(Vector3(20, 10, -15), Vector3(50, 10, 5), Vector4(0.8f, 0.8f, 0.8f, 1), "Wall_R_05");
	gameInst->AddWallToWorld(Vector3(-45, 10, 5), Vector3(35, 10, 5), Vector4(0.8f, 0.8f, 0.8f, 1), "Wall_R_06");
	gameInst->AddWallToWorld(Vector3(65, 10, 5), Vector3(25, 10, 5), Vector4(0.8f, 0.8f, 0.8f, 1), "Wall_R_07");
	gameInst->AddWallToWorld(Vector3(15, 10, 55), Vector3(35, 10, 5), Vector4(0.8f, 0.8f, 0.8f, 1), "Wall_R_08");
	gameInst->AddWallToWorld(Vector3(-10, 10, 75), Vector3(60, 10, 5), Vector4(0.8f, 0.8f, 0.8f, 1), "Wall_R_09");

	//gameInst->AddWallToWorld(Vector3(75, 10, -65), Vector3(5, 10, 5), Vector4(0.8f, 0.8f, 0.8f, 1), "Wall_C_01");
	gameInst->AddWallToWorld(Vector3(-45, 10, -55), Vector3(5, 10, 15), Vector4(0.8f, 0.8f, 0.8f, 1), "Wall_C_02");
	gameInst->AddWallToWorld(Vector3(-75, 10, -40), Vector3(5, 10, 20), Vector4(0.8f, 0.8f, 0.8f, 1), "Wall_C_03");
	gameInst->AddWallToWorld(Vector3(45, 10, -35), Vector3(5, 10, 15), Vector4(0.8f, 0.8f, 0.8f, 1), "Wall_C_04");
	gameInst->AddWallToWorld(Vector3(15, 10, 15), Vector3(5, 10, 25), Vector4(0.8f, 0.8f, 0.8f, 1), "Wall_C_05");
	gameInst->AddWallToWorld(Vector3(-15, 10, 30), Vector3(5, 10, 20), Vector4(0.8f, 0.8f, 0.8f, 1), "Wall_C_06");
	gameInst->AddWallToWorld(Vector3(45, 10, 30), Vector3(5, 10, 20), Vector4(0.8f, 0.8f, 0.8f, 1), "Wall_C_07");
	gameInst->AddWallToWorld(Vector3(-55, 10, 45), Vector3(5, 10, 25), Vector4(0.8f, 0.8f, 0.8f, 1), "Wall_C_08");

	AddRotateGate(Vector3(75, 10, -65), Vector3(.5, 10, 5), "Gate_01")->GetPhysicsObject()->SetCollisionMethod(CM_Penalty);
	
	player = AddPlayer();
	player->GetRenderObject()->SetColour(Vector4(0.3, 1, 0.3, 1));
	player->SetRestorePoint(Vector3(0, 4, 0));
	player->RestorePosition();

	enemy = AddEnemy();
	enemy->GetRenderObject()->SetColour(Vector4(1, 0.3, 0.3, 1));
	enemy->RandomSpawn();

	coin = AddCoinObject();
	coin->AddTriggerCallback([&](GameObject* a)
	{
		MazePlayerObject* t_player = dynamic_cast<MazePlayerObject*>(a);
		if (t_player) 
		{
			score += 50;
			menu->SetStatusString("Coin collected!");
		}
		ReplaceBonusObject(coin);
	});
	ReplaceBonusObject(coin);

	powerUp_01 = AddPowerUpObject("PowerUp 01");
	powerUp_01->AddTriggerCallback([&](GameObject* a)
	{
		MazePlayerObject* t_player = dynamic_cast<MazePlayerObject*>(a);
		if (t_player)
		{
			score += 20;
			enemy->SetFreeze(3.0f);
			player->SetMoveForce(200.0f);
			player->SetPowerUpCount(3.0f);
			menu->SetStatusString("Blue capsule collected!");
		}
		ReplaceBonusObject(powerUp_01);
	});
	ReplaceBonusObject(powerUp_01);

	powerUp_02 = AddPowerUpObject("PowerUp 02");
	powerUp_02->AddTriggerCallback([&](GameObject* a)
	{
		MazePlayerObject* t_player = dynamic_cast<MazePlayerObject*>(a);
		if (t_player)
		{
			score += 20;
			enemy->SetFreeze(3.0f);
			player->SetMoveForce(200.0f);
			player->SetPowerUpCount(3.0f);
			menu->SetStatusString("Blue capsule collected!");
		}
		ReplaceBonusObject(powerUp_02);
	});
	ReplaceBonusObject(powerUp_02);

	enemy->SetPowerUps(powerUp_01, powerUp_02);

}

void MazeGameMode::Update(float dt)
{
	menu->Update(dt);
}

MazePlayerObject* MazeGameMode::AddPlayer()
{
	float meshSize = 4.0f;
	float inverseMass = 0.5f;

	MazePlayerObject* character = new MazePlayerObject("MazePlayer");

	AABBVolume* volume = new AABBVolume(Vector3(0.3f, 0.85f, 0.3f) * meshSize);

	character->SetBoundingVolume((CollisionVolume*)volume);

	character->GetTransform()
		.SetScale(Vector3(meshSize, meshSize, meshSize))
		.SetPosition(Vector3(0,0,0));

	character->SetRenderObject(new RenderObject(&character->GetTransform(), gameInst->charMeshA, nullptr, gameInst->basicShader));
	
	character->SetPhysicsObject(new PhysicsObject(&character->GetTransform(), character->GetBoundingVolume()));

	character->GetPhysicsObject()->SetInverseMass(inverseMass);
	character->GetPhysicsObject()->InitSphereInertia();
	character->GetPhysicsObject()->SetElasticity(0.2);

	gameInst->world->AddGameObject(character);

	return character;
}

MazeEnemyObject* MazeGameMode::AddEnemy()
{
	float meshSize = 4.0f;
	float inverseMass = 0.5f;

	MazeEnemyObject* character = new MazeEnemyObject("MazeEnemy", map, player);

	AABBVolume* volume = new AABBVolume(Vector3(0.3f, 0.85f, 0.3f) * meshSize);

	character->SetBoundingVolume((CollisionVolume*)volume);

	character->GetTransform()
		.SetScale(Vector3(meshSize, meshSize, meshSize))
		.SetPosition(Vector3(0, 0, 0));

	character->SetRenderObject(new RenderObject(&character->GetTransform(), gameInst->charMeshB, nullptr, gameInst->basicShader));

	character->SetPhysicsObject(new PhysicsObject(&character->GetTransform(), character->GetBoundingVolume()));

	character->GetPhysicsObject()->SetInverseMass(inverseMass);
	character->GetPhysicsObject()->InitSphereInertia();
	character->GetPhysicsObject()->SetElasticity(0.2);
	character->GetPhysicsObject()->SetFriction(0.8);

	gameInst->world->AddGameObject(character);

	return character;
}

BonusObject* MazeGameMode::AddCoinObject()
{
	BonusObject* bonus = new BonusObject("Coin");

	SphereVolume* volume = new SphereVolume(4.0f);
	bonus->SetBoundingVolume((CollisionVolume*)volume);
	bonus->GetTransform()
		.SetScale(Vector3(0.5, 0.5, 0.5))
		.SetPosition(Vector3(0, 0, 0));

	bonus->SetRenderObject(new RenderObject(&bonus->GetTransform(), gameInst->bonusMesh, nullptr, gameInst->basicShader));
	bonus->GetRenderObject()->SetColour(Vector4(.9, .9, .2, 1));
	bonus->SetPhysicsObject(new PhysicsObject(&bonus->GetTransform(), bonus->GetBoundingVolume()));
	bonus->GetPhysicsObject()->SetInverseMass(0.0f);
	bonus->GetPhysicsObject()->InitSphereInertia();
	bonus->GetPhysicsObject()->AddPhysicsChannel(PhysCh_Static);
	bonus->GetPhysicsObject()->AddPhysicsChannel(PhysCh_EventOnly);

	bonus->autoRotate = true;

	gameInst->world->AddGameObject(bonus);

	return bonus;
}

BonusObject* MazeGameMode::AddPowerUpObject(const std::string& name)
{
	BonusObject* bonus = new BonusObject(name);

	AABBVolume* volume = new AABBVolume(Vector3(2,4,2));
	bonus->SetBoundingVolume((CollisionVolume*)volume);
	bonus->GetTransform()
		.SetScale(Vector3(2, 2, 2))
		.SetPosition(Vector3(0, 0, 0));

	bonus->SetRenderObject(new RenderObject(&bonus->GetTransform(), gameInst->capsuleMesh, nullptr, gameInst->basicShader));
	bonus->GetRenderObject()->SetColour(Vector4(.2, .2, .9, 1));
	bonus->SetPhysicsObject(new PhysicsObject(&bonus->GetTransform(), bonus->GetBoundingVolume()));
	bonus->GetPhysicsObject()->SetInverseMass(0.0f);
	bonus->GetPhysicsObject()->InitSphereInertia();
	bonus->GetPhysicsObject()->AddPhysicsChannel(PhysCh_Static);
	bonus->GetPhysicsObject()->AddPhysicsChannel(PhysCh_EventOnly);

	bonus->autoRotate = false;

	gameInst->world->AddGameObject(bonus);

	return bonus;
}

void MazeGameMode::ReplaceBonusObject(BonusObject* object)
{
	Vector3 objectPos;
	size_t lastNodeId = object->mazeNodeId;
	object->mazeNodeId = map->FindRandomPos(objectPos);
	map->SetPosAvailable(lastNodeId);
	object->GetTransform().SetPosition(Vector3(objectPos.x, 4, objectPos.z));
	object->SetActive(true);
}

GameObject* MazeGameMode::AddRotateGate(const Vector3& position, Vector3 dimensions, const std::string& name)
{
	GameObject* center = new GameObject(name+"_Center");
	AABBVolume* centerVolume = new AABBVolume(Vector3(1,1,1));
	center->SetBoundingVolume((CollisionVolume*)centerVolume);
	center->GetTransform().SetPosition(position);
	center->SetRenderObject(nullptr);
	center->SetPhysicsObject(new PhysicsObject(&center->GetTransform(), center->GetBoundingVolume()));

	center->GetPhysicsObject()->SetInverseMass(0);
	center->GetPhysicsObject()->InitCubeInertia();
	center->GetPhysicsObject()->SetPhysicsChannel(PhysCh_Static);
	center->GetPhysicsObject()->SetElasticity(0);
	center->GetPhysicsObject()->SetFriction(0);

	gameInst->world->AddGameObject(center);

	GameObject* door = new GameObject(name+"_Door");
	OBBVolume* doorVolume = new OBBVolume(dimensions);
	door->SetBoundingVolume((CollisionVolume*)doorVolume);
	door->GetTransform()
		.SetPosition(position)
		.SetScale(dimensions * 2);

	door->SetRenderObject(new RenderObject(&door->GetTransform(), gameInst->cubeMesh, nullptr, gameInst->basicShader));
	door->GetRenderObject()->SetColour(Vector4(.5, .5, .5, 1));
	door->SetPhysicsObject(new PhysicsObject(&door->GetTransform(), door->GetBoundingVolume()));

	door->GetPhysicsObject()->SetInverseMass(8);
	door->GetPhysicsObject()->InitCubeInertia();
	door->GetPhysicsObject()->AddPhysicsChannel(PhysCh_Static);
	door->GetPhysicsObject()->SetElasticity(0.8);
	door->GetPhysicsObject()->SetFriction(0.1);

	gameInst->world->AddGameObject(door);

	HingeConstraint* hingeCons = new HingeConstraint(center, door, 1);
	gameInst->world->AddConstraint(hingeCons);

	PositionConstraint* posCons = new PositionConstraint(center, door, 0);
	gameInst->world->AddConstraint(posCons);



	return door;
}
