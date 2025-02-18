#pragma once
#include "GameObject.h"

class StaticGameObject : public GameObject
{
private:
	BoundingBox box;
	
	Vector3 initialPos;
public:
	StaticGameObject();
	~StaticGameObject();

	//bool initialize(const char* filePath, ID3D11Device* device, ID3D11DeviceContext* deviceContext, constantBuffer<MatrixVertex>& ConstantBuffer, const std::wstring texturePath);
	//bool initialize2(const char* filePath, ID3D11Device* device, ID3D11DeviceContext* deviceContext, constantBuffer<MatrixVertex>& ConstantBuffer, const std::wstring texturePath);
	bool initialize(MeshT& mesh);
	void setSpawn(float x, float y, float z);

	bool CreateBoundingBox();
	BoundingBox GetBox();
	void removeBoundingBox();
	void UpdateBox();
	void scaleBox(float factor);
	void RotateBox();
};