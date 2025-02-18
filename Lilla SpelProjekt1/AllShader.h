#pragma once
#pragma once
#include <d3d11.h>
#include <string>
#pragma comment(lib, "D3DCompiler.lib")
#include <d3dcompiler.h>
using namespace std;

template<class T>
struct AllShaders
{
	bool Initialize(ID3D11Device*& device, wstring shaderPath);

	AllShaders() : shader(0), shaderBuffer(0) {};
	~AllShaders() {

		if (shader)
			shader->Release();
	}

	T* shader;
	ID3D10Blob* shaderBuffer;
};

inline bool AllShaders<ID3D11PixelShader>::Initialize(ID3D11Device*& device, wstring shaderPath)
{
	HRESULT hr = D3DReadFileToBlob(shaderPath.c_str(), &shaderBuffer);
	if (FAILED(hr)) {
		return false;
	}

	hr = device->CreatePixelShader(shaderBuffer->GetBufferPointer(), shaderBuffer->GetBufferSize(), NULL, &shader);
	if (FAILED(hr)) {
		return false;
	}

	return true;
}

inline bool AllShaders<ID3D11VertexShader>::Initialize(ID3D11Device*& device, wstring shaderPath)
{
	HRESULT hr = D3DReadFileToBlob(shaderPath.c_str(), &shaderBuffer);
	if (FAILED(hr)) {
		return false;
	}

	hr = device->CreateVertexShader(shaderBuffer->GetBufferPointer(), shaderBuffer->GetBufferSize(), NULL, &shader);
	if (FAILED(hr)) {
		return false;
	}

	return true;
}

inline bool AllShaders<ID3D11HullShader>::Initialize(ID3D11Device*& device, wstring shaderPath)
{
	HRESULT hr = D3DReadFileToBlob(shaderPath.c_str(), &shaderBuffer);
	if (FAILED(hr)) {
		return false;
	}

	hr = device->CreateHullShader(shaderBuffer->GetBufferPointer(), shaderBuffer->GetBufferSize(), NULL, &shader);
	if (FAILED(hr)) {
		return false;
	}

	return true;
}

inline bool AllShaders<ID3D11GeometryShader>::Initialize(ID3D11Device*& device, wstring shaderPath)
{
	HRESULT hr = D3DReadFileToBlob(shaderPath.c_str(), &shaderBuffer);
	if (FAILED(hr)) {
		return false;
	}

	hr = device->CreateGeometryShader(shaderBuffer->GetBufferPointer(), shaderBuffer->GetBufferSize(), NULL, &shader);
	if (FAILED(hr)) {
		return false;
	}

	return true;
}

inline bool AllShaders<ID3D11DomainShader>::Initialize(ID3D11Device*& device, wstring shaderPath)
{
	HRESULT hr = D3DReadFileToBlob(shaderPath.c_str(), &shaderBuffer);
	if (FAILED(hr)) {
		return false;
	}

	hr = device->CreateDomainShader(shaderBuffer->GetBufferPointer(), shaderBuffer->GetBufferSize(), NULL, &shader);
	if (FAILED(hr)) {
		return false;
	}

	return true;
}

inline bool AllShaders<ID3D11ComputeShader>::Initialize(ID3D11Device*& device, wstring shaderPath)
{
	HRESULT hr = D3DReadFileToBlob(shaderPath.c_str(), &shaderBuffer);
	if (FAILED(hr)) {
		return false;
	}

	hr = device->CreateComputeShader(shaderBuffer->GetBufferPointer(), shaderBuffer->GetBufferSize(), NULL, &shader);
	if (FAILED(hr)) {
		return false;
	}

	return true;
}

using PixelShader = AllShaders<ID3D11PixelShader>;
using VertexShader = AllShaders<ID3D11VertexShader>;
using HullShader = AllShaders<ID3D11HullShader>;
using DomainShader = AllShaders<ID3D11DomainShader>;
using GeometryShader = AllShaders<ID3D11GeometryShader>;
using ComputeShader = AllShaders<ID3D11ComputeShader>;