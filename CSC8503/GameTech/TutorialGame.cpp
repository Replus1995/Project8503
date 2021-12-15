#include "TutorialGame.h"
#include "../CSC8503Common/GameWorld.h"
#include "../../Plugins/OpenGLRendering/OGLMesh.h"
#include "../../Plugins/OpenGLRendering/OGLShader.h"
#include "../../Plugins/OpenGLRendering/OGLTexture.h"
#include "../../Common/TextureLoader.h"

#include "../CSC8503Common/PositionConstraint.h"
#include "../CSC8503Common/SliderConstraint.h"
#include "../CSC8503Common/HingeConstraint.h"

#include "BallGameMode.h"
#include "MazeGameMode.h"
#include "GameUI.h"
#include "TutorialMenu.h"
#include "PauseState.h"

#include "../CSC8503Common/Debug.h"

using namespace NCL;
using namespace CSC8503;

TutorialGame::TutorialGame(){
	world		= new GameWorld();
	renderer	= new GameTechRenderer(*world);
	physics		= new PhysicsSystem(*world);

	forceMagnitude	= 100.0f;
	useGravity		= true;

	quit = false;
	freezed = true;

	Debug::SetRenderer(renderer);
	physics->UseGravity(useGravity);

	InitialiseUI();
	InitialiseAssets();

	//SetBallLevel();
}

void TutorialGame::InitialiseUI()
{
	gameUI = new GameUI();
	renderer->SetUI(gameUI);
	gameMenu.reset(new TutorialMenu(this));
	gameUI->PushMenu(gameMenu);
	pauseMachine = new PushdownMachine(new InGameState(this));
}

/*

Each of the little demo scenarios used in the game uses the same 2 meshes, 
and the same texture and shader. There's no need to ever load in anything else
for this module, even in the coursework, but you can add it if you like!

*/
void TutorialGame::InitialiseAssets() {
	auto loadFunc = [](const string& name, OGLMesh** into) {
		*into = new OGLMesh(name);
		(*into)->SetPrimitiveType(GeometryPrimitive::Triangles);
		(*into)->UploadToGPU();
	};

	loadFunc("cube.msh"		 , &cubeMesh);
	loadFunc("sphere.msh"	 , &sphereMesh);
	loadFunc("Male1.msh"	 , &charMeshA);
	loadFunc("courier.msh"	 , &charMeshB);
	loadFunc("security.msh"	 , &enemyMesh);
	loadFunc("coin.msh"		 , &bonusMesh);
	loadFunc("capsule.msh"	 , &capsuleMesh);
	loadFunc("apple.msh"	 , &appleMesh);

	basicTex	= (OGLTexture*)TextureLoader::LoadAPITexture("checkerboard.png");
	basicShader = new OGLShader("GameTechVert.glsl", "GameTechFrag.glsl");

	dogeTex = (OGLTexture*)TextureLoader::LoadAPITexture("doge.png");

	InitCamera();
}

TutorialGame::~TutorialGame()	{
	freezed = true;
	gameMode.reset();

	delete cubeMesh;
	delete sphereMesh;
	delete charMeshA;
	delete charMeshB;
	delete enemyMesh;
	delete bonusMesh;
	delete capsuleMesh;
	delete appleMesh;

	delete dogeTex;

	delete pauseMachine;
	delete gameUI;

	delete basicTex;
	delete basicShader;

	delete physics;
	delete renderer;
	delete world;
}

