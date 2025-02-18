#include "Meshlist.h"

MeshList::~MeshList()
{
	for (int i = 0; i < ARRAYSIZE(meshAsset); i++)
	{
		delete meshAsset[i];
	}
}

void MeshList::initMeshList(ID3D11Device*& device, ID3D11DeviceContext*& deviceContext, constantBuffer<MatrixVertex>& ConstantBuffer)
{
	for (int i = 0; i < ARRAYSIZE(meshAsset); i++)
	{
		this->meshAsset[i] = new MeshT;
	}
	this->meshAsset[(int)ASSET::SANDBOX]->initMesh("Assets/ACJL1.2/stage.acjl", device, deviceContext, ConstantBuffer);
	this->meshAsset[(int)ASSET::TURRET]->initMesh("Assets/ACJL1.2/turret.acjl", device, deviceContext, ConstantBuffer);
	this->meshAsset[(int)ASSET::WALL]->initMesh("Assets/ACJL1.2/container.acjl", device, deviceContext, ConstantBuffer);
	this->meshAsset[(int)ASSET::CUBE]->initMesh("Assets/ACJL1.2/battery.acjl", device, deviceContext, ConstantBuffer);
	this->meshAsset[(int)ASSET::PROJECTILE]->initMesh("Assets/ACJL1.2/projectile.acjl", device, deviceContext, ConstantBuffer);
	this->meshAsset[(int)ASSET::TELEPORTER]->initMesh("Assets/ACJL1.2/teleport.acjl", device, deviceContext, ConstantBuffer);
	this->meshAsset[(int)ASSET::PLAYER1]->initMesh("Assets/ACJL1.2/drone.acjl", device, deviceContext, ConstantBuffer);
	this->meshAsset[(int)ASSET::PLAYER2]->initMesh("Assets/ACJL1.2/drone2.acjl", device, deviceContext, ConstantBuffer);
	this->meshAsset[(int)ASSET::COIN]->initMesh("Assets/ACJL1.2/coin.acjl", device, deviceContext, ConstantBuffer);
}

MeshT& MeshList::getMesh(ASSET index) const
{
	return *this->meshAsset[(int)index];
}
