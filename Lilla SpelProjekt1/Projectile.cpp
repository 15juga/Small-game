#include "Projectile.h"


bool Projectile::CreateBoundingSphere()
{
	this->sphere.Center = transform.GetPos();
	this->sphere.Radius = (0.2f);
	return true;
}
bool Projectile::uppdateBoundSpherePos()
{
	this->sphere.Center = transform.GetPos();
	return true;
}
BoundingSphere Projectile::getSphere()
{
	return this->sphere;
}

Projectile::Projectile()
{
}

Projectile::~Projectile()
{
}

bool Projectile::Initialize(const char* filePath, ID3D11Device* device, ID3D11DeviceContext* deviceContext, constantBuffer<MatrixVertex>& ConstantBuffer, const std::wstring texturePath)
{
	if (!GameObject::initialize(filePath, device, deviceContext, ConstantBuffer, texturePath))
	{
		ErrorLogger::Log("Failed to initialize in AIEnemyTurret class");
		return false;
	}

	CreateBoundingSphere();
	return true;
}

bool Projectile::Initialize(MeshT& pMesh)
{
	if (!GameObject::initialize(pMesh))
	{
		ErrorLogger::Log("Failed to initialize in AIEnemyTurret class");
		return false;
	}

	CreateBoundingSphere();
	return true;
}

bool Projectile::SetShot(bool statement)
{
	shot = statement;
	return shot;
}

bool Projectile::GetShot()
{
	return shot;
}
float Projectile::GetSpeed()
{
	return speed;
}

void Projectile::MoveProjectile(float x, float y, float z)
{
	transform.SetPos(x, y, z);
}

void Projectile::FireInDirection(float deltaTime)
{
	getTransform().MoveRelative(0.f, 0.f, speed * deltaTime);
}