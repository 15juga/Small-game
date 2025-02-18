#pragma once
#include "SCN_Gameover.h"
#include "SCN_Level.h"
#include "SCN_Menu.h"
#include "CollisionManager.h"
#include "Meshlist.h"
#include "Credits.h"

#ifdef _DEBUG
#define TY_NEW new( _NORMAL_BLOCK, __FILE__, __LINE__)
#else
#define TY_NEW new
#endif

class SceneManager
{
private:
	bool hit;
	float Height;
	float Width;

protected:
	Scene* engineScene;
	SCN nextScn;
	SCN currentScn;
	MeshList meshList;

	Level* lvl1;
	Level* lvl2;
	float timer;
	Menu* menu;
	GameOver* gameover;
	CreditScene* creditScene;


public:
	SceneManager();
	~SceneManager();
	
	bool initializeSceneManagerObjects(ID3D11Device*& device, ID3D11DeviceContext*& deviceContext, constantBuffer<MatrixVertex>& ConstantBuffer);

	void update(ID3D11Device*& device, ID3D11DeviceContext*& deviceContext, constantBuffer<MatrixVertex>& ConstantBuffer, float deltaTime);
	void drawScene(const XMMATRIX& worldMatrix, const XMMATRIX& viewProjectionMatrix);
	void release();
	void loadScene(SCN nextScene, ID3D11Device*& device, ID3D11DeviceContext*& deviceContext, constantBuffer<MatrixVertex>& ConstantBuffer);
	void SetCam(CameraClass& camera, CameraClass& OCamera);

	SCN GetScene();
	void Initialize(HWND hwnd, int width, int height, ID2D1RenderTarget*& RTD);
};