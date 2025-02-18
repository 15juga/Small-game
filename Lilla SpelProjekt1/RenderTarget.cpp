#include "RenderTarget.h"

RenderTarget::RenderTarget()
{
	rtv = nullptr;
}

RenderTarget::~RenderTarget()
{
	if (rtv) {
		rtv->Release();
	}
}

bool RenderTarget::Initialize(ID3D11Device*& device, IDXGISwapChain* swapChain)
{
	ID3D11Texture2D* backBuffer = nullptr;

	if (FAILED(swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer)))) {
		return false;
	}

	HRESULT hr = device->CreateRenderTargetView(backBuffer, nullptr, &rtv);

	backBuffer->Release();

	return !FAILED(hr);
}