void TutorialGame::UpdateGame(float dt) {
	
	gameUI->UpdateUI();
	quit = !pauseMachine->Update(dt);

	if (freezed) 
		return;

	physics->BuildSpaceTree();
	
	UpdateKeyActions(dt);
	if (gameMode) gameMode->Update(dt);

	physics->Update(dt);

	if (lockedObject != nullptr) {
		Vector3 objPos = lockedObject->GetTransform().GetPosition();
		Vector3 camPos = objPos + lockedOffset;

		Matrix4 temp = Matrix4::BuildViewMatrix(camPos, objPos, Vector3(0,1,0));

		Matrix4 modelMat = temp.Inverse();

		Quaternion q(modelMat);
		Vector3 angles = q.ToEuler(); //nearly there now!

		world->GetMainCamera()->SetPosition(camPos);
		world->GetMainCamera()->SetPitch(angles.x);
		world->GetMainCamera()->SetYaw(angles.y);

		//Debug::DrawAxisLines(lockedObject->GetTransform().GetMatrix(), 2.0f);
	}

	world->UpdateGameObjects(dt);
	world->UpdateWorld(dt);
}

void TutorialGame::UpdateRender(float dt)
{
	Debug::FlushRenderables(dt);
	renderer->Update(dt);
	renderer->Render();
}

void TutorialGame::SetBallLevel()
{
	gameMode.reset(new BallGameMode(this));
	InitWorld();
	InitCamera();
}

void TutorialGame::SetMazeLevel()
{
	gameMode.reset(new MazeGameMode(this));
	InitWorld();
	InitCamera();
}

void TutorialGame::UpdateKeyActions(float dt) {
	world->GetMainCamera()->UpdateCamera(dt);

	if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::F1)) {
		InitWorld(); //We can reset the simulation at any time with F1
		selectionObject = nullptr;
		lockedObject	= nullptr;
	}

	if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::F2)) {
		InitCamera(); //F2 will reset the camera to a specific default place
	}

	//Running certain physics updates in a consistent order might cause some
	//bias in the calculations - the same objects might keep 'winning' the constraint
	//allowing the other one to stretch too much etc. Shuffling the order so that it
	//is random every frame can help reduce such bias.
	if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::F9)) {
		world->ShuffleConstraints(true);
	}
	if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::F10)) {
		world->ShuffleConstraints(false);
	}

	if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::F7)) {
		world->ShuffleObjects(true);
	}
	if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::F8)) {
		world->ShuffleObjects(false);
	}

	if (lockedObject) {
		LockedObjectMovement();
	}
	else {
		DebugObjectMovement();
	}

	SelectObject();
	MoveSelectedObject();

	
}

void TutorialGame::LockedObjectMovement() {
	return;

	Matrix4 view		= world->GetMainCamera()->BuildViewMatrix();
	Matrix4 camWorld	= view.Inverse();

	Vector3 rightAxis = Vector3(camWorld.GetColumn(0)); //view is inverse of model!

	//forward is more tricky -  camera forward is 'into' the screen...
	//so we can take a guess, and use the cross of straight up, and
	//the right axis, to hopefully get a vector that's good enough!

	Vector3 fwdAxis = Vector3::Cross(Vector3(0, 1, 0), rightAxis);
	fwdAxis.y = 0.0f;
	fwdAxis.Normalise();

	Vector3 charForward  = lockedObject->GetTransform().GetOrientation() * Vector3(0, 0, 1);
	Vector3 charForward2 = lockedObject->GetTransform().GetOrientation() * Vector3(0, 0, 1);

	float force = 100.0f;

	if (Window::GetKeyboard()->KeyDown(KeyboardKeys::LEFT)) {
		lockedObject->GetPhysicsObject()->AddForce(-rightAxis * force);
	}

	if (Window::GetKeyboard()->KeyDown(KeyboardKeys::RIGHT)) {
		Vector3 worldPos = selectionObject->GetTransform().GetPosition();
		lockedObject->GetPhysicsObject()->AddForce(rightAxis * force);
	}

	if (Window::GetKeyboard()->KeyDown(KeyboardKeys::UP)) {
		lockedObject->GetPhysicsObject()->AddForce(fwdAxis * force);
	}

	if (Window::GetKeyboard()->KeyDown(KeyboardKeys::DOWN)) {
		lockedObject->GetPhysicsObject()->AddForce(-fwdAxis * force);
	}

	if (Window::GetKeyboard()->KeyDown(KeyboardKeys::UP) && Window::GetKeyboard()->KeyDown(KeyboardKeys::MENU)) {
		lockedObject->GetPhysicsObject()->AddForce(Vector3(0, 10, 0));
	}
	if (Window::GetKeyboard()->KeyDown(KeyboardKeys::DOWN) && Window::GetKeyboard()->KeyDown(KeyboardKeys::MENU)) {
		lockedObject->GetPhysicsObject()->AddForce(Vector3(0, -10, 0));
	}
}

