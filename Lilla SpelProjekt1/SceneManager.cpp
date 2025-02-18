#include "SceneManager.h"

SceneManager::SceneManager()
{
	this->hit = false;
	nextScn = SCN::MENU;
}

SceneManager::~SceneManager()
{
	delete engineScene;
}

bool SceneManager::initializeSceneManagerObjects(ID3D11Device*& device, ID3D11DeviceContext*& deviceContext, constantBuffer<MatrixVertex>& ConstantBuffer)
{
	meshList.initMeshList(device, deviceContext, ConstantBuffer);
	this->lvl1 = new Level(this->nextScn, "Levels/Level1.txt", meshList);
	this->lvl2 = new Level(this->nextScn, "Levels/Level2.txt", meshList);
	this->menu = new Menu(this->nextScn);
	this->gameover = new GameOver(this->nextScn);
	this->creditScene = new CreditScene(this->nextScn);
	return true;
}

void SceneManager::drawScene(const XMMATRIX& worldMatrix, const XMMATRIX& viewProjectionMatrix)
{
	engineScene->draW();
}

void SceneManager::update(ID3D11Device*& device, ID3D11DeviceContext*& deviceContext, constantBuffer<MatrixVertex>& ConstantBuffer,float deltaTime)
{
	if (currentScn != nextScn)
		loadScene(nextScn, device, deviceContext, ConstantBuffer);

	engineScene->update(deltaTime);

	if (currentScn == SCN::LEVEL1)
	{
		timer = lvl1->GetTime();
	}
}

void SceneManager::loadScene(SCN nextScene, ID3D11Device*& device, ID3D11DeviceContext*& deviceContext, constantBuffer<MatrixVertex>& ConstantBuffer)
{
	if (engineScene != nullptr)
	{
		lvl2->SetTimer(timer);

		engineScene->killState();
		engineScene = nullptr;
		delete engineScene;
		//free(engineScene);
	}
	switch (nextScene)
	{
	case SCN::E_NULL:
		// BLANK
		break;
	case SCN::MENU:
		this->engineScene = this->menu;
		break;
	case SCN::CREDITS:
		this->engineScene = this->creditScene;
		break;
	case SCN::LEVEL1:
		this->engineScene = this->lvl1;
		break;
	case SCN::LEVEL2:
		this->engineScene = this->lvl2;
		break;
	case SCN::GAMEOVER:
		this->engineScene = this->gameover;
		break;
	default:
		break;
	}
	engineScene->init(device, deviceContext, ConstantBuffer);
	currentScn = nextScene;
	//OutputDebugStringA("loadScene called\n");
}

void SceneManager::SetCam(CameraClass& camera, CameraClass& OCamera)
{
	if (currentScn == SCN::LEVEL1)
	{
		this->lvl1->SetCamPos(camera, OCamera);
	}
	if (currentScn == SCN::LEVEL2)
	{
		this->lvl2->SetCamPos(camera, OCamera);
	}
}

SCN SceneManager::GetScene()
{
	return this->currentScn;
}

void SceneManager::Initialize(HWND hwnd, int width, int height, ID2D1RenderTarget*& RTD)
{
	menu->Initialize(hwnd, width, height, RTD);
	gameover->Initialize(hwnd, width, height, RTD);
	creditScene->Initialize(hwnd, width, height, RTD);
	lvl1->InitializeUI(hwnd, width, height, RTD);
	lvl2->InitializeUI(hwnd, width, height, RTD);
}

void SceneManager::release()
{
}
