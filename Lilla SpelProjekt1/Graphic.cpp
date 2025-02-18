#include "Graphic.h"

bool Graphic::InitializeDirectX(HWND hwnd, int width, int height)
{
	DXGI_SWAP_CHAIN_DESC scd;
	ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));
	scd.BufferDesc.Width = width;
	scd.BufferDesc.Height = height;
	scd.BufferDesc.RefreshRate.Numerator = 60; //Defining the refresh rate if the program is in fullscreen mode with vSync on
	scd.BufferDesc.RefreshRate.Denominator = 1;
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	scd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	scd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	scd.SampleDesc.Count = 1;
	scd.SampleDesc.Quality = 0;
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	scd.BufferCount = 1;
	scd.OutputWindow = hwnd;
	scd.Windowed = TRUE;
	scd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	ASSERT_HR(D3D11CreateDeviceAndSwapChain(NULL, //IDXGI Adapter
		D3D_DRIVER_TYPE_HARDWARE, //UNKNOWN means unspecfied in this case
		NULL, //For software driver type
		D3D11_CREATE_DEVICE_DEBUG | D3D11_CREATE_DEVICE_BGRA_SUPPORT, //Flags for runtime flags
		NULL, //Feature levels of array
		0, //nrOf feature levels in the array
		D3D11_SDK_VERSION,
		&scd, //SwapChainDescription
		&this->swapchain, // SwapChain adress
		&this->device, // Device adress
		NULL, //Supported feature level
		&this->deviceContext)); //DeviceContext adress
	

	ID3D11Texture2D* backBuffer;
	ASSERT_HR(this->swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer)));

	ASSERT_HR(this->device->CreateRenderTargetView(backBuffer,NULL, &rtv));

	//___________Direct2D______________
	// Create a Direct2D render target
	ASSERT_HR(D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &factory));

	IDXGISurface* IXSurface;
	if (SUCCEEDED(this->swapchain->GetBuffer(0, IID_PPV_ARGS(&IXSurface))))
	{
		FLOAT dpiX;
		FLOAT dpiY;
		dpiX = dpiY = static_cast<FLOAT>(GetDpiForWindow(hwnd));
		D2D1_RENDER_TARGET_PROPERTIES props = D2D1::RenderTargetProperties(D2D1_RENDER_TARGET_TYPE_DEFAULT, D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED), dpiX, dpiY);

		ASSERT_HR(factory->CreateDxgiSurfaceRenderTarget(IXSurface, props, &RTD));
	}

	ASSERT_HR(this->device->CreateRenderTargetView(backBuffer,NULL, &rtv));

	// used to compute the stage walls
	sceneBounds.Center = Vector3::Zero;
	sceneBounds.Radius = 40;
	






	return true;
}

void Graphic::SetViewPort(int width, int height)
{
	//F�r splitScreen viewport
	ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = width / 2; 
	viewport.Height = height;
	viewport.MinDepth = 0.0F;
	viewport.MaxDepth = 1.0F;

	ZeroMemory(&viewport2, sizeof(D3D11_VIEWPORT));
	viewport2.Width = width / 2.0f;
	viewport2.Height = height;
	viewport2.MinDepth = 0.0f;
	viewport2.MaxDepth = 1.0f;
	viewport2.TopLeftX = width / 2;
	viewport2.TopLeftY = 0;

	ZeroMemory(&normalViewport, sizeof(D3D11_VIEWPORT));
	normalViewport.TopLeftX = 0;
	normalViewport.TopLeftY = 0;
	normalViewport.Width = width; //F�r singleplayer ta bort /2
	normalViewport.Height = height;
	normalViewport.MinDepth = 0.0F;
	normalViewport.MaxDepth = 1.0F;
}

void Graphic::SingleScreen(CameraClass& camera, SceneManager& sceneManager)
{
	this->deviceContext->RSSetViewports(1, &normalViewport);
	WVPM.GetData().WVPmat = Matrix(camera.GetViewMatrix() * camera.GetProjectionMatrix()).Transpose();
	WVPM.ApplyChange(deviceContext);

	sceneManager.drawScene(XMMatrixIdentity(), camera.GetViewMatrix() * camera.GetProjectionMatrix());
}