void TutorialGame::DebugObjectMovement() {
//If we've selected an object, we can manipulate it with some key presses
	if (selectionObject) {
		//Twist the selected object!
		if (Window::GetKeyboard()->KeyDown(KeyboardKeys::LEFT)) {
			selectionObject->GetPhysicsObject()->AddTorque(Vector3(-10, 0, 0));
		}

		if (Window::GetKeyboard()->KeyDown(KeyboardKeys::RIGHT)) {
			selectionObject->GetPhysicsObject()->AddTorque(Vector3(10, 0, 0));
		}

		if (Window::GetKeyboard()->KeyDown(KeyboardKeys::UP)) {
			selectionObject->GetPhysicsObject()->AddForce(Vector3(0, 0, -10));
		}

		if (Window::GetKeyboard()->KeyDown(KeyboardKeys::DOWN)) {
			selectionObject->GetPhysicsObject()->AddForce(Vector3(0, 0, 10));
		}

		if (Window::GetKeyboard()->KeyDown(KeyboardKeys::UP) && Window::GetKeyboard()->KeyDown(KeyboardKeys::MENU)) {
			selectionObject->GetPhysicsObject()->AddForce(Vector3(0, 10, 0));
		}
		if (Window::GetKeyboard()->KeyDown(KeyboardKeys::DOWN) && Window::GetKeyboard()->KeyDown(KeyboardKeys::MENU)) {
			selectionObject->GetPhysicsObject()->AddForce(Vector3(0, -10, 0));
		}
	}

}

void TutorialGame::InitCamera() {
	world->GetMainCamera()->SetNearPlane(0.1f);
	world->GetMainCamera()->SetFarPlane(5000.0f);
	/*world->GetMainCamera()->SetPitch(-15.0f);
	world->GetMainCamera()->SetYaw(315.0f);
	world->GetMainCamera()->SetPosition(Vector3(-60, 40, 60));*/
	world->GetMainCamera()->SetPitch(camInitRot.z);
	world->GetMainCamera()->SetYaw(camInitRot.y);
	world->GetMainCamera()->SetPosition(camInitPos);
	lockedObject = nullptr;
}

void TutorialGame::InitWorld() {
	world->ClearAndErase();
	physics->Clear();
	selectionObject = nullptr;

	if(gameMode) gameMode->SetupScene();

	//SliderContraintTest();
	//HingeContraintTest();

	//InitGameExamples();
	//InitDefaultFloor();
}

void TutorialGame::BridgeConstraintTest() {
	Vector3 cubeSize = Vector3(1, 1, 1);

	float invCubeMass = 5;
	int numLinks = 10;
	float maxDistance = 4;
	float cubeDistance = 3;

	Vector3 startPos = Vector3(50, 50, 50);
	GameObject* start = AddCubeToWorld(startPos + Vector3(0, 0, 0), cubeSize, 0, "BridgeStart");
	GameObject* end = AddCubeToWorld(startPos + Vector3((numLinks + 2) * cubeDistance, 0, 0), cubeSize, 0, "BridgeEnd");
	GameObject* previous = start;
	for (int i = 0; i < numLinks; i++)
	{
		GameObject* block = AddCubeToWorld(startPos + Vector3((i + 1) * cubeDistance, 0, 0), cubeSize, invCubeMass, "BridgeBlock_" + std::to_string(i));
		PositionConstraint* constraint = new PositionConstraint(previous, block, maxDistance);
		world->AddConstraint(constraint);
		previous = block;
	}
	PositionConstraint* constraint = new PositionConstraint(previous, end, maxDistance);
	world->AddConstraint(constraint);

}

void TutorialGame::SliderContraintTest()
{
	GameObject* obj_a = AddCubeToWorld(Vector3(50, 50, 50), Vector3(1, 1, 1), 0, "SliderTest_A");
	GameObject* obj_b = AddCubeToWorld(Vector3(60, 50, 50), Vector3(1, 1, 1), 5, "SliderTest_B");
	obj_b->GetPhysicsObject()->AddPhysicsChannel(PhysCh_RayCast);

	SliderConstraint* constraint = new SliderConstraint(obj_a, obj_b, Vector3(1,0,0), 5, 15);
	//HingeConstraint* constraint = new HingeConstraint(obj_a, obj_b, 1);
	world->AddConstraint(constraint);
}

void TutorialGame::HingeContraintTest()
{
	GameObject* obj_a = AddCubeToWorld_OBB(Vector3(50, 50, 50), Vector3(1, 1, 1), 10, "HingeTest_A");
	GameObject* obj_b = AddCubeToWorld_OBB(Vector3(60, 50, 50), Vector3(1, 1, 1), 10, "HingeTest_B");
	obj_a->GetPhysicsObject()->AddPhysicsChannel(PhysCh_RayCast);
	obj_b->GetPhysicsObject()->AddPhysicsChannel(PhysCh_RayCast);

	HingeConstraint* constraint = new HingeConstraint(obj_a, obj_b, 2);
	//HingeConstraint* constraint = new HingeConstraint(obj_a, obj_b, 1);
	world->AddConstraint(constraint);
}

/*

A single function to add a large immoveable cube to the bottom of our world

*/
GameObject* TutorialGame::AddFloorToWorld(const Vector3& position, const std::string& name) {
	GameObject* floor = new GameObject(name);

	Vector3 floorSize	= Vector3(100, 2, 100);
	AABBVolume* volume	= new AABBVolume(floorSize);
	floor->SetBoundingVolume((CollisionVolume*)volume);
	floor->GetTransform()
		.SetScale(floorSize * 2)
		.SetPosition(position);

	floor->SetRenderObject(new RenderObject(&floor->GetTransform(), cubeMesh, basicTex, basicShader));
	floor->SetPhysicsObject(new PhysicsObject(&floor->GetTransform(), floor->GetBoundingVolume()));

	floor->GetPhysicsObject()->SetInverseMass(0);
	floor->GetPhysicsObject()->InitCubeInertia();
	floor->GetPhysicsObject()->AddPhysicsChannel(PhysCh_Static);

	world->AddGameObject(floor);

	return floor;
}

/*

Builds a game object that uses a sphere mesh for its graphics, and a bounding sphere for its
rigid body representation. This and the cube function will let you build a lot of 'simple' 
physics worlds. You'll probably need another function for the creation of OBB cubes too.

*/
GameObject* TutorialGame::AddSphereToWorld(const Vector3& position, float radius, float inverseMass, const std::string& name) {
	GameObject* sphere = new GameObject(name);

	Vector3 sphereSize = Vector3(radius, radius, radius);
	SphereVolume* volume = new SphereVolume(radius);
	sphere->SetBoundingVolume((CollisionVolume*)volume);

	sphere->GetTransform()
		.SetScale(sphereSize)
		.SetPosition(position);

	sphere->SetRenderObject(new RenderObject(&sphere->GetTransform(), sphereMesh, basicTex, basicShader));
	sphere->SetPhysicsObject(new PhysicsObject(&sphere->GetTransform(), sphere->GetBoundingVolume()));
	sphere->GetPhysicsObject()->SetInverseMass(inverseMass);
	sphere->GetPhysicsObject()->InitSphereInertia();

	world->AddGameObject(sphere);

	return sphere;
}

GameObject* TutorialGame::AddCapsuleToWorld(const Vector3& position, float halfHeight, float radius, float inverseMass, const std::string& name) {
	GameObject* capsule = new GameObject(name);

	CapsuleVolume* volume = new CapsuleVolume(halfHeight, radius);
	capsule->SetBoundingVolume((CollisionVolume*)volume);

	capsule->GetTransform()
		.SetScale(Vector3(radius* 2, halfHeight, radius * 2))
		.SetPosition(position);

	capsule->SetRenderObject(new RenderObject(&capsule->GetTransform(), capsuleMesh, basicTex, basicShader));
	capsule->SetPhysicsObject(new PhysicsObject(&capsule->GetTransform(), capsule->GetBoundingVolume()));

	capsule->GetPhysicsObject()->SetInverseMass(inverseMass);
	capsule->GetPhysicsObject()->InitCubeInertia();

	world->AddGameObject(capsule);

	return capsule;

}

GameObject* TutorialGame::AddCubeToWorld(const Vector3& position, Vector3 dimensions, float inverseMass, const std::string& name) {
	GameObject* cube = new GameObject(name);

	AABBVolume* volume = new AABBVolume(dimensions);

	cube->SetBoundingVolume((CollisionVolume*)volume);

	cube->GetTransform()
		.SetPosition(position)
		.SetScale(dimensions * 2);

	cube->SetRenderObject(new RenderObject(&cube->GetTransform(), cubeMesh, basicTex, basicShader));
	cube->SetPhysicsObject(new PhysicsObject(&cube->GetTransform(), cube->GetBoundingVolume()));
	cube->GetPhysicsObject()->SetInverseMass(inverseMass);
	cube->GetPhysicsObject()->InitCubeInertia();

	world->AddGameObject(cube);

	return cube;
}

GameObject* TutorialGame::AddCubeToWorld_OBB(const Vector3& position, Vector3 dimensions, float inverseMass, const std::string& name)
{
	GameObject* cube = new GameObject(name);

	OBBVolume* volume = new OBBVolume(dimensions);

	cube->SetBoundingVolume((CollisionVolume*)volume);

	cube->GetTransform()
		.SetPosition(position)
		.SetScale(dimensions * 2);

	cube->SetRenderObject(new RenderObject(&cube->GetTransform(), cubeMesh, basicTex, basicShader));
	cube->SetPhysicsObject(new PhysicsObject(&cube->GetTransform(), cube->GetBoundingVolume()));

	cube->GetPhysicsObject()->SetInverseMass(inverseMass);
	cube->GetPhysicsObject()->InitCubeInertia();

	world->AddGameObject(cube);

	return cube;
}

void TutorialGame::InitSphereGridWorld(int numRows, int numCols, float rowSpacing, float colSpacing, float radius) {
	for (int x = 0; x < numCols; ++x) {
		for (int z = 0; z < numRows; ++z) {
			Vector3 position = Vector3(x * colSpacing, 10.0f, z * rowSpacing);
			AddSphereToWorld(position, radius, 1.0f);
		}
	}
	AddFloorToWorld(Vector3(0, -2, 0), "SphereGridFloor");
}

void TutorialGame::InitMixedGridWorld(int numRows, int numCols, float rowSpacing, float colSpacing) {
	float sphereRadius = 1.0f;
	Vector3 cubeDims = Vector3(1, 1, 1);

	for (int x = 0; x < numCols; ++x) {
		for (int z = 0; z < numRows; ++z) {
			Vector3 position = Vector3(x * colSpacing, 10.0f, z * rowSpacing);

			if (rand() % 2) {
				AddCubeToWorld(position, cubeDims, 10.0f, "Cube_"+ std::to_string(x * numRows + z));
			}
			else {
				AddSphereToWorld(position, sphereRadius, 10.0f, "Sphere_" + std::to_string(x * numRows + z));
			}
		}
	}
}

void TutorialGame::InitCubeGridWorld(int numRows, int numCols, float rowSpacing, float colSpacing, const Vector3& cubeDims) {
	for (int x = 1; x < numCols+1; ++x) {
		for (int z = 1; z < numRows+1; ++z) {
			Vector3 position = Vector3(x * colSpacing, 10.0f, z * rowSpacing);
			AddCubeToWorld(position, cubeDims, 1.0f);
		}
	}
}

void TutorialGame::InitDefaultFloor() {
	AddFloorToWorld(Vector3(0, -2, 0), "DefaultFloor");
}

void TutorialGame::InitGameExamples() {
	
	//InitMixedGridWorld(5, 5, 3.5f, 3.5f);

	//AddPlayerToWorld(Vector3(0, 5, 0), "DefaultPlayer");
	//AddEnemyToWorld(Vector3(5, 5, 0), "DefaultEnemy");
	//AddBonusToWorld(Vector3(10, 5, 0), "Bonus");
	//AddCapsuleToWorld(Vector3(15, 5, 0), 2, 1, 10.0f, "TestCapsule");
	//AddSphereToWorld(Vector3(20, 5, 0), 1, 10.0f, "TestSphere");

	//BridgeConstraintTest();

	AddCubeToWorld_OBB(Vector3(-5, 20, -10), Vector3(1, 1, 1), 10.0f, "TestCollision_OBB_1")->GetPhysicsObject()->AddPhysicsChannel(PhysCh_RayCast);
	AddCubeToWorld_OBB(Vector3(0, 20, -10), Vector3(1, 1, 1), 10.0f, "TestCollision_OBB")->GetPhysicsObject()->AddPhysicsChannel(PhysCh_RayCast);
	//AddSphereToWorld(Vector3(5, 20, -10), 1.0f, 10.0f, "TestCollision_Sphere");
	//AddCubeToWorld(Vector3(0, 20, -5), Vector3(1, 1, 1), 10.0f, "TestCollision_AABB");

	//AddStateObjectToWorld(Vector3(50, 10, 0), "TestStateObject");

	//AddCubeToWorld_OBB(Vector3(0, 0, -5), Vector3(1, 5, 1), 0.0f, "TestStaticChannel_OBB")->GetPhysicsObject()->AddPhysicsChannel(PhysCh_Static);
	
}

GameObject* TutorialGame::AddPlayerToWorld(const Vector3& position, const std::string& name) {
	float meshSize = 3.0f;
	float inverseMass = 0.5f;

	GameObject* character = new GameObject(name);

	AABBVolume* volume = new AABBVolume(Vector3(0.3f, 0.85f, 0.3f) * meshSize);

	character->SetBoundingVolume((CollisionVolume*)volume);

	character->GetTransform()
		.SetScale(Vector3(meshSize, meshSize, meshSize))
		.SetPosition(position);

	if (rand() % 2) {
		character->SetRenderObject(new RenderObject(&character->GetTransform(), charMeshA, nullptr, basicShader));
	}
	else {
		character->SetRenderObject(new RenderObject(&character->GetTransform(), charMeshB, nullptr, basicShader));
	}
	character->SetPhysicsObject(new PhysicsObject(&character->GetTransform(), character->GetBoundingVolume()));

	character->GetPhysicsObject()->SetInverseMass(inverseMass);
	character->GetPhysicsObject()->InitSphereInertia();

	world->AddGameObject(character);

	//lockedObject = character;

	return character;
}

GameObject* TutorialGame::AddEnemyToWorld(const Vector3& position, const std::string& name) {
	float meshSize		= 3.0f;
	float inverseMass	= 0.5f;

	GameObject* character = new GameObject(name);

	AABBVolume* volume = new AABBVolume(Vector3(0.3f, 0.9f, 0.3f) * meshSize);
	character->SetBoundingVolume((CollisionVolume*)volume);

	character->GetTransform()
		.SetScale(Vector3(meshSize, meshSize, meshSize))
		.SetPosition(position);

	character->SetRenderObject(new RenderObject(&character->GetTransform(), enemyMesh, nullptr, basicShader));
	character->SetPhysicsObject(new PhysicsObject(&character->GetTransform(), character->GetBoundingVolume()));

	character->GetPhysicsObject()->SetInverseMass(inverseMass);
	character->GetPhysicsObject()->InitSphereInertia();

	world->AddGameObject(character);

	return character;
}

GameObject* TutorialGame::AddBonusToWorld(const Vector3& position, const std::string& name) {
	GameObject* apple = new GameObject(name);

	SphereVolume* volume = new SphereVolume(0.25f);
	apple->SetBoundingVolume((CollisionVolume*)volume);
	apple->GetTransform()
		.SetScale(Vector3(0.25, 0.25, 0.25))
		.SetPosition(position);

	apple->SetRenderObject(new RenderObject(&apple->GetTransform(), bonusMesh, nullptr, basicShader));
	apple->SetPhysicsObject(new PhysicsObject(&apple->GetTransform(), apple->GetBoundingVolume()));

	apple->GetPhysicsObject()->SetInverseMass(1.0f);
	apple->GetPhysicsObject()->InitSphereInertia();

	world->AddGameObject(apple);

	return apple;
}

GameObject* TutorialGame::AddFloorToWorld(const Vector3& position, Vector3 dimensions, Vector4 Colour, const std::string& name)
{
	GameObject* floor = new GameObject(name);
	AABBVolume* volume = new AABBVolume(dimensions);
	floor->SetBoundingVolume((CollisionVolume*)volume);
	floor->GetTransform()
		.SetScale(dimensions * 2)
		.SetPosition(position);
	
	TextureBase* tex = Colour.w ? nullptr : basicTex;
	floor->SetRenderObject(new RenderObject(&floor->GetTransform(), cubeMesh, tex, basicShader));
	floor->GetRenderObject()->SetColour(Colour);

	floor->SetPhysicsObject(new PhysicsObject(&floor->GetTransform(), floor->GetBoundingVolume()));

	floor->GetPhysicsObject()->SetInverseMass(0);
	floor->GetPhysicsObject()->InitCubeInertia();
	floor->GetPhysicsObject()->AddPhysicsChannel(PhysCh_Static);

	world->AddGameObject(floor);

	return floor;
}

GameObject* TutorialGame::AddWallToWorld(const Vector3& position, Vector3 dimensions, Vector4 Colour, const std::string& name)
{
	GameObject* wall = new GameObject(name);
	OBBVolume* volume = new OBBVolume(dimensions);
	wall->SetBoundingVolume((CollisionVolume*)volume);
	wall->GetTransform()
		.SetPosition(position)
		.SetScale(dimensions * 2);

	TextureBase* tex = Colour.w ? nullptr : basicTex;
	wall->SetRenderObject(new RenderObject(&wall->GetTransform(), cubeMesh, tex, basicShader));
	wall->GetRenderObject()->SetColour(Colour);
	wall->SetPhysicsObject(new PhysicsObject(&wall->GetTransform(), wall->GetBoundingVolume()));

	wall->GetPhysicsObject()->SetInverseMass(0);
	wall->GetPhysicsObject()->InitCubeInertia();
	wall->GetPhysicsObject()->AddPhysicsChannel(PhysCh_Static);
	wall->GetPhysicsObject()->SetElasticity(0.8);
	wall->GetPhysicsObject()->SetFriction(0.1);

	world->AddGameObject(wall);

	return wall;
}

GameObject* TutorialGame::AddAirWallToWorld(const Vector3& position, Vector3 dimensions, const std::string& name)
{
	GameObject* wall = new GameObject(name);
	AABBVolume* volume = new AABBVolume(dimensions);
	wall->SetBoundingVolume((CollisionVolume*)volume);
	wall->GetTransform()
		.SetPosition(position)
		.SetScale(dimensions * 2);

	wall->SetPhysicsObject(new PhysicsObject(&wall->GetTransform(), wall->GetBoundingVolume()));

	wall->GetPhysicsObject()->SetInverseMass(0);
	wall->GetPhysicsObject()->InitCubeInertia();
	wall->GetPhysicsObject()->SetPhysicsChannel(PhysCh_Static | PhysCh_AirWall);
	wall->GetPhysicsObject()->SetElasticity(1);
	wall->GetPhysicsObject()->SetFriction(0);

	world->AddGameObject(wall);

	return wall;
}

StateGameObject* TutorialGame::AddStateObjectToWorld(const Vector3& position, const std::string& name)
{
	StateGameObject* object = new StateGameObject(name);

	SphereVolume* volume = new SphereVolume(1.0f);
	object->SetBoundingVolume((CollisionVolume*)volume);
	object->GetTransform()
		.SetScale(Vector3(1,1,1))
		.SetPosition(position);

	object->SetRenderObject(new RenderObject(&object->GetTransform(), sphereMesh, nullptr, basicShader));
	object->SetPhysicsObject(new PhysicsObject(&object->GetTransform(), object->GetBoundingVolume()));

	object->GetPhysicsObject()->SetInverseMass(1.0f);
	object->GetPhysicsObject()->InitSphereInertia();

	world->AddGameObject(object);

	return object;
}

/*

Every frame, this code will let you perform a raycast, to see if there's an object
underneath the cursor, and if so 'select it' into a pointer, so that it can be 
manipulated later. Pressing Q will let you toggle between this behaviour and instead
letting you move the camera around. 

*/
bool TutorialGame::SelectObject() {
	if (Window::GetMouse()->ButtonDown(NCL::MouseButtons::LEFT)) {
		if (selectionObject) {	//set colour to deselected;
			selectionObject->GetRenderObject()->SetColour(Vector4(1, 1, 1, 1));
			selectionObject = nullptr;
			lockedObject	= nullptr;
		}

		Ray ray = CollisionDetection::BuildRayFromMouse(*world->GetMainCamera());

		RayCollision closestCollision;
		//if (world->Raycast(ray, closestCollision, true)) 
		if (physics->Raycast(ray, closestCollision, true))
		{
			selectionObject = (GameObject*)closestCollision.node;
			selectionObject->GetRenderObject()->SetColour(Vector4(0, 1, 0, 1));
			return true;
		}
		else {
			return false;
		}
	}

	if (Window::GetKeyboard()->KeyPressed(NCL::KeyboardKeys::L)) {
		if (selectionObject) {
			if (lockedObject == selectionObject) {
				lockedObject = nullptr;
			}
			else {
				lockedObject = selectionObject;
			}
		}

	}

	return false;
}

/*
If an object has been clicked, it can be pushed with the right mouse button, by an amount
determined by the scroll wheel. In the first tutorial this won't do anything, as we haven't
added linear motion into our physics system. After the second tutorial, objects will move in a straight
line - after the third, they'll be able to twist under torque aswell.
*/
void TutorialGame::MoveSelectedObject() {
	if (!selectionObject)
	{
		return;
	}
	if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::SPACE))
	{
		Ray ray = CollisionDetection::BuildRayFromMouse(*world->GetMainCamera());
		RayCollision closestCollision;
		if (world->Raycast(ray, closestCollision, true))
		{
			if (closestCollision.node == selectionObject)
			{
				const CollisionVolume* volume = selectionObject->GetBoundingVolume();
				if (volume->type == VolumeType::AABB)
					selectionObject->GetPhysicsObject()->AddForce(ray.GetDirection() * forceMagnitude);
				else
					selectionObject->GetPhysicsObject()->AddForceAtPosition(ray.GetDirection() * forceMagnitude, closestCollision.collidedAt);	
			}
		}
	}
}