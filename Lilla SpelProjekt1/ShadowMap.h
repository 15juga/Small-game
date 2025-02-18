#pragma once
#include <d3d11.h>
#include "SimpleMath.h"
#include <DirectXCollision.h>
#include <DirectXHelpers.h>
#include <ppltasks.h>

#include "ConstantBuffer.h"

struct LightCB
{
	DirectX::SimpleMath::Vector4 position;
	DirectX::SimpleMath::Matrix view;
	DirectX::SimpleMath::Matrix projection;
};

class ShadowMap
{
public:
	ShadowMap(ID3D11Device *device, int width, int height);
	~ShadowMap();

	ID3D11ShaderResourceView* GetShaderRsrcView();
	ID3D11ShaderResourceView** GetShaderRsrcViewAddress();

	ID3D11DepthStencilView* getDSV();
	D3D11_VIEWPORT* GetViewPort();
	ID3D11SamplerState** GetSamplerState();
	//void RenderToDepthBuffer(); // need to call in a lot of parameters to make it work in this file


private:
	ShadowMap(const ShadowMap& rhs);

	int width, height;

	ID3D11ShaderResourceView* depthMapSrv;
	ID3D11DepthStencilView* depthMapDsv; // OM set render targets, writes the data of shader output to the depthStencilView

	ID3D11Texture2D* shadowMapTexture = nullptr;
	ID3D11SamplerState* comparisonSampler = nullptr;
	ID3D11RasterizerState* drawingRState = nullptr;

	D3D11_VIEWPORT viewPort;

};

