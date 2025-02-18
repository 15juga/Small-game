#include "AIEnemyTurret.h"

AIEnemyTurret::AIEnemyTurret()
	:attackCooldown(1.f), attackCountdown(3.5f)
{

}

AIEnemyTurret::~AIEnemyTurret()
{

}

//bool AIEnemyTurret::initialize(const char* filePath, ID3D11Device* device, ID3D11DeviceContext* deviceContext, constantBuffer<MatrixVertex>& ConstantBuffer, std::wstring texturePath)
//{
//	if (!GameObject::initialize(filePath, device, deviceContext, ConstantBuffer, texturePath))
//	{
//		ErrorLogger::Log("Failed to initialize in AIEnemyTurret class");
//		return false;
//	}
//
//	createBoundSphere();
//
//	return true;
//}

//bool AIEnemyTurret::initialize(MeshT& mesh)
//{
//	if (GameObject::initialize(mesh))
//	{
//		ErrorLogger::Log("Failed to initialize in AIEnemyTurret class");
//		return false;
//	}
//	createBoundSphere();
//	return true;
//}

BoundingSphere AIEnemyTurret::getSphere()
{
	return this->sphere;
}

bool AIEnemyTurret::createBoundSphere()
{
	//Change size of Radius
	//sphere.CreateFromPoints(this->sphere, getMesh().getnrV(), getMesh().getpPos(), 4);
	this->sphere.Center = transform.GetPos();
	this->sphere.Radius = (0.8f);
	return true;
}

bool AIEnemyTurret::uppdateBoundSpherePos()
{
	//Uppdating center of Radius
	this->sphere.Center = transform.GetPos();
	/*this->sphere.Center = this->getPosMesh();*/

	return true;
}
void AIEnemyTurret::countDown(float deltaTime)
{
	//count down with time	
	//OutputDebugStringA(("Time: " + to_string(attackCountdown) + "\n").c_str());
	attackCountdown -= deltaTime;
}
bool AIEnemyTurret::update(Player* player)
{
	return inShootingRange(player);
}
bool AIEnemyTurret::inShootingRange(Player* player)
{
	Vector3 distance;
	bool inRange = false;

	float distanceToPlayer = (float)distance.Distance((Vector3)player->getTransform().GetPos(), this->transform.GetPos());

	if (distanceToPlayer <= radius && attackCountdown <= 0 && alreadyAttacked == false)
	{
		//OutputDebugStringA("Shooting Range\n");

		alreadyAttacked = true;
		inRange = true;
	}
	else if (attackCountdown <= 0 && alreadyAttacked == true)
	{
		//OutputDebugStringA("Not Shooting Range\n");
		attackCountdown = attackCooldown;

		alreadyAttacked = false;
		inRange = false;
	}

	return inRange;

}

void AIEnemyTurret::setProjectilePosition(Projectile& projectile)
{
	//Set projectile to turret
	projectile.getTransform().SetPos({ this->transform.GetPos().x,this->transform.GetPos().y,this->transform.GetPos().z});
	alreadyAttacked = true;
}

void AIEnemyTurret::moveTurret(float x, float y, float z)
{
	transform.SetPos(x, y, z);
}
