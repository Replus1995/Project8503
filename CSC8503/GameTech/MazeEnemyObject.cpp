#include "MazeEnemyObject.h"

#include "../CSC8503Common/BehaviourAction.h"
#include "../CSC8503Common/BehaviourSelector.h"
#include "../CSC8503Common/BehaviourSequence.h"
#include "../CSC8503Common/BehaviourParallel.h"

#include "../CSC8503Common/GameWorld.h"
#include "DebugAI.h"

using namespace NCL;
using namespace CSC8503;

MazeEnemyObject::MazeEnemyObject(const std::string& name, MazeMap* map, MazePlayerObject* player)
	: GameObject(name)
	, mazeMap(map)
	, mazePlayer(player)
{
	destPowerUpPos = mazePlayer->GetTransform().GetPosition();
	FreezeCount = 0.0f;

	BehaviourAction* searchAction = new BehaviourAction("Enemy search", [&](float dt, BehaviourState state)->BehaviourState {
		if (state == Initialise)
		{
			Quaternion rotation = Quaternion::EulerAnglesToQuaternion(0, dt * 120, 0) * transform.GetOrientation();
			transform.SetOrientation(rotation);
			state = Success;
		}
		return state;
	});

	BehaviourAction* findPowerUp_01 = new BehaviourAction("Enemy find powerup 01", [&](float dt, BehaviourState state)->BehaviourState {
		if (state == Initialise)
		{
			if (!powerUp_01) return Failure;

			//test obstacles
			Vector3 puDir = powerUp_01->GetTransform().GetPosition() - transform.GetPosition();
			float puDist = puDir.Length();
			puDir.Normalise();
			Ray t_ray(transform.GetPosition(), puDir);
			RayCollision collision;
			if (GameWorld::GetInstance()->Raycast(t_ray, collision, true, PhysCh_Dynamic))
			{
				GameObject* collidedObject = (GameObject*)collision.node;
				if (collidedObject->GetWorldID() != powerUp_01->GetWorldID()) return Failure;
			}
			else
			{
				return Failure;
			}
			
			Quaternion orientation = Quaternion::FromAxis(Vector3::Cross(puDir, Vector3(0, 1, 0)), Vector3(0, 1, 0), puDir);
			transform.SetOrientation(orientation);

			destPowerUpPos = powerUp_01->GetTransform().GetPosition();
			float destDistance = (destPowerUpPos - transform.GetPosition()).Length();
			pathToPowerUp.Clear();
			if (destDistance <= mazeMap->GetHalfSize() * 2)
			{
				puNextPos = destPowerUpPos;
			}
			else 
			{
				mazeMap->FindPath(transform.GetPosition(), destPowerUpPos, pathToPowerUp);
				pathToPowerUp.PopWaypoint(puNextPos);
			}

			state = Ongoing;
		}
		else if (state == Ongoing)
		{
			float puDistance = (powerUp_01->GetTransform().GetPosition() - destPowerUpPos).Length();
			if (puDistance > 1.0f)
			{
				if (pathToPowerUp.IsEmpty()) 
					return Success;
				else 
					return Failure;
			}

			Vector3 curPos = transform.GetPosition();
			if (!pathToPowerUp.IsEmpty())
			{
				Vector3 nextDist = curPos - puNextPos;
				if (abs(nextDist.x) <= mazeMap->GetHalfSize() && abs(nextDist.z) <= mazeMap->GetHalfSize())
				{
					pathToPowerUp.PopWaypoint(puNextPos);
					/*if (pathToPowerUp.IsEmpty())
					{
						puNextPos = powerUp_01->GetTransform().GetPosition();
					}*/
				}
			}
			Vector3 moveDir = puNextPos - curPos;
			moveDir.y = 0;
			//physicsObject->AddForce(moveDir * 10);
			physicsObject->SetLinearVelocity(moveDir.Normalised() * 50);
		}
		return state;
	});

	BehaviourAction* findPowerUp_02 = new BehaviourAction("Enemy find powerup 02", [&](float dt, BehaviourState state)->BehaviourState {
		if (state == Initialise)
		{
			if (!powerUp_02) return Failure;

			//test obstacles
			Vector3 puDir = powerUp_02->GetTransform().GetPosition() - transform.GetPosition();
			float puDist = puDir.Length();
			puDir.Normalise();
			Ray t_ray(transform.GetPosition(), puDir);
			RayCollision collision;
			if (GameWorld::GetInstance()->Raycast(t_ray, collision, true, PhysCh_Dynamic))
			{
				GameObject* collidedObject = (GameObject*)collision.node;
				if (collidedObject->GetWorldID() != powerUp_02->GetWorldID()) return Failure;
			}
			else
			{
				return Failure;
			}

			destPowerUpPos = powerUp_02->GetTransform().GetPosition();
			float destDistance = (destPowerUpPos - transform.GetPosition()).Length();
			pathToPowerUp.Clear();
			if (destDistance <= mazeMap->GetHalfSize() * 2)
			{
				puNextPos = destPowerUpPos;
			}
			else
			{
				mazeMap->FindPath(transform.GetPosition(), destPowerUpPos, pathToPowerUp);
				pathToPowerUp.PopWaypoint(puNextPos);
			}

			state = Ongoing;
		}
		else if (state == Ongoing)
		{
			float puDistance = (powerUp_02->GetTransform().GetPosition() - destPowerUpPos).Length();
			if (puDistance > 1.0f)
			{
				if (pathToPowerUp.IsEmpty())
					return Success;
				else
					return Failure;
			}

			Vector3 curPos = transform.GetPosition();
			if (!pathToPowerUp.IsEmpty())
			{
				Vector3 nextDist = curPos - puNextPos;
				if (abs(nextDist.x) <= mazeMap->GetHalfSize() && abs(nextDist.z) <= mazeMap->GetHalfSize())
				{
					pathToPowerUp.PopWaypoint(puNextPos);
					/*if (pathToPowerUp.IsEmpty())
					{
						puNextPos = powerUp_02->GetTransform().GetPosition();
					}*/
				}
			}
			Vector3 moveDir = puNextPos - curPos;
			moveDir.y = 0;
			//physicsObject->AddForce(moveDir * 10);
			physicsObject->SetLinearVelocity(moveDir.Normalised() * 50);
		}
		return state;
	});

	BehaviourSelector* findPowerUpSelector = new BehaviourSelector("Find PowerUp");
	findPowerUpSelector->AddChild(findPowerUp_01);
	findPowerUpSelector->AddChild(findPowerUp_02);

	puFindBehaviour = new BehaviourParallel("Try find PowerUp");
	puFindBehaviour->AddChild(searchAction);
	puFindBehaviour->AddChild(findPowerUpSelector);


}

