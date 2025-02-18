#pragma once
#include "GameObject.h"
#include "Player.h"
#include "Projectile.h"

class AIEnemyTurret : public GameObject
{
private:

	BoundingSphere sphere;
	float attackCooldown;
	float attackCountdown;

	bool alreadyAttacked = false;

	float radius = 15.0f;

protected:


public:
	AIEnemyTurret();
	~AIEnemyTurret();

	BoundingSphere getSphere();
	bool createBoundSphere();
	bool uppdateBoundSpherePos();
	void countDown(float deltaTime);

	bool update(Player* player);
	bool inShootingRange(Player* player);
	void setProjectilePosition(Projectile& projectile);
	void moveTurret(float x, float y, float z);

	//bool initialize(const char* filePath, ID3D11Device* device, ID3D11DeviceContext* deviceContext, constantBuffer<MatrixVertex>& ConstantBuffer, std::wstring texturePath);
	//bool initialize(MeshT& mesh);

};