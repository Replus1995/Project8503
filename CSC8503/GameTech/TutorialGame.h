#pragma once
#include <memory>
#include "GameTechRenderer.h"
#include "../CSC8503Common/PhysicsSystem.h"
#include "../CSC8503Common/StateGameObject.h"
#include "../CSC8503Common/PushdownMachine.h"

class GameUI;

namespace NCL {
	namespace CSC8503 {
		class TutorialGame		{
		public:
			TutorialGame();
			~TutorialGame();

			virtual void UpdateGame(float dt);
			virtual void UpdateRender(float dt);

			GameUI* GetUI() const { return gameUI; };
			bool IsFreezed() const { return freezed; };
			void SetFreeze(bool freeze) { freezed = freeze; };
			bool ShouldQuit() const { return quit; };

			void SetBallLevel();
			void SetMazeLevel();

		protected:
			virtual void UpdateKeyActions(float dt);

			void InitialiseUI();
			void InitialiseAssets();
			void InitCamera();

			void InitWorld();
			void InitGameExamples();

			void InitSphereGridWorld(int numRows, int numCols, float rowSpacing, float colSpacing, float radius);
			void InitMixedGridWorld(int numRows, int numCols, float rowSpacing, float colSpacing);
			void InitCubeGridWorld(int numRows, int numCols, float rowSpacing, float colSpacing, const Vector3& cubeDims);
			void InitDefaultFloor();
			void BridgeConstraintTest();
			void SliderContraintTest();
			void HingeContraintTest();
	
			bool SelectObject();
			void MoveSelectedObject();
			void DebugObjectMovement();
			void LockedObjectMovement();

			GameObject* AddFloorToWorld(const Vector3& position, const std::string& name);
			GameObject* AddSphereToWorld(const Vector3& position, float radius, float inverseMass = 10.0f, const std::string& name = "");
			GameObject* AddCubeToWorld(const Vector3& position, Vector3 dimensions, float inverseMass = 10.0f, const std::string& name = "");
			GameObject* AddCubeToWorld_OBB(const Vector3& position, Vector3 dimensions, float inverseMass = 10.0f, const std::string& name = "");
			
			GameObject* AddCapsuleToWorld(const Vector3& position, float halfHeight, float radius, float inverseMass = 10.0f, const std::string& name = "");

			GameObject* AddPlayerToWorld(const Vector3& position, const std::string& name);
			GameObject* AddEnemyToWorld(const Vector3& position, const std::string& name);
			GameObject* AddBonusToWorld(const Vector3& position, const std::string& name);


			GameObject* AddFloorToWorld(const Vector3& position, Vector3 dimensions, Vector4 Colour, const std::string& name);
			GameObject* AddWallToWorld(const Vector3& position, Vector3 dimensions, Vector4 Colour, const std::string& name);
			GameObject* AddAirWallToWorld(const Vector3& position, Vector3 dimensions, const std::string& name);
			//GameObject* AddAirCubeToWorld(const Vector3& position, Vector3 dimensions, float inverseMass = 10.0f, const std::string& name = "");

			StateGameObject* AddStateObjectToWorld(const Vector3& position, const std::string& name);

			GameTechRenderer*	renderer;
			PhysicsSystem*		physics;
			GameWorld*			world;

			bool useGravity;

			float		forceMagnitude;

			GameObject* selectionObject = nullptr;

			OGLMesh*	capsuleMesh = nullptr;
			OGLMesh*	cubeMesh	= nullptr;
			OGLMesh*	sphereMesh	= nullptr;
			OGLTexture* basicTex	= nullptr;
			OGLShader*	basicShader = nullptr;

			//Coursework Meshes
			OGLMesh*	charMeshA	= nullptr;
			OGLMesh*	charMeshB	= nullptr;
			OGLMesh*	enemyMesh	= nullptr;
			OGLMesh*	bonusMesh	= nullptr;
			OGLMesh*	appleMesh = nullptr;
			OGLTexture* dogeTex = nullptr;

			//Coursework Additional functionality	
			GameObject* lockedObject	= nullptr;
			Vector3 lockedOffset		= Vector3(0, 14, 20);
			void LockCameraToObject(GameObject* o) {
				lockedObject = o;
			}
			
			Vector3 camInitPos = Vector3(-60,40,60);
			Vector3 camInitRot = Vector3(0, 315, -15);


			friend class GameMode;
			std::unique_ptr<GameMode> gameMode = nullptr;
			friend class BallGameMode;
			friend class MazeGameMode;
			friend class MazeMap;

			friend class TutorialMenu;
			std::shared_ptr<TutorialMenu> gameMenu = nullptr;

			GameUI* gameUI;

			bool quit;
			bool freezed;
			PushdownMachine* pauseMachine;
		};
	}
}