bool Graphic::SetSState()
{
	D3D11_SAMPLER_DESC sDesc;
	ZeroMemory(&sDesc, sizeof(D3D11_SAMPLER_DESC));

	sDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sDesc.MinLOD = 0;
	sDesc.MaxLOD = D3D11_FLOAT32_MAX;

	ASSERT_HR(device->CreateSamplerState(&sDesc, &sampler));

	return true;
}

void Graphic::lightSystem(CameraClass &lightCamera)
{	
	constBufferLight.GetData().ambient = DirectX::XMFLOAT4(0.25f, 0.25f, 0.25f, 1.0f);
	constBufferLight.GetData().diffuse = Vector4(0.8f);
	constBufferLight.GetData().specular = DirectX::XMFLOAT4(2.0f, 2.0f, 2.0f, 1.0f);
	constBufferLight.GetData().lightDirection = DirectX::XMFLOAT4(0.0f, -1.0f, 1.0f, 1.0f); // variable not used
	constBufferLight.GetData().position = DirectX::XMFLOAT4(4.0f, 10.0f, 7.0f, 0.0f);
	constBufferLight.GetData().camPosition = Vector4(lightCamera.GetPos().x, lightCamera.GetPos().y, lightCamera.GetPos().z , 1.0f);
	constBufferLight.GetData().range = 100.0f;
	constBufferLight.GetData().strength = 2.5f;
	constBufferLight.GetData().att0 = 0.f;
	constBufferLight.GetData().att1 = 1.f;
	constBufferLight.GetData().att2 = 0.f;
}

bool Graphic::SetRState()
{
	D3D11_RASTERIZER_DESC rDesc;
	ZeroMemory(&rDesc, sizeof(D3D11_RASTERIZER_DESC));
	rDesc.FillMode = D3D11_FILL_SOLID;
	rDesc.CullMode = D3D11_CULL_BACK;
	rDesc.AntialiasedLineEnable = FALSE;
	rDesc.MultisampleEnable = FALSE;

	ASSERT_HR(device->CreateRasterizerState(&rDesc, &rState));

	return true;
}

void Graphic::Shutdown()
{
	delete sMap;
	//VrtxBuffer->killBuffer();
	//delete VrtxBuffer;
	delete textureSRV;

	factory->Release();
	//delete factory;
	RTD->Release();
	//delete RTD;
	rState->Release();
	sampler->Release();
	inputLayout->Release();
	rtv->Release();
	swapchain->Release();
	deviceContext->Release();
	device->Release();
}

bool Graphic::Initialize(int width, int height)
{
	if (!vertexShader.Initialize(device, L"../x64/Debug/VertexShader.cso"))
	{
		ErrorLogger::Log("Failed to create Vertex Shader");
		return false;
	}
	if (!shadowVertexShader.Initialize(device, L"../x64/Debug/ShadowVertexShader.cso"))
	{
		ErrorLogger::Log("Failed to create shadow Vertex Shader");
		return false;
	}
	if (!pixelShader.Initialize(device, L"../x64/Debug/PixelShader.cso"))
	{
		ErrorLogger::Log("Failed to create Pixel Shader Shader");
		return false;
	}
	D3D11_INPUT_ELEMENT_DESC inputLayoutDesc[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"UV", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA},
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};
	HRESULT hr = device->CreateInputLayout(inputLayoutDesc, ARRAYSIZE(inputLayoutDesc), this->vertexShader.shaderBuffer->GetBufferPointer(), this->vertexShader.shaderBuffer->GetBufferSize(), &this->inputLayout);

	if (FAILED(hr))
	{
		ErrorLogger::Log(hr, "Failed to create input layout");
		return false;
	}

	//initialize CB's
	if (!cbPlayer.Initialize(device))
	{
		ErrorLogger::Log("Failed to create ConstantBuffer");
		return false;
	}

	if (!depthStencil.Initialize(device, width, height))
	{
		ErrorLogger::Log("Failed to initialize DepthStencil");
		return false;
	}
	if (!depthStencil.Initialize(device, width, height))
	{
		ErrorLogger::Log("Failed to initialize DepthStencil");
		return false;
	}

	if (!SetSState())
		return false;
	if (!SetRState())
		return false;
	if (!camBuffer.Initialize(device))
		return false;
	if (!otherCamBuffer.Initialize(device))
		return false;
	if (!WVPM.Initialize(device))
		return false;

	if (!constBufferLight.Initialize(device)) {
		ErrorLogger::Log("Failed to start light buffer");
		return false;
	}
	if (!viewProjectionBuffer.Initialize(device)) {
		ErrorLogger::Log("Failed to Initialize buffer light");
		return false;
	}
		/*SetViewPort(width, height);*/
		//setSplitScreen(width, height);
	if (!lightMatrixBuffer.Initialize(device)) {
		ErrorLogger::Log("Failed to Initialize buffer light");
		return false;
	}

	SetViewPort(width, height);

//	return true;

	// initalize the Shadow Map
	sMap = new ShadowMap(device, 2048, 2048);


		return true;
}

void Graphic::Render(CameraClass& camera, CameraClass& otherCamera, CameraClass& lightCamera, SceneManager& sceneManager)
{
	UINT offset = 0;
	this->deviceContext->RSSetState(rState);
	float bgColor[] = { 0.2f, 0.3f, 0.2f, 1.0f };
	this->deviceContext->ClearRenderTargetView(rtv, bgColor);
	this->deviceContext->ClearDepthStencilView(depthStencil.DepthView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1, 0);

	this->deviceContext->VSSetShader(vertexShader.shader, NULL, 0);
	this->deviceContext->PSSetShader(pixelShader.shader, NULL, 0);
	this->deviceContext->RSSetState(rState);

	this->deviceContext->OMSetRenderTargets(1, &rtv, depthStencil.DepthView);

	this->deviceContext->PSSetShaderResources(3,1, sMap->GetShaderRsrcViewAddress());

	this->deviceContext->IASetInputLayout(inputLayout);
	this->deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	this->deviceContext->PSSetSamplers(0, 1, &sampler);
	this->deviceContext->PSSetSamplers(1, 1, sMap->GetSamplerState());

	constBufferLight.ApplyChange(deviceContext);

	this->deviceContext->VSSetConstantBuffers(1, 1, &WVPM.Get()); 
	this->deviceContext->PSSetConstantBuffers(0,1, &constBufferLight.Get());

	// if we ever set a new depth setencil state somewhere else this needs to be here
	// right now though it could be move to initialize()
	deviceContext->OMSetDepthStencilState(depthStencil.DepthState, 1);

	//--------------------
	// Send the constant buffers to the Graphics device.
	// buffer of camera we want to see out of
	viewProjectionBuffer.GetData().projection = camera.GetProjectionMatrix().Transpose();
	viewProjectionBuffer.GetData().view = camera.GetViewMatrix().Transpose();
	// do i want to call applychange on this buffer?
	viewProjectionBuffer.ApplyChange(deviceContext);
	deviceContext->VSSetConstantBuffers(2, 1, &viewProjectionBuffer.Get());

	// buffer of the camera acting as the light
	lightMatrixBuffer.GetData().lightProjection = lightCamera.GetProjectionMatrix().Transpose();
	lightMatrixBuffer.GetData().lightView = lightCamera.GetViewMatrix().Transpose();

	Vector4 v4 = Vector4(lightCamera.GetPos().x, lightCamera.GetPos().y, lightCamera.GetPos().z, 1.0f);
	lightMatrixBuffer.GetData().lPos = v4; //lightPos
	v4 = Vector4(camera.GetPos().x, camera.GetPos().y, camera.GetPos().z, 1.0f);
	lightMatrixBuffer.GetData().eyePos = v4;

	lightMatrixBuffer.ApplyChange(deviceContext);
	deviceContext->VSSetConstantBuffers(3, 1, &lightMatrixBuffer.Get());
	//-------------------

	this->deviceContext->PSSetSamplers(0,1, &sampler);

	leftSetSplitScreen(camera, sceneManager);
	rightSetSplitScreen(otherCamera, sceneManager);
	//SingleScreen(camera, sceneManager);

	//ImGui_ImplDX11_NewFrame();
	//ImGui_ImplWin32_NewFrame();
	//ImGui::NewFrame();
	//ImGui::Begin("Test");
	//ImGui::End();
	//ImGui::Render();
	//ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	this->swapchain->Present(1, NULL);
	ID3D11ShaderResourceView* const pSRV[1] = { NULL };
	deviceContext->PSSetShaderResources(3, 1, pSRV);
}

void Graphic::leftSetSplitScreen(CameraClass& camera, SceneManager& sceneManager)
{
	this->deviceContext->RSSetViewports(1, &viewport);
	WVPM.GetData().WVPmat = Matrix(camera.GetViewMatrix() * camera.GetProjectionMatrix()).Transpose();
	WVPM.ApplyChange(deviceContext);

	sceneManager.drawScene(XMMatrixIdentity(), camera.GetViewMatrix() * camera.GetProjectionMatrix());
}

void Graphic::rightSetSplitScreen(CameraClass& camera, SceneManager& sceneManager)
{
	// set half viewport
	this->deviceContext->RSSetViewports(1, &viewport2);
	// set up the view and projection matrix for the render pass 
	WVPM.GetData().WVPmat = Matrix(camera.GetViewMatrix() * camera.GetProjectionMatrix()).Transpose();
	WVPM.ApplyChange(deviceContext);

	sceneManager.drawScene(XMMatrixIdentity(), camera.GetViewMatrix() * camera.GetProjectionMatrix());
}

// shadow pass
void Graphic::RenderToDepthBuffer(CameraClass& lightCamera, SceneManager& sceneManager)
{
	// clear depth and stencil
	this->deviceContext->ClearDepthStencilView(sMap->getDSV(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	// update the lighsystem with the light Camera. -- could be done once somewhere else
	lightSystem(lightCamera);

	// set the depth stencil to be the render target for this pass
	deviceContext->OMSetRenderTargets(0, nullptr, sMap->getDSV());

	// set the correct shaders for this pass
	deviceContext->VSSetShader(shadowVertexShader.shader, nullptr, 0);
	// the PS shader is null so we just discard the output here
	deviceContext->PSSetShader(nullptr, NULL, 0);

	// reset the viewports to be the shadow texture size
	deviceContext->RSSetViewports(1, sMap->GetViewPort());

	// Set the depth stencil state
	//deviceContext->OMSetDepthStencilState(depthStencil.DepthState, 1);// check stencilRef
	// Set rendering state. -- seems to not be neccessary
	//deviceContext->RSSetState(sMap->GetShadowRenderState());
	//IASetVertexBuffers -- we bind the vertex buffers in each objects mesh 
	//deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST); -- already initalized correctly
	//deviceContext->IASetInputLayout(inputLayout); //maybe should create a unique on in shadow map. -- otherwise it is already initialized correctly

	// Attach our vertex shader.
	WVPM.GetData().WVPmat = Matrix(lightCamera.GetViewMatrix() * lightCamera.GetProjectionMatrix()).Transpose(); // projection matrix is orthographic

	deviceContext->VSSetConstantBuffers(1, 1, &WVPM.Get());
	WVPM.ApplyChange(deviceContext);

	sceneManager.drawScene(Matrix::Identity, lightCamera.GetViewMatrix() * lightCamera.GetProjectionMatrix());
}

ID3D11ShaderResourceView* Graphic::GetTexture()
{
	return textureSRV;
}

int Graphic::GetIndexCount()
{
	return vertexCount;
}

ID3D11Device*& Graphic::GetDevice()
{
	return this->device;
}

ID3D11DeviceContext*& Graphic::GetDeviceContext()
{
	return this->deviceContext;
}

constantBuffer<MatrixVertex>& Graphic::getcbPlayer()
{
	return this->cbPlayer;
}

XMMATRIX Graphic::getCamViewM(CameraClass& cam)
{
	return cam.GetViewMatrix();
}

XMMATRIX Graphic::getCamProjM(CameraClass& cam)
{
	return cam.GetProjectionMatrix();
}

XMMATRIX Graphic::getOtherCamViewM(CameraClass& otherCam)
{
	return otherCam.GetViewMatrix();
}

XMMATRIX Graphic::getOtherCamProjM(CameraClass& othercam)
{
	return othercam.GetProjectionMatrix();
}

void Graphic::MatrixUpdate(XMMATRIX& modelMatrix, XMFLOAT3 rotation, XMFLOAT3 scale, XMFLOAT3 translation)
{
	    modelMatrix = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z) *
		DirectX::XMMatrixRotationX(rotation.x) *
		DirectX::XMMatrixRotationY(rotation.y) *
		DirectX::XMMatrixRotationZ(rotation.z) *
		DirectX::XMMatrixTranslation(translation.x, translation.y, translation.z);
}

ID2D1RenderTarget*& Graphic::GetRTD()
{
	return this->RTD;
}
