#include "TextureT.h"

void TextureFile::init1x1ColourTexture(ID3D11Device* device, const Colour& colorData)
{
	initColorTexture(device, &colorData, 1, 1);
}

void TextureFile::initColorTexture(ID3D11Device* device, const Colour* colorData, UINT width, UINT height)
{
	CD3D11_TEXTURE2D_DESC textureDesc(DXGI_FORMAT_R8G8B8A8_UNORM, width, height);
	ID3D11Texture2D* p2DTexture = nullptr;
	D3D11_SUBRESOURCE_DATA initData{};
	initData.pSysMem = colorData;
	initData.SysMemPitch = sizeof(Colour);
	HRESULT hr = device->CreateTexture2D(&textureDesc, &initData, &p2DTexture);
	if (FAILED(hr))	ErrorLogger::Log(hr, "Failed to init texture from color data");
	texture = static_cast<ID3D11Texture2D*>(p2DTexture);
	CD3D11_SHADER_RESOURCE_VIEW_DESC srvDesc(D3D11_SRV_DIMENSION_TEXTURE2D, textureDesc.Format);
	hr = device->CreateShaderResourceView(texture, &srvDesc, &textureView);
	if (FAILED(hr)) ErrorLogger::Log(hr, "failed to create resource view from generated colour data");
}

void TextureFile::initTextureFile(const std::wstring path, ID3D11Device* device)
{
	HRESULT hr = DirectX::CreateWICTextureFromFile(device, path.c_str(), &texture, &textureView);
	if (FAILED(hr))
	{
		hr = DirectX::CreateWICTextureFromFile(device, L"Assets/missingTexture.png", &texture, &textureView);
		if (FAILED(hr))
		{
			ErrorLogger::Log("Assets/missingTexture.png not found, put it back");
			this->init1x1ColourTexture(device, { 255, 0, 255} );
		}
	}
}

TextureFile::TextureFile()
{}

TextureFile::TextureFile(ID3D11Device* device, const Colour& color)
{
	this->init1x1ColourTexture(device, color);
}

TextureFile::TextureFile(ID3D11Device* device, const Colour* colorData, UINT width, UINT height)
{
	this->initColorTexture(device, colorData, width, height);
}

ID3D11ShaderResourceView* TextureFile::getTextureResourceView()
{
	return this->textureView;
}

ID3D11ShaderResourceView** TextureFile::getTextureResourceViewAdress()
{
	return &this->textureView;
}
