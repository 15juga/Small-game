#pragma once
#include <d3d11.h>
#include <WICTextureLoader.h>
#include "Colour.h"
#include "ErrorLogger.h"

class TextureFile
{
private:
	void init1x1ColourTexture(ID3D11Device* device, const Colour& colorData);
	void initColorTexture(ID3D11Device* device, const Colour* colorData, UINT width, UINT height);
	
	ID3D11Resource* texture = nullptr;
	ID3D11ShaderResourceView* textureView = nullptr;
public:
	TextureFile();
	TextureFile(ID3D11Device* device, const Colour& color);
	TextureFile(ID3D11Device* device, const Colour* colorData, UINT width, UINT height);
	void initTextureFile(const std::wstring path, ID3D11Device* device);

	ID3D11ShaderResourceView* getTextureResourceView();
	ID3D11ShaderResourceView** getTextureResourceViewAdress();

};