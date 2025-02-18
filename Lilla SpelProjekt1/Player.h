#pragma once
#include "GameObject.h"
#include "Item.h"
#include "Camera.h"
#include "UserInterface.h"

using namespace std;

class Player: public GameObject
{
private:
	UI ui;
	Item items;
	bool hit;
	float speed;
	float originalSpeed = 0.2f;
	BoundingSphere sphere;
	
	
	Vector3 axis;
	Vector3 initialPos;
	bool colliding;
protected:

public:
	Player();
	~Player();

	//bool initialize(const char* filePath, ID3D11Device* device, ID3D11DeviceContext* deviceContext, constantBuffer<MatrixVertex>& ConstantBuffer, const std::wstring texturePath);
	bool initialize(MeshT& mesh);
	void Update();
	void Player2Update();

	BoundingSphere &getSphere();
	bool buildSphere();
	void DebugPos();
	float GetOriginalSpeed();
	void Slow();
	void SpeedBoost();
	void RevertSpeed();

	void BounceBack(const BoundingBox& box);
	void BounceBack(const BoundingSphere& sphere);
	void SetColliding(bool t);

	float getSpeed() const;
	void setSpeed(float speed);

	void Move(float time);
	void MoveOtherPlayer(float time);
	bool HitPlayer(Item*& items);
	void SetSpawn(float x, float y, float z);
	bool uppdateBoundSpherePos();
};