#include "Item.h"

Item::Item()
{
	this->spawned = false;
	this->hit = false;
	this->speed = 0.0f;
	this->health = 10;
	this->nrOfItems = 0;
	this->bonusPoints = 0;
}

Item::~Item()
{	
}

//bool Item::initialize(const char* filePath, ID3D11Device* device, ID3D11DeviceContext* deviceContext, constantBuffer<MatrixVertex>& ConstantBuffer, std::wstring texturePath)
//{
//	if (!GameObject::initialize(filePath, device, deviceContext, ConstantBuffer, texturePath))
//	{
//		ErrorLogger::Log("Failed to load itemMesh in Item class");
//		return false;
//	}
//	createBoundSphere();
//	return true;
//}
bool Item::initialize(MeshT& mesh)
{
	GameObject::initialize(mesh);
	return true;
}
BoundingSphere &Item::getSphere()
{
	return this->itemSphere;
}
bool Item::createBoundSphere()
{
	//Change size of Radius
	//itemSphere.CreateFromPoints(this->sphere, getMesh().getnrV(), getMesh().getpPos(), 4);
	this->itemSphere.Center = transform.GetPos();
	itemSphere.Radius = (0.9f);
	return true;
}
bool Item::updateBoundSphere()
{
	//Uppdating center of Radius
	this->itemSphere.Center = transform.GetPos();
	return true;
}

void Item::spawn(float x, float y, float z)
{
	transform.SetPos(x,y,z);
	updateBoundSphere();
}

void Item::randomSpawn(float x, float y, float z)
{
	transform.SetPos(rand()% 7 +1, rand() % 4 + 1, 0);
	updateBoundSphere();
}
void Item::itemRotate(float x, float y, float z)
{
	transform.Rotate(x,y,z);
}

void Item::addItemHealth(int itemHealth)
{
	this->health += itemHealth;
}
void Item::loseHealth(int itemHealth)
{
	this->health -= itemHealth;
}
int Item::getHealth()
{
	return this->health;
}
void Item::addBonusPoints(int  bonusPointsFunc)
{
	this->bonusPoints += bonusPointsFunc;
}
void Item::loseBonusPoints(int bonusPointsFunc)
{
	this->bonusPoints -= bonusPointsFunc;
}
int Item::getBonusPoints()
{
	return this->bonusPoints;
}
void Item::addItemSpeed(int itemSpeed)
{
	this->speed += itemSpeed;
}
void Item::loseItemSpeed(int itemSpeed)
{
	this->speed -= itemSpeed;
}
