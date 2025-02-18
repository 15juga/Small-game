#include <DirectXMath.h>
#include <DirectXCollision.h>
#include <d3d11.h>
#include <SimpleMath.h>



using namespace DirectX;
using namespace SimpleMath;

class CollisionTesting
{
public:
	CollisionTesting();
	~CollisionTesting();

	bool SphereSphere(BoundingSphere sph1, BoundingSphere sph2);
	bool SphereAABB(BoundingSphere sph, BoundingBox box);
	bool SphereOBB(BoundingSphere sph, BoundingOrientedBox box);

	bool BoxInBox(BoundingBox box1, BoundingBox box2);

	bool SphereInFrustum (BoundingSphere sph, BoundingFrustum frustum);
	bool AABBInFrustum (BoundingBox box, BoundingFrustum frustum);
	bool OBBInFrustum (BoundingOrientedBox box, BoundingFrustum frustum);
private:
	Vector3 v;
	XMVECTOR plane;
	XMVECTOR plane1;
	XMVECTOR plane2;
	XMVECTOR plane3;
	XMVECTOR plane4;
	XMVECTOR plane5;
};