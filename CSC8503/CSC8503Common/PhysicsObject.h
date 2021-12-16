#pragma once
#include "../../Common/Vector3.h"
#include "../../Common/Matrix3.h"

using namespace NCL::Maths;

namespace NCL {
	class CollisionVolume;
	namespace CSC8503 {
		class Transform;
		enum PhysicsChannel : unsigned int
		{
			PhysCh_Static = 1,
			PhysCh_Dynamic = 1 << 1,
			PhysCh_RayCast = 1 << 2,
			PhysCh_AirWall = 1 << 3,
			PhysCh_EventOnly = 1 << 4,
			PhysCh_NoForce = 1 << 5,
		};

		enum CollisionMethod
		{
			CM_Impulse,
			CM_Projection,
			CM_Penalty
		};


		class PhysicsObject	{
		public:
			PhysicsObject(Transform* parentTransform, const CollisionVolume* parentVolume);
			~PhysicsObject();

			Vector3 GetLinearVelocity() const { return linearVelocity; };
			Vector3 GetAngularVelocity() const { return angularVelocity; };

			Vector3 GetTorque() const { return torque; };
			Vector3 GetForce() const { return force; };

			void SetInverseMass(float invMass) { inverseMass = invMass; };
			float GetInverseMass() const { return inverseMass; };
			void SetElasticity(float inElasticity) { elasticity = inElasticity; };
			float GetElasticity() const { return elasticity; };
			void SetFriction(float inFriction) { friction = inFriction; };
			float GetFriction() const { return friction; };

			void UpdatePhysicsScales(float invMass, float inElasticity, float inFriction)
			{
				inverseMass = invMass;
				elasticity = inElasticity;
				friction = inFriction;
			}

			void ApplyAngularImpulse(const Vector3& force);
			void ApplyLinearImpulse(const Vector3& force);
			
			void AddForce(const Vector3& force);

			void AddForceAtPosition(const Vector3& force, const Vector3& position);

			void AddTorque(const Vector3& torque);


			void ClearForces();

			void SetLinearVelocity(const Vector3& v) {
				linearVelocity = v;
			}

			void SetAngularVelocity(const Vector3& v) {
				angularVelocity = v;
			}

			void InitCubeInertia();
			void InitSphereInertia();
			Vector3 GetInertia() { return inverseInertia; };

			void UpdateInertiaTensor();

			Matrix3 GetInertiaTensor() const {
				return inverseInteriaTensor;
			}

			unsigned int GetPhysicsChannel() const {
				return physicsChannel;
			}
			void SetPhysicsChannel(unsigned int newChannel) {
				physicsChannel = newChannel;
			}
			void AddPhysicsChannel(PhysicsChannel newChannel) {
				physicsChannel |= newChannel;
			}
			bool CanCollide(PhysicsObject* b) const;
			bool HasChannel(PhysicsChannel channel) const;

			CollisionMethod GetCollisionMethod() const { return cMethod; };
			void SetCollisionMethod(CollisionMethod method) { cMethod = method; };


		protected:
			const CollisionVolume* volume;
			Transform*		transform;

			float inverseMass;
			float elasticity;
			float friction;

			//linear stuff
			Vector3 linearVelocity;
			Vector3 force;
			

			//angular stuff
			Vector3 angularVelocity;
			Vector3 torque;
			Vector3 inverseInertia;
			Matrix3 inverseInteriaTensor;

			//channel
			unsigned int physicsChannel;
			//collision
			CollisionMethod cMethod = CM_Impulse;

		};
	}
}