MazeEnemyObject::~MazeEnemyObject()
{
	delete puFindBehaviour;
}

void MazeEnemyObject::Update(float dt)
{
	if (FreezeCount > 0.0f) 
	{ 
		FreezeCount -= dt;  
		return;
	}

	BehaviourState puFindState = puFindBehaviour->Execute(dt);
	if (puFindState == Ongoing)
	{
		return;
	}
	else{
		puFindBehaviour->Reset();
	}
	Vector3 moveDest;
	float playDistance = (mazePlayer->GetTransform().GetPosition() - transform.GetPosition()).Length();
	if (playDistance <= mazeMap->GetHalfSize() * 2)
	{
		moveDest = mazePlayer->GetTransform().GetPosition();
	}
	else
	{

		NavigationPath pathToPlayer;
		mazeMap->FindPath(transform.GetPosition(), mazePlayer->GetTransform().GetPosition(), pathToPlayer);
		DebugAI::DisplayPathFinding(pathToPlayer.GetPoints(), Vector4(0, 1, 0, 1));
		pathToPlayer.PopWaypoint(moveDest);
		pathToPlayer.PopWaypoint(moveDest);
	}

	Vector3 moveDir = moveDest - transform.GetPosition();
	moveDir.y = 0;
	//physicsObject->AddForce(moveDir * 8);
	physicsObject->SetLinearVelocity(moveDir.Normalised() * 30);
}

void MazeEnemyObject::OnCollisionBegin(GameObject* otherObject)
{
	if (otherObject->GetWorldID() == mazePlayer->GetWorldID())
	{
		mazePlayer->RestorePosition();
		RandomSpawn();
	}
}

void MazeEnemyObject::RandomSpawn()
{
	float signX = rand() % 2 > 0 ? 1 : -1;
	float signZ = rand() % 2 > 0 ? 1 : -1;
	transform.SetPosition(Vector3(signX * 85, 4, signZ * 85));
}

void MazeEnemyObject::SetFreeze(float time)
{
	FreezeCount = time;
	physicsObject->SetLinearVelocity(Vector3());
}