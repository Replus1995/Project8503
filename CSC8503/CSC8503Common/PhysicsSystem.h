#pragma once
#include "../CSC8503Common/GameWorld.h"
#include <set>
#include <memory>
#include "Octree.h";

namespace NCL {
	namespace CSC8503 {
		class PhysicsSystem	{
		public:
			PhysicsSystem(GameWorld& g);
			~PhysicsSystem();

			void Clear();

			void Update(float dt);

			void UseGravity(bool state) {
				applyGravity = state;
			}

			float GetGlobalDamping() const { return globalDamping; };
			void SetGlobalDamping(float d) { globalDamping = d; };

			void SetGravity(const Vector3& g);

			void BuildSpaceTree();

			bool Raycast(Ray& r, RayCollision& closestCollision, bool closestObject, PhysicsChannel channel = PhysCh_RayCast) const;

			float GetPenaltyScale() const { return penaltyScale; };
			void SetPenaltyScale(float s) { penaltyScale = s; };

		protected:
			void BasicCollisionDetection();
			void BroadPhase();
			void NarrowPhase();

			void ClearForces();

			void IntegrateAccel(float dt);
			void IntegrateVelocity(float dt);

			void UpdateConstraints(float dt);

			void UpdateCollisionList();
			void UpdateObjectAABBs();

			void ImpulseResolveCollision(GameObject& a , GameObject&b, CollisionDetection::ContactPoint& p) const;
			static Vector3 GetImpulseFriction(PhysicsObject* physObj, const Vector3& normal, const Vector3& v, float m, float j);

			void ProjectionResolveCollision(GameObject& a, GameObject& b, CollisionDetection::ContactPoint& p) const;
			void PenaltyResolveCollision(GameObject& a, GameObject& b, CollisionDetection::ContactPoint& p) const;


			GameWorld& gameWorld;

			bool	applyGravity;
			Vector3 gravity;
			float	dTOffset;
			float	globalDamping;
			float	penaltyScale;

			std::set<CollisionDetection::CollisionInfo> allCollisions;
			std::set<CollisionDetection::CollisionInfo> broadphaseCollisions;

			bool useBroadPhase		= true;
			int numCollisionFrames	= 5;

			std::unique_ptr<OcTree<GameObject*>> spaceOctree;
		};
	}
}

