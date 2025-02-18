#include <d3d11.h>
#include <SimpleMath.h>



struct BoundingSphere
{
	BoundingSphere() : Center(0.0f, 0.0f, 0.0f), Radius(0.0f) {}

	DirectX::SimpleMath::Vector3 Center;

	float Radius;
};