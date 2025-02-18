#pragma once
#include <d3d11.h>
#include <wrl\client.h>

template<class T>
class constantBuffer {
private:
	ID3D11Buffer* cBuffer;
	T data;

	constantBuffer(const constantBuffer& rhs);

public:
	constantBuffer()
		:data() {
	}
	constantBuffer<T>& operator=(const constantBuffer<T>& CB)
	{
		this->cBuffer = CB.cBuffer;
		this->data = CB.data;
		//this->bSize = VB.bSize;
		return *this;
	}
	~constantBuffer() {

		if (cBuffer) {

			cBuffer->Release();
		}
	}

	T& GetData() {
		return data;
	}

	bool Initialize(ID3D11Device* device) {

		D3D11_BUFFER_DESC cDesc;
		ZeroMemory(&cDesc, sizeof(D3D11_BUFFER_DESC));

		UINT size = (sizeof(T) % 16 == 0) ? static_cast<UINT>(sizeof(T)) : static_cast<UINT>(sizeof(T) + (16 - (sizeof(T) % 16)));

		cDesc.Usage = D3D11_USAGE_DYNAMIC;
		cDesc.ByteWidth = size;
		cDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		cDesc.MiscFlags = 0;
		cDesc.StructureByteStride = 0;

		HRESULT hr = device->CreateBuffer(&cDesc, 0, &cBuffer);

		return !FAILED(hr);
	}

	bool ApplyChange(ID3D11DeviceContext* devicecontext) {
		D3D11_MAPPED_SUBRESOURCE mResource;
		HRESULT hr = devicecontext->Map(cBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mResource);
		if (FAILED(hr)) {
			return false;
		}

		CopyMemory(mResource.pData, &data, sizeof(T));
		devicecontext->Unmap(cBuffer, 0);
		return true;
	}

	ID3D11Buffer*& Get() {

		return cBuffer;
	}
	ID3D11Buffer* const* GetAddressOf()const
	{
		Microsoft::WRL::ComPtr<ID3D11Buffer> buffer;
		buffer = cBuffer;
		return buffer.GetAddressOf();
	}
};
