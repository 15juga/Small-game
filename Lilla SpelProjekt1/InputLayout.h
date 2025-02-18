#pragma once
#include <d3d11.h>
#include "AllShader.h"
class InputLayout
{
private:
	VertexShader vertexShader;
	PixelShader pixelShader;

public:
	inline bool InitilizeShader(ID3D11Device*& device)
	{
		if (vertexShader.Initialize(device, L"../x64/Debug/VertexShader.cso"))
		{
			std::cerr << "Error creating vertex shader" << std::endl;
			return false;
		}
		if (pixelShader.Initialize(device, L"../x64/Debug/PixelShader.cso"))
		{
			std::cerr << "Error creating pixel shader" << std::endl;
			return false;
		}
		D3D11_INPUT_ELEMENT_DESC inputLayout[] =
		{
			{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"UV", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
			
		};
	}
};

