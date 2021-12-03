#include "CollisionDetection.h"
#include "CollisionVolume.h"
#include "AABBVolume.h"
#include "OBBVolume.h"
#include "SphereVolume.h"
#include "../../Common/Vector2.h"
#include "../../Common/Window.h"
#include "../../Common/Maths.h"
#include "Debug.h"

#include <list>

using namespace NCL;

bool CollisionDetection::RayPlaneIntersection(const Ray&r, const Plane&p, RayCollision& collisions) {
	float ln = Vector3::Dot(p.GetNormal(), r.GetDirection());

	if (ln == 0.0f) {
		return false; //direction vectors are perpendicular!
	}
	
	Vector3 planePoint = p.GetPointOnPlane();

	Vector3 pointDir = planePoint - r.GetPosition();

	float d = Vector3::Dot(pointDir, p.GetNormal()) / ln;

	collisions.collidedAt = r.GetPosition() + (r.GetDirection() * d);

	return true;
}

bool CollisionDetection::RayIntersection(const Ray& r,GameObject& object, RayCollision& collision) {
	bool hasCollided = false;

	const Transform& worldTransform = object.GetTransform();
	const CollisionVolume* volume	= object.GetBoundingVolume();

	if (!volume) {
		return false;
	}

	switch (volume->type) {
		case VolumeType::AABB:		hasCollided = RayAABBIntersection(r, worldTransform, (const AABBVolume&)*volume, collision); break;
		case VolumeType::OBB:		hasCollided = RayOBBIntersection(r, worldTransform, (const OBBVolume&)*volume, collision); break;
		case VolumeType::Sphere:	hasCollided = RaySphereIntersection(r, worldTransform, (const SphereVolume&)*volume, collision); break;
		case VolumeType::Capsule:	hasCollided = RayCapsuleIntersection(r, worldTransform, (const CapsuleVolume&)*volume, collision); break;
	}

	return hasCollided;
}

bool CollisionDetection::RayBoxIntersection(const Ray&r, const Vector3& boxPos, const Vector3& boxSize, RayCollision& collision) {
	Vector3 boxMin = boxPos - boxSize;
	Vector3 boxMax = boxPos + boxSize;
	Vector3 rayPos = r.GetPosition();
	Vector3 rayDir = r.GetDirection();
	Vector3 tVals(-1, -1, -1);
	for (int i = 0; i < 3; i++)
	{
		if (rayDir[i] > 0)
		{
			tVals[i] = (boxMin[i] - rayPos[i]) / rayDir[i];
		}
		else if (rayDir[i] < 0)
		{
			tVals[i] = (boxMax[i] - rayPos[i]) / rayDir[i];
		}
	}
	float bestT = tVals.GetMaxElement();
	if (bestT < 0.0f)
	{
		return false;
	}
	Vector3 intersection = rayPos + (rayDir * bestT);
	const float epsilon = 0.0001f;
	for (int i = 0; i < 3; i++)
	{
		if (intersection[i] + epsilon < boxMin[i] || intersection[i] - epsilon > boxMax[i])
		{
			return false;
		}
	}
	collision.collidedAt = intersection;
	collision.rayDistance = bestT;
	return true;
}

