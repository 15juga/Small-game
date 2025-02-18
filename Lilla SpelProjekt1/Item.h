#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include "SimpleMath.h"
#include "GameObject.h"



using namespace DirectX;
using namespace SimpleMath;

class Item: public GameObject
{
private:
	int speed;
	int health;
	int nrOfItems;
	bool hit;
	bool spawned;
	int bonusPoints;
	BoundingSphere itemSphere;
	BoundingSphere sphere;

protected:

public:

	Item();
	~Item();

	//bool initialize(const char* filePath, ID3D11Device* device, ID3D11DeviceContext* deviceContext, constantBuffer<MatrixVertex>& ConstantBuffer, std::wstring texturePath);
	bool initialize(MeshT& mesh);

	BoundingSphere &getSphere();
	bool createBoundSphere();
	bool updateBoundSphere();

	void spawn(float x, float y, float z);
	void randomSpawn(float x, float y, float z);
	void itemRotate(float x, float y, float z);

	void addItemHealth(int itemHealth);
	void loseHealth(int itemHealth);
	int getHealth();

	void addItemSpeed(int itemSpeed);
	void loseItemSpeed(int itemSpeed);

	void addBonusPoints(int bonusPointsFunc);
	void loseBonusPoints(int bonusPointsFunc);
	int getBonusPoints();


};