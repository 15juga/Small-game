#pragma once
#include "GameObject.h"

class Projectile : public GameObject
{
private:

	float speed = 1.f;
	bool shot = false;

//BUILD BOUNDING BOX
	BoundingSphere sphere;

public:

	Projectile();
	~Projectile();

	BoundingSphere getSphere();
	bool CreateBoundingSphere();	
	bool uppdateBoundSpherePos();

	bool Initialize(const char* filePath, ID3D11Device* device, ID3D11DeviceContext* deviceContext, constantBuffer<MatrixVertex>& ConstantBuffer, const std::wstring texturePath);
	bool Initialize(MeshT& pMesh);

	void Update();
	bool SetShot(bool statement);
	bool GetShot();
	float GetSpeed();
	void MoveProjectile(float x, float y, float z);

	void FireInDirection(float deltaTime);
};