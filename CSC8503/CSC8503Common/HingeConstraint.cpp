#include "HingeConstraint.h"
#include "GameObject.h"

using namespace NCL;
using namespace CSC8503;
using namespace Maths;

void HingeConstraint::UpdateConstraint(float dt)
{
	Quaternion orientationA = objectA->GetTransform().GetOrientation();
	Quaternion orientationB = objectB->GetTransform().GetOrientation();
	Matrix3 rotMatA = Matrix3(orientationA);
	Matrix3 rotMatB = Matrix3(orientationB);
	Vector3 w1 = rotMatA.GetColumn(axisIndex);
	Vector3 w2 = rotMatB.GetColumn(axisIndex);

	PhysicsObject* physA = objectA->GetPhysicsObject();
	PhysicsObject* physB = objectB->GetPhysicsObject();
	Vector3 rAngularVelocity = physA->GetAngularVelocity() - physB->GetAngularVelocity();


	float C = Vector3::Dot(rAngularVelocity, w1);

	if (abs(C) > 0.0f)
	{
		

		Vector3 J = Vector3::Cross(rAngularVelocity, w1).Normalised();
		
		
		//Vector3 rAngularVelocity = physB->GetAngularVelocity() - physA->GetAngularVelocity();
		float dC = Vector3::Dot(J, rAngularVelocity);

		printf("C1: %f, dC1: %f, dt: %f\n", C, dC, dt);

		Matrix3 invInertia = physA->GetInertiaTensor() + physB->GetInertiaTensor();
		float K = Vector3::Dot(J, invInertia * J);
		float biasFactor = 0.01f;
		float lambda = -(dC + biasFactor * C / dt) / K;

		Vector3 aImpulse = -J * lambda;
		Vector3 bImpulse = J * lambda;
		physA->ApplyAngularImpulse(aImpulse);
		physB->ApplyAngularImpulse(bImpulse);
	}

}