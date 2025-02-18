#pragma once
#include <d3d11.h>

struct RenderTarget
{
	ID3D11RenderTargetView* rtv;

	RenderTarget();
	~RenderTarget();


	bool Initialize(ID3D11Device*& device, IDXGISwapChain* swapChain);
};