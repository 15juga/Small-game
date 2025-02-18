#pragma once
#include <d3d11.h>
#include <memory>
#include <Reader.h>
//#include <wrl/client.h>
//#include "Vertex.h"


template<class T = Vertex>
class vertexBuffer {
private:
	ID3D11Buffer* vBuffer;
	UINT stride;
	UINT bSize;

	vertexBuffer(const vertexBuffer<T>& rhs);

public:
	vertexBuffer()
		:stride(0), bSize(0), vBuffer(nullptr) {
	}
	~vertexBuffer() {
		//if (vBuffer)
			//vBuffer->Release();
	}
	void killBuffer()
	{
		if (vBuffer)
			vBuffer->Release();
	}
	vertexBuffer<T>& operator=(const vertexBuffer<T>&VB)
	{
		this->vBuffer = VB.vBuffer;
		this->stride = VB.stride;
		this->bSize = VB.bSize;
		return *this;
	}

	HRESULT Initialize(T* data, UINT vCount, ID3D11Device* device) {

		bSize = vCount;
		stride = sizeof(T);

		D3D11_BUFFER_DESC vDesc;
		ZeroMemory(&vDesc, sizeof(D3D11_BUFFER_DESC));

		vDesc.Usage = D3D11_USAGE_IMMUTABLE;
		vDesc.ByteWidth = sizeof(T) * vCount;
		vDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vDesc.CPUAccessFlags = 0;
		vDesc.MiscFlags = 0;
		vDesc.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA vData;
		ZeroMemory(&vData, sizeof(D3D11_SUBRESOURCE_DATA));

		vData.pSysMem = data;
		vData.SysMemPitch = 0;
		vData.SysMemSlicePitch = 0;

		HRESULT hr = device->CreateBuffer(&vDesc, &vData, &vBuffer);

		return hr;
	}

	ID3D11Buffer*& Get() {

		return vBuffer;
	}

	UINT* Stride() {
		return &stride;
	}


	UINT BufferSize() const {
		return bSize;
	}
};
