#pragma once
#pragma once
#include <d3d11.h>
#include "ErrorLogger.h"

struct DepthStencil
{
	ID3D11DepthStencilState* DepthState;
	ID3D11DepthStencilView* DepthView;

	DepthStencil();
	~DepthStencil();

	bool Initialize(ID3D11Device*& device, int width, int height);
};