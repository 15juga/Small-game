#include "GameObject.h"

bool GameObject::initialize(const char* filePath,/* string tag,*/  ID3D11Device* device, ID3D11DeviceContext* deviceContext, constantBuffer<MatrixVertex>& ConstantBuffer, const std::wstring texturePath)
{
	this->device = device;
	this->deviceContext = deviceContext;
	this->ConstantBuffer = &ConstantBuffer;
	//this->tag = tag;
	mesh.loadMesh(filePath, device, texturePath);

	//buildSphere(sphere, mesh);
	return true;
}

bool GameObject::initialize(MeshT& mesh)
{
	this->pMesh = &mesh;
	this->device = mesh.getDevice();
	this->deviceContext = mesh.getDeviceContext();
	this->ConstantBuffer = mesh.getConstantBuffer();
	return true;
}
const MeshT GameObject::getMesh() const
{
	return this->mesh;
}

MeshT GameObject::getMesh()
{
	return *this->pMesh;
}

Transform& GameObject::getTransform()
{
	// TODO: insert return statement here
	return this->transform;
}


void GameObject::draw()
{
	ConstantBuffer->GetData().world = transform.GetTransform().Transpose();
	if (!ConstantBuffer->ApplyChange(deviceContext))
	{
		OutputDebugStringA("failed to map CB in gameobject");
		return;
	}
	deviceContext->VSSetConstantBuffers(0, 1, &ConstantBuffer->Get());

	mesh.Draw(this->deviceContext);
	deviceContext->Draw(this->mesh.getCount(), 0);

}

void GameObject::pDraw()
{
	deviceContext->VSSetConstantBuffers(0, 1, &ConstantBuffer->Get());
	this->ConstantBuffer->GetData().world = transform.GetTransform().Transpose();
	if (!ConstantBuffer->ApplyChange(deviceContext))
	{
		OutputDebugStringA("failed to map CB in gameobject");
		return;
	}
	pMesh->Pdraw(this->deviceContext);
	deviceContext->Draw(pMesh->getCount(), 0);
}

void GameObject::updateWorldMatrix()
{
	//this->worldMatrix = XMMatrixRotationRollPitchYaw(this->rot.x, this->rot.y, this->rot.z) * XMMatrixTranslation(this->pos.x, this->pos.y, this->pos.z);
	//XMMATRIX vecRotationMatrix = XMMatrixRotationRollPitchYaw(0.0f, this->rot.y, 0.0f);
	//this->vec_forward = XMVector3TransformCoord(this->DEFAULT_FORWARD_VECTOR, vecRotationMatrix);
	//this->vec_backward = XMVector3TransformCoord(this->DEFAULT_BACKWARD_VECTOR, vecRotationMatrix);
	//this->vec_left = XMVector3TransformCoord(this->DEFAULT_LEFT_VECTOR, vecRotationMatrix);
	//this->vec_right = XMVector3TransformCoord(this->DEFAULT_RIGHT_VECTOR, vecRotationMatrix);
	//ConstantBuffer->ApplyChange(deviceContext);

	//mesh.Draw(this->deviceContext);
}