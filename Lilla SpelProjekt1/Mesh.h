#pragma once
#include "VertexBuffer.h"
#include "ConstantBuffer.h"
#include "MatrixVertex.h"
#include "ErrorLogger.h"
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include<sstream>
#include <unordered_map>
#include <limits.h>
#include "TextureT.h"

class MeshT;

struct MeshCluster {

	static int index;

	static std::vector<MeshT*>& Get() {

		static std::vector<MeshT*> meshes;
		return meshes;
	}
};

class MeshT
{
private:
	std::vector<Vertex> shape;
	vertexBuffer<Vertex> vertexBuffer;
	ID3D11Device* device;
	ID3D11DeviceContext* deviceContext;
	constantBuffer<MatrixVertex>* ConstantBuffer = nullptr;
	int count;
	int nrVertices = 0;
	//pointer pos of all vertices in one mesh
	//XMFLOAT3* pPosV = new XMFLOAT3[8];
	Vector3* pPosV;
	TextureFile textureFile;
	ACJL::Material material;

	void loadObj(const char* path, std::vector<Vertex>& mesh);
	void Load(const char* path, std::vector<Vertex>& mesh, ACJL::Material& material);
	//constantBuffer<MatrixVertex>* ConstantBuffer = nullptr;
public:
	MeshT();
	MeshT(ID3D11Device* device, ID3D11DeviceContext* deviceContext);
	MeshT(const MeshT& mesh);
	~MeshT();

	void loadMesh(const char* path, ID3D11Device*& device);
	void loadMesh(const char* path, ID3D11Device*& device, std::wstring texturePath);
	void initMesh(const char* path, ID3D11Device*& device, ID3D11DeviceContext*& devContext, constantBuffer<MatrixVertex>& ConstantBuffer);

	const XMFLOAT3* getpPos();
	const size_t getnrV();
	const int getCount();

	ID3D11Device*& getDevice();
	ID3D11DeviceContext*& getDeviceContext();
	constantBuffer<MatrixVertex>*& getConstantBuffer();

	//Material& GetMaterial();
	void RenderTexture(ID3D11DeviceContext* devicecontext, ID3D11Device* device);

	const bool& isLoaded()const;
	void Draw( ID3D11DeviceContext*& deviceContext);
	void Pdraw( ID3D11DeviceContext*& deviceContext);
	//void Draw(const Matrix pr, ID3D11DeviceContext*& deviceContext);
};