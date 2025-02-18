#pragma once
#include <fstream>
#include "DepthStencil.h"
#include "LightSystem.h"
//#include "RenderTarget.h"
#include "InputLayout.h"
#include "WICTextureLoader.h"
#include "SceneManager.h"
#include "ShadowMap.h"
#include "ConstantBufferStructs.h"
//#include "Structs.h"
#include <d2d1_1.h>
#include <assert.h>
#include <wincodec.h>
#include "imgui/imgui.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_dx11.h"

#define ASSERT_HR(hr) assert(SUCCEEDED(hr))

using namespace SimpleMath;
using namespace DirectX;

class Graphic
{
private:

	XMMATRIX modelMatrix = XMMatrixIdentity();
	vertexBuffer<Vertex> plane;
	int vertexCount;
	static const int CAP = 2;
	//DirectX11
	ID3D11ShaderResourceView* textureSRV;
	ID3D11Device* device;
	ID3D11DeviceContext* deviceContext;
	IDXGISwapChain* swapchain;
	ID3D11RenderTargetView* rtv;
	DepthStencil depthStencil;
	ID3D11InputLayout* inputLayout;
	ID3D11SamplerState* sampler;
	
	D3D11_VIEWPORT viewport;
	D3D11_VIEWPORT viewport2;
	D3D11_VIEWPORT normalViewport;

	VertexShader vertexShader;
	VertexShader shadowVertexShader;
	PixelShader pixelShader;

	ID3D11RasterizerState* rState;

	//ConstantBuffer
	constantBuffer<MatrixVertex> camBuffer;
	constantBuffer<MatrixVertex> otherCamBuffer;
	
	constantBuffer<WVPMatrix> WVPM;

	//new base vertexShader ConstantBuffers
	constantBuffer<ViewProjection> viewProjectionBuffer;
	// to contain the matrices of the camera that acts as the light
	constantBuffer<LightMatrices> lightMatrixBuffer;

	constantBuffer<DirectionalLight> constBufferLight;

	constantBuffer<MatrixVertex> cbPlayer;
	constantBuffer<MatrixVertex> cbPlayer1;

	constantBuffer<MatrixVertex> cbPlane;

	//______________________Direct2D____________________
	ID2D1Factory1* factory;
	ID2D1RenderTarget* RTD;

	bool SetSState();
	void lightSystem(CameraClass& lightCamera);
	bool SetRState();
	
	BoundingFrustum frustum;

	void SetViewPort(int width, int height);
	void rightSetSplitScreen(CameraClass& camera, SceneManager& sceneManager);
	void leftSetSplitScreen(CameraClass& camera, SceneManager& sceneManager);
	void SingleScreen(CameraClass& camera, SceneManager& sceneManager);
	ShadowMap* sMap;
	BoundingSphere sceneBounds;
	Matrix shadowTransform;
	vertexBuffer<Vertex> *VrtxBuffer;

protected:

public:

	Graphic() {};
	~Graphic() {};

	void Shutdown();

	bool Initialize(int width, int height);
	bool InitializeDirectX(HWND hwnd, int width, int height);

	//Render Camera
	void Render(CameraClass& camera, CameraClass& otherCamera, CameraClass& lightCamera, SceneManager& sceneManager);
	void RenderToDepthBuffer(CameraClass& lightCamera, SceneManager& sceneManager);

	//GET FUNCTIONS
	ID3D11ShaderResourceView* GetTexture();
	int GetIndexCount();

	//GetDevice and DeviceContext
	ID3D11Device*& GetDevice();
	ID3D11DeviceContext*& GetDeviceContext();
	constantBuffer<MatrixVertex>& getcbPlayer();

	XMMATRIX getCamViewM(CameraClass& cam);
	XMMATRIX getCamProjM(CameraClass& cam);

	XMMATRIX getOtherCamViewM(CameraClass& otherCam);
	XMMATRIX getOtherCamProjM(CameraClass& othercam);

	//UPDATING MODEL MATRIX
	void MatrixUpdate(XMMATRIX& modelMatrix, XMFLOAT3 rotation, XMFLOAT3 scale, XMFLOAT3 translation);
	ID2D1RenderTarget*& GetRTD();
};