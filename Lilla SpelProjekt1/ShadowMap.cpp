#include "ShadowMap.h"

ShadowMap::ShadowMap(ID3D11Device* device, int width, int height) 
	:width(width), height(height)
{
	// Viewport instancing
	ZeroMemory(&viewPort, sizeof(D3D11_VIEWPORT));
	viewPort.TopLeftX = 0;
	viewPort.TopLeftY = 0;
	viewPort.Width = width;
	viewPort.Height = height;
	viewPort.MinDepth = 0.0f;
	viewPort.MaxDepth = 1.0f;



	D3D11_TEXTURE2D_DESC shadowMapDSC;
	ZeroMemory(&shadowMapDSC, sizeof(D3D11_TEXTURE2D_DESC));
	shadowMapDSC.Width = this->width;
	shadowMapDSC.Height = this->height;
	shadowMapDSC.MipLevels = 1;
	shadowMapDSC.ArraySize = 1;
	//shadowMapDSC.Format = DXGI_FORMAT_R24G8_TYPELESS;
	shadowMapDSC.Format = DXGI_FORMAT_R32_TYPELESS;
	shadowMapDSC.SampleDesc.Count = 1;
	shadowMapDSC.SampleDesc.Quality = 0;
	shadowMapDSC.Usage = D3D11_USAGE_DEFAULT;
	shadowMapDSC.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
	shadowMapDSC.CPUAccessFlags = 0;
	shadowMapDSC.MiscFlags = 0;

	HRESULT hr = device->CreateTexture2D(&shadowMapDSC, nullptr, &shadowMapTexture);

	// create a depth stencil that acts as a buffer for the depth data
	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDsc;
	ZeroMemory(&dsvDsc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
	//dsvDsc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	dsvDsc.Format = DXGI_FORMAT_D32_FLOAT;
	dsvDsc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	dsvDsc.Texture2D.MipSlice = 0;
	
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
	//srvDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
	srvDesc.Format = DXGI_FORMAT_R32_FLOAT;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;
	//srvDesc.Texture2D.MostDetailedMip = 0;

	hr = device->CreateDepthStencilView(shadowMapTexture, &dsvDsc, &depthMapDsv);

	hr = device->CreateShaderResourceView(shadowMapTexture, &srvDesc, &depthMapSrv);

	//create comparison state
	D3D11_SAMPLER_DESC comparisonSamplerDesc;
	ZeroMemory(&comparisonSamplerDesc, sizeof(D3D11_SAMPLER_DESC));
	comparisonSamplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	comparisonSamplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	comparisonSamplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	comparisonSamplerDesc.BorderColor[0] = 0.0f;
	comparisonSamplerDesc.BorderColor[1] = 0.0f;
	comparisonSamplerDesc.BorderColor[2] = 0.0f;
	comparisonSamplerDesc.BorderColor[3] = 0.0f;
	comparisonSamplerDesc.MinLOD = 0.f;
	comparisonSamplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
	comparisonSamplerDesc.MipLODBias = 0.f;
	comparisonSamplerDesc.MaxAnisotropy = 1;
	comparisonSamplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	comparisonSamplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;

	hr = device->CreateSamplerState(&comparisonSamplerDesc, &comparisonSampler);

	// create render state for front face culling
	D3D11_RASTERIZER_DESC drawingRenderStateDesc;
	ZeroMemory(&drawingRenderStateDesc, sizeof(D3D11_RASTERIZER_DESC));
	drawingRenderStateDesc.CullMode = D3D11_CULL_BACK;
	drawingRenderStateDesc.FillMode = D3D11_FILL_SOLID;
	drawingRenderStateDesc.DepthClipEnable = true; // Feature level 9_1 requires DepthClipEnable == true
	device->CreateRasterizerState(
			&drawingRenderStateDesc,
			&drawingRState
		);

}

ShadowMap::~ShadowMap()
{
	depthMapDsv->Release();
	depthMapSrv->Release();

	shadowMapTexture->Release();
	comparisonSampler->Release();
	drawingRState->Release();
}

ID3D11ShaderResourceView* ShadowMap::GetShaderRsrcView()
{
	return depthMapSrv;
}

ID3D11ShaderResourceView** ShadowMap::GetShaderRsrcViewAddress()
{
	return &this->depthMapSrv;
}

ID3D11DepthStencilView* ShadowMap::getDSV()
{
	return this->depthMapDsv;
}


D3D11_VIEWPORT* ShadowMap::GetViewPort()
{
	return &this->viewPort;
}

ID3D11SamplerState** ShadowMap::GetSamplerState()
{
	return &this->comparisonSampler;
}