int CollisionDetection::RayCylinderIntersection(const Ray& r, const float halfHeight, const float radius, RayCollision& collision)
{
	Vector3 rayPos = r.GetPosition();
	Vector3 rayDir = r.GetDirection();
	float a = rayDir.x * rayDir.x + rayDir.z * rayDir.z;
	float b = 2 * rayPos.x * rayDir.x + 2 * rayPos.z * rayDir.z;
	float c = rayPos.x * rayPos.x + rayPos.z * rayPos.z - radius * radius;
	float hMax = halfHeight;
	float hMin = halfHeight * -1;
	if (a == 0)
	{
		if (c <= 0)
		{
			float posY = Maths::Clamp(rayPos.y, hMin, hMax);
			collision.collidedAt = Vector3(rayPos.x, posY, rayPos.z);
			collision.rayDistance = abs(rayPos.y - posY);
			return 0;
		}
		else
		{
			return -2;
		}
	}

	float b2_4ac = b * b - 4 * a * c;
	if (b2_4ac < 0)
	{
		return -2;
	}

	std::vector<float> validTs;

	float t1 = (-1 * b + sqrt(b2_4ac)) / (2 * a);
	float t2 = (-1 * b - sqrt(b2_4ac)) / (2 * a);
	float h1 = rayPos.y + t1 * rayDir.y;
	float h2 = rayPos.y + t2 * rayDir.y;

	if (h1 >= hMin && h1 <= hMax && t1 >= 0)
	{
		validTs.push_back(t1);
	}
	if (h2 >= hMin && h2 <= hMax && t2 >= 0)
	{
		validTs.push_back(t2);
	}

	float tTop = (hMax - rayPos.y) / rayDir.y;
	Vector2 xzTop(rayPos.x + tTop * rayDir.x, rayPos.z + tTop * rayDir.z);
	if (tTop >= 0 && xzTop.Length() < radius)
	{
		validTs.push_back(tTop);
	}

	float tBottom = (hMin - rayPos.y) / rayDir.y;
	Vector2 xzBottom(rayPos.x + tBottom * rayDir.x, rayPos.z + tBottom * rayDir.z);
	if (tBottom >= 0 && xzBottom.Length() < radius)
	{
		validTs.push_back(tBottom);
	}

	if (validTs.empty())
	{
		if (h1 > hMax && h2 > hMax)
		{
			return 1;
		}
		else if (h1 < hMin && h2 < hMin)
		{
			return -1;
		}
		else
		{
			return -2;
		}
	}

	float tMin = FLT_MAX;
	for (size_t i = 0; i < validTs.size(); i++)
	{
		if (validTs[i] < tMin)
		{
			tMin = validTs[i];
		}
	}

	collision.collidedAt = rayPos + rayDir * tMin;
	collision.rayDistance = tMin;

	return 0;
}

bool CollisionDetection::RayAABBIntersection(const Ray&r, const Transform& worldTransform, const AABBVolume& volume, RayCollision& collision) {
	Vector3 boxPos = worldTransform.GetPosition();
	Vector3 boxSize = volume.GetHalfDimensions();
	return RayBoxIntersection(r, boxPos, boxSize, collision);
}

bool CollisionDetection::RayOBBIntersection(const Ray&r, const Transform& worldTransform, const OBBVolume& volume, RayCollision& collision) {
	Quaternion orientation = worldTransform.GetOrientation();
	Vector3 position = worldTransform.GetPosition();
	Matrix3 transform = Matrix3(orientation);
	Matrix3 invTranfrom = Matrix3(orientation.Conjugate());
	Vector3 localRayPos = r.GetPosition() - position;
	Ray tempRay(invTranfrom * localRayPos, invTranfrom * r.GetDirection());
	bool collided = RayBoxIntersection(tempRay, Vector3(), volume.GetHalfDimensions(), collision);
	if (collided)
	{
		collision.collidedAt = transform * collision.collidedAt + position;
	}
	return collided;
}

bool CollisionDetection::RayCapsuleIntersection(const Ray& r, const Transform& worldTransform, const CapsuleVolume& volume, RayCollision& collision) {
	
	float cylinderHalfHeight = volume.GetHalfHeight() - volume.GetRadius();

	Quaternion orientation = worldTransform.GetOrientation();
	Vector3 position = worldTransform.GetPosition();
	Matrix3 transform = Matrix3(orientation);
	Matrix3 invTranfrom = Matrix3(orientation.Conjugate());
	Vector3 localRayPos = r.GetPosition() - position;
	Ray tempRay(invTranfrom * localRayPos, invTranfrom * r.GetDirection());
	int cylinderCollided = RayCylinderIntersection(tempRay, cylinderHalfHeight, volume.GetRadius(), collision);
	if (cylinderCollided == 0)
	{
		float collisionRadius = sqrt(collision.collidedAt.x * collision.collidedAt.x + collision.collidedAt.z * collision.collidedAt.z);
		if (collision.collidedAt.y < cylinderHalfHeight && collision.collidedAt.y > -cylinderHalfHeight 
			|| collisionRadius == volume.GetRadius())
		{
			collision.collidedAt = transform * collision.collidedAt + position;
			return true;
		}
	}
	else if (cylinderCollided < -1)
	{
		return false;
	}

	Vector3 up = transform.GetColumn(1);
	SphereVolume sphereVolume(volume.GetRadius());
	Transform sphereTranform = worldTransform;

	if (cylinderCollided == 0)
	{
		sphereTranform.SetPosition(position + up * collision.collidedAt.y);
		return RaySphereIntersection(r, sphereTranform, sphereVolume, collision);
	}
	else
	{
		sphereTranform.SetPosition(position + up * cylinderHalfHeight * cylinderCollided);
		return RaySphereIntersection(r, sphereTranform, sphereVolume, collision);
	}
}

bool CollisionDetection::RaySphereIntersection(const Ray&r, const Transform& worldTransform, const SphereVolume& volume, RayCollision& collision) {
	Vector3 spherePos = worldTransform.GetPosition();
	float sphereRadius = volume.GetRadius();
	Vector3 dir = (spherePos - r.GetPosition());
	float sphereProj = Vector3::Dot(dir, r.GetDirection());
	if (sphereProj < 0.0f)
	{
		return false;
	}
	Vector3 point = r.GetPosition() + (r.GetDirection() * sphereProj);
	float sphereDist = (point - spherePos).Length();
	if (sphereDist > sphereRadius)
	{
		return false;
	}
	float offset = sqrt((sphereRadius * sphereRadius) - (sphereDist * sphereDist));
	collision.rayDistance = sphereProj - offset;
	collision.collidedAt = r.GetPosition() + (r.GetDirection() * collision.rayDistance);
	return true;
}

Matrix4 GenerateInverseView(const Camera &c) {
	float pitch = c.GetPitch();
	float yaw	= c.GetYaw();
	Vector3 position = c.GetPosition();

	Matrix4 iview =
		Matrix4::Translation(position) *
		Matrix4::Rotation(-yaw, Vector3(0, -1, 0)) *
		Matrix4::Rotation(-pitch, Vector3(-1, 0, 0));

	return iview;
}

Vector3 CollisionDetection::Unproject(const Vector3& screenPos, const Camera& cam) {
	Vector2 screenSize = Window::GetWindow()->GetScreenSize();

	float aspect	= screenSize.x / screenSize.y;
	float fov		= cam.GetFieldOfVision();
	float nearPlane = cam.GetNearPlane();
	float farPlane  = cam.GetFarPlane();

	//Create our inverted matrix! Note how that to get a correct inverse matrix,
	//the order of matrices used to form it are inverted, too.
	Matrix4 invVP = GenerateInverseView(cam) * GenerateInverseProjection(aspect, fov, nearPlane, farPlane);

	//Our mouse position x and y values are in 0 to screen dimensions range,
	//so we need to turn them into the -1 to 1 axis range of clip space.
	//We can do that by dividing the mouse values by the width and height of the
	//screen (giving us a range of 0.0 to 1.0), multiplying by 2 (0.0 to 2.0)
	//and then subtracting 1 (-1.0 to 1.0).
	Vector4 clipSpace = Vector4(
		(screenPos.x / (float)screenSize.x) * 2.0f - 1.0f,
		(screenPos.y / (float)screenSize.y) * 2.0f - 1.0f,
		(screenPos.z),
		1.0f
	);

	//Then, we multiply our clipspace coordinate by our inverted matrix
	Vector4 transformed = invVP * clipSpace;

	//our transformed w coordinate is now the 'inverse' perspective divide, so
	//we can reconstruct the final world space by dividing x,y,and z by w.
	return Vector3(transformed.x / transformed.w, transformed.y / transformed.w, transformed.z / transformed.w);
}

Ray CollisionDetection::BuildRayFromMouse(const Camera& cam) {
	Vector2 screenMouse = Window::GetMouse()->GetAbsolutePosition();
	Vector2 screenSize	= Window::GetWindow()->GetScreenSize();

	//We remove the y axis mouse position from height as OpenGL is 'upside down',
	//and thinks the bottom left is the origin, instead of the top left!
	Vector3 nearPos = Vector3(screenMouse.x,
		screenSize.y - screenMouse.y,
		-0.99999f
	);

	//We also don't use exactly 1.0 (the normalised 'end' of the far plane) as this
	//causes the unproject function to go a bit weird. 
	Vector3 farPos = Vector3(screenMouse.x,
		screenSize.y - screenMouse.y,
		0.99999f
	);

	Vector3 a = Unproject(nearPos, cam);
	Vector3 b = Unproject(farPos, cam);
	Vector3 c = b - a;

	c.Normalise();

	//std::cout << "Ray Direction:" << c << std::endl;

	return Ray(cam.GetPosition(), c);
}

//http://bookofhook.com/mousepick.pdf
Matrix4 CollisionDetection::GenerateInverseProjection(float aspect, float fov, float nearPlane, float farPlane) {
	Matrix4 m;

	float t = tan(fov*PI_OVER_360);

	float neg_depth = nearPlane - farPlane;

	const float h = 1.0f / t;

	float c = (farPlane + nearPlane) / neg_depth;
	float e = -1.0f;
	float d = 2.0f*(nearPlane*farPlane) / neg_depth;

	m.array[0]  = aspect / h;
	m.array[5]  = tan(fov*PI_OVER_360);

	m.array[10] = 0.0f;
	m.array[11] = 1.0f / d;

	m.array[14] = 1.0f / e;

	m.array[15] = -c / (d*e);

	return m;
}

/*
And here's how we generate an inverse view matrix. It's pretty much
an exact inversion of the BuildViewMatrix function of the Camera class!
*/
Matrix4 CollisionDetection::GenerateInverseView(const Camera &c) {
	float pitch = c.GetPitch();
	float yaw	= c.GetYaw();
	Vector3 position = c.GetPosition();

	Matrix4 iview =
Matrix4::Translation(position) *
Matrix4::Rotation(yaw, Vector3(0, 1, 0)) *
Matrix4::Rotation(pitch, Vector3(1, 0, 0));

return iview;
}


/*
If you've read through the Deferred Rendering tutorial you should have a pretty
good idea what this function does. It takes a 2D position, such as the mouse
position, and 'unprojects' it, to generate a 3D world space position for it.

Just as we turn a world space position into a clip space position by multiplying
it by the model, view, and projection matrices, we can turn a clip space
position back to a 3D position by multiply it by the INVERSE of the
view projection matrix (the model matrix has already been assumed to have
'transformed' the 2D point). As has been mentioned a few times, inverting a
matrix is not a nice operation, either to understand or code. But! We can cheat
the inversion process again, just like we do when we create a view matrix using
the camera.

So, to form the inverted matrix, we need the aspect and fov used to create the
projection matrix of our scene, and the camera used to form the view matrix.

*/
Vector3	CollisionDetection::UnprojectScreenPosition(Vector3 position, float aspect, float fov, const Camera &c) {
	//Create our inverted matrix! Note how that to get a correct inverse matrix,
	//the order of matrices used to form it are inverted, too.
	Matrix4 invVP = GenerateInverseView(c) * GenerateInverseProjection(aspect, fov, c.GetNearPlane(), c.GetFarPlane());

	Vector2 screenSize = Window::GetWindow()->GetScreenSize();

	//Our mouse position x and y values are in 0 to screen dimensions range,
	//so we need to turn them into the -1 to 1 axis range of clip space.
	//We can do that by dividing the mouse values by the width and height of the
	//screen (giving us a range of 0.0 to 1.0), multiplying by 2 (0.0 to 2.0)
	//and then subtracting 1 (-1.0 to 1.0).
	Vector4 clipSpace = Vector4(
		(position.x / (float)screenSize.x) * 2.0f - 1.0f,
		(position.y / (float)screenSize.y) * 2.0f - 1.0f,
		(position.z) - 1.0f,
		1.0f
	);

	//Then, we multiply our clipspace coordinate by our inverted matrix
	Vector4 transformed = invVP * clipSpace;

	//our transformed w coordinate is now the 'inverse' perspective divide, so
	//we can reconstruct the final world space by dividing x,y,and z by w.
	return Vector3(transformed.x / transformed.w, transformed.y / transformed.w, transformed.z / transformed.w);
}

bool CollisionDetection::ObjectIntersection(GameObject* a, GameObject* b, CollisionInfo& collisionInfo) {
	const CollisionVolume* volA = a->GetBoundingVolume();
	const CollisionVolume* volB = b->GetBoundingVolume();

	if (!volA || !volB) {
		return false;
	}

	collisionInfo.a = a;
	collisionInfo.b = b;

	Transform& transformA = a->GetTransform();
	Transform& transformB = b->GetTransform();

	VolumeType pairType = (VolumeType)((int)volA->type | (int)volB->type);

	if (pairType == VolumeType::AABB) {
		return AABBIntersection((AABBVolume&)*volA, transformA, (AABBVolume&)*volB, transformB, collisionInfo);
	}

	if (pairType == VolumeType::Sphere) {
		return SphereIntersection((SphereVolume&)*volA, transformA, (SphereVolume&)*volB, transformB, collisionInfo);
	}

	if (pairType == VolumeType::OBB) {
		return OBBIntersection((OBBVolume&)*volA, transformA, (OBBVolume&)*volB, transformB, collisionInfo);
	}

	if (volA->type == VolumeType::AABB && volB->type == VolumeType::Sphere) {
		return AABBSphereIntersection((AABBVolume&)*volA, transformA, (SphereVolume&)*volB, transformB, collisionInfo);
	}
	if (volA->type == VolumeType::Sphere && volB->type == VolumeType::AABB) {
		collisionInfo.a = b;
		collisionInfo.b = a;
		return AABBSphereIntersection((AABBVolume&)*volB, transformB, (SphereVolume&)*volA, transformA, collisionInfo);
	}
	if (volA->type == VolumeType::OBB && volB->type == VolumeType::Sphere) {
		return OBBSphereIntersection((OBBVolume&)*volA, transformA, (SphereVolume&)*volB, transformB, collisionInfo);
	}
	if (volA->type == VolumeType::Sphere && volB->type == VolumeType::OBB) {
		collisionInfo.a = b;
		collisionInfo.b = a;
		return OBBSphereIntersection((OBBVolume&)*volB, transformB, (SphereVolume&)*volA, transformA, collisionInfo);
	}

	if (volA->type == VolumeType::Capsule && volB->type == VolumeType::Sphere) {
		return SphereCapsuleIntersection((CapsuleVolume&)*volA, transformA, (SphereVolume&)*volB, transformB, collisionInfo);
	}
	if (volA->type == VolumeType::Sphere && volB->type == VolumeType::Capsule) {
		collisionInfo.a = b;
		collisionInfo.b = a;
		return SphereCapsuleIntersection((CapsuleVolume&)*volB, transformB, (SphereVolume&)*volA, transformA, collisionInfo);
	}

	return false;
}

bool CollisionDetection::AABBTest(const Vector3& posA, const Vector3& posB, const Vector3& halfSizeA, const Vector3& halfSizeB) {
	Vector3 delta = posB - posA;
	Vector3 totalSize = halfSizeA + halfSizeB;
	if (abs(delta.x) < totalSize.x &&
		abs(delta.y) < totalSize.y &&
		abs(delta.z) < totalSize.z)
	{
		return true;
	}
	return false;
}

void CollisionDetection::SATMinMax(const Vector3& axis, const std::vector<Vector3>& points, float& minA, float& maxA)
{
	minA = FLT_MAX;
	maxA = -FLT_MAX;
	for (const Vector3& point : points)
	{
		float dotVal = Vector3::Dot(point, axis);
		if (dotVal < minA) minA = dotVal;
		if (dotVal > maxA) maxA = dotVal;
	}
}

bool CollisionDetection::SATOverlap(float minA, float maxA, float minB, float maxB)
{
	return ValueBetween(minB, minA, maxA) || ValueBetween(minA, minB, maxB);
}

bool CollisionDetection::ValueBetween(float value, float min, float max)
{
	return min <= value && value <= max;
}

//AABB/AABB Collisions
bool CollisionDetection::AABBIntersection(const AABBVolume& volumeA, const Transform& worldTransformA,
	const AABBVolume& volumeB, const Transform& worldTransformB, CollisionInfo& collisionInfo) {
	Vector3 boxAPos = worldTransformA.GetPosition();
	Vector3 boxBPos = worldTransformB.GetPosition();
	Vector3 boxASize = volumeA.GetHalfDimensions();
	Vector3 boxBSize = volumeB.GetHalfDimensions();
	bool overlap = AABBTest(boxAPos, boxBPos, boxASize, boxBSize);
	if (overlap)
	{
		static const Vector3 faces[6] =
		{
			Vector3(-1, 0, 0), Vector3(1, 0, 0),
			Vector3(0, -1, 0), Vector3(0, 1, 0),
			Vector3(0, 0, -1), Vector3(0, 0, 1)
		};

		Vector3 maxA = boxAPos + boxASize;
		Vector3 minA = boxAPos - boxASize;
		Vector3 maxB = boxBPos + boxBSize;
		Vector3 minB = boxBPos - boxBSize;

		float distance[6] =
		{
			(maxB.x - minA.x),
			(maxA.x - minB.x),
			(maxB.y - minA.y),
			(maxA.y - minB.y),
			(maxB.z - minA.z),
			(maxA.z - minB.z)
		};
		float penetration = FLT_MAX;
		Vector3 bestAxis;
		for (int i = 0; i < 6; i++)
		{
			if (distance[i] < penetration)
			{
				penetration = distance[i];
				bestAxis = faces[i];
			}
		}
		collisionInfo.AddContactPoint(Vector3(), Vector3(), bestAxis, penetration);
		return true;
	}
	return false;
}

//Sphere / Sphere Collision
bool CollisionDetection::SphereIntersection(const SphereVolume& volumeA, const Transform& worldTransformA,
	const SphereVolume& volumeB, const Transform& worldTransformB, CollisionInfo& collisionInfo) {

	float radii = volumeA.GetRadius() + volumeB.GetRadius();
	Vector3 delta = worldTransformB.GetPosition() - worldTransformA.GetPosition();
	float deltaLength = delta.Length();
	if (deltaLength < radii)
	{
		float penetration = radii - deltaLength;
		Vector3 normal = delta.Normalised();
		Vector3 localA = normal * volumeA.GetRadius();
		Vector3 localB = -normal * volumeB.GetRadius();
		collisionInfo.AddContactPoint(localA, localB, normal, penetration);
		return true;
	}
	return false;
}

bool CollisionDetection::BoxSphereIntersection(const Vector3& boxSize, const Vector3& boxPos, float sphereRadius, const Vector3& spherePos, CollisionInfo& collisionInfo)
{
	Vector3 delta = spherePos - boxPos;
	Vector3 closestPointOnBox = Maths::Clamp(delta, -boxSize, boxSize);
	Vector3 localPoint = delta - closestPointOnBox;
	float distance = localPoint.Length();
	if (distance < sphereRadius)
	{
		Vector3 collisionNormal = localPoint.Normalised();
		float penetration = sphereRadius - distance;
		Vector3 localA = closestPointOnBox;
		Vector3 localB = -collisionNormal * sphereRadius;

		collisionInfo.AddContactPoint(localA, localB, collisionNormal, penetration);
		return true;
	}
	return false;
}

//AABB - Sphere Collision
bool CollisionDetection::AABBSphereIntersection(const AABBVolume& volumeA, const Transform& worldTransformA,
	const SphereVolume& volumeB, const Transform& worldTransformB, CollisionInfo& collisionInfo) {

	bool collided = BoxSphereIntersection(volumeA.GetHalfDimensions(), worldTransformA.GetPosition(), 
										volumeB.GetRadius(), worldTransformB.GetPosition(), collisionInfo);
	if (collided)
	{
		collisionInfo.point.localA = Vector3();
		return true;
	}
	return false;
}

bool CollisionDetection::OBBSphereIntersection(const OBBVolume& volumeA, const Transform& worldTransformA,
	const SphereVolume& volumeB, const Transform& worldTransformB, CollisionInfo& collisionInfo)
{
	Quaternion orientationA = worldTransformA.GetOrientation();
	Vector3 positionA = worldTransformA.GetPosition();
	Matrix3 transformA = Matrix3(orientationA);
	Matrix3 invTranfromA = Matrix3(orientationA.Conjugate());
	Vector3 localSpherePos = worldTransformB.GetPosition() - positionA;
	bool collided = BoxSphereIntersection(volumeA.GetHalfDimensions(), Vector3(),
										volumeB.GetRadius(), invTranfromA * localSpherePos, collisionInfo);
	if (collided)
	{
		collisionInfo.point.normal = transformA * collisionInfo.point.normal;
	}
	return collided;
}

bool CollisionDetection::OBBIntersection(
	const OBBVolume& volumeA, const Transform& worldTransformA,
	const OBBVolume& volumeB, const Transform& worldTransformB, CollisionInfo& collisionInfo) {

	static const Vector3 cornerScales[8] = { Vector3(-1,-1,-1), Vector3(1,-1,-1), Vector3(-1,1,-1), Vector3(-1,-1,1),
											Vector3(1,1,-1), Vector3(1,-1,1), Vector3(-1,1,1), Vector3(1,1,1) };
	Vector3 aPos = worldTransformA.GetPosition();
	Vector3 bPos = worldTransformB.GetPosition();
	Vector3 aSize = volumeA.GetHalfDimensions();
	Vector3 bSize = volumeB.GetHalfDimensions();

	std::vector<Vector3> aCorners(8);
	std::vector<Vector3> bCorners(8);
	for (int i = 0; i < 8; i++)
	{
		aCorners[i] = worldTransformA.GetMatrix() * (aSize * cornerScales[i]);
		bCorners[i] = worldTransformB.GetMatrix() * (bSize * cornerScales[i]);
	}

	bool collided = false;

	// Test A normals
	Matrix3 aRotation = Matrix3(worldTransformA.GetOrientation());
	for (int i = 0; i < 3; i++)
	{
		Vector3 normal = aRotation.GetColumn(i);
		float aMax = Vector3::Dot(aPos, normal) + aSize[i] ;
		float aMin = Vector3::Dot(aPos, normal) - aSize[i];
		float bMin = 0.0f, bMax = 0.0f;
		SATMinMax(normal, bCorners, bMin, bMax);
		if (!SATOverlap(aMin, aMax, bMin, bMax))
		{
			return false;
		}
	}

	// Test B normals
	Matrix3 bRotation = Matrix3(worldTransformB.GetOrientation());
	for (int i = 0; i < 3; i++)
	{
		Vector3 normal = bRotation.GetColumn(i);
		float bMax = Vector3::Dot(bPos, normal) + bSize[i];
		float bMin = Vector3::Dot(bPos, normal) - bSize[i];
		float aMin = 0.0f, aMax = 0.0f;
		SATMinMax(normal, aCorners, aMin, aMax);
		if (!SATOverlap(bMin, bMax, aMin, aMax))
		{
			return false;
		}
	}

	// Test cross product of normals 
	for (int i = 0; i < 3; i++)
	{
		Vector3 normalA = aRotation.GetColumn(i);
		for (int j = 0; j < 3; j++)
		{
			Vector3 normalB = aRotation.GetColumn(i);
			Vector3 normal = Vector3::Cross(normalA, normalB);
			float aMin = 0.0f, aMax = 0.0f;
			float bMin = 0.0f, bMax = 0.0f;
			SATMinMax(normal, aCorners, aMin, aMax);
			SATMinMax(normal, bCorners, bMin, bMax);
			if (!SATOverlap(bMin, bMax, aMin, aMax))
			{
				return false;
			}
		}
	}
	return false;
}

bool CollisionDetection::SphereCapsuleIntersection(
	const CapsuleVolume& volumeA, const Transform& worldTransformA,
	const SphereVolume& volumeB, const Transform& worldTransformB, CollisionInfo& collisionInfo) {
	return false;
}