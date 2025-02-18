#pragma once
#include "Mesh.h"
#include "Transform.h"
//#include "CollisionTest.h"
#include "TextureT.h"

class GameObject
{
private:
	void updateWorldMatrix();

	//XMMATRIX worldMatrix = XMMatrixIdentity();

	ID3D11Device* device = nullptr;
	ID3D11DeviceContext* deviceContext = nullptr;
	constantBuffer<MatrixVertex>* ConstantBuffer = nullptr;

protected:
	Transform transform;
	MeshT mesh;
	MeshT* pMesh;

public:
	bool initialize(const char* filePath, ID3D11Device* device, ID3D11DeviceContext* deviceContext, constantBuffer<MatrixVertex>& ConstantBuffer, const std::wstring texturePath);
	bool initialize(MeshT& mesh);


	//incase you want a constant
	const MeshT getMesh() const;
	//Mesh
	MeshT getMesh();
	Transform &getTransform();

	//testIntersection
	//should be handled by collisionManager
	//bool intersection(GameObject*& sourceObject, GameObject*& targetObject);
	
	void draw();
	void pDraw();
};