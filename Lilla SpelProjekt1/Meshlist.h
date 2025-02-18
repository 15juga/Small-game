#pragma once
#include "Mesh.h"
#include <d3d11.h>
enum class ASSET
{
	SANDBOX, TURRET, WALL, CUBE, TELEPORTER, PLAYER1, PLAYER2, PROJECTILE, COIN// ifall du l�gger till en ny mesh l�gg m�ste du �ka meshAsset
};

class MeshList
{
private:
	MeshT *meshAsset[9];
public:
	~MeshList();
	void initMeshList(ID3D11Device*& device, ID3D11DeviceContext*& deviceContext, constantBuffer<MatrixVertex>& ConstantBuffer);
	MeshT& getMesh(ASSET index) const;
};