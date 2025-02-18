#pragma once
#include "SceneClass.h"
#include "ButtonUI.h"

#define ASSERT_HR(hr) assert(SUCCEEDED(hr))

class LeaderBoardScene : public Scene
{
private:
	SCN& nextScn;
	Buttons button;
	D2D1_RECT_F menu;
	ID2D1Bitmap* MenuBmp;
	ID2D1RenderTarget* rtd;
	HWND handle;

public:

	LeaderBoardScene(SCN& nextScn);
	~LeaderBoardScene();
	//Inherited via Scene
	virtual void init(ID3D11Device*& device, ID3D11DeviceContext*& deviceContext, constantBuffer<MatrixVertex>& ConstantBuffer) override;
	virtual void draW() override;
	virtual void update(float deltaTime) override;
	virtual void killState() override;

	void Initialize(HWND hwnd, int width, int height, ID2D1RenderTarget*& RTD);
	void StartBoard(ID2D1RenderTarget*& RTD, LPCWSTR path, ID2D1Bitmap*& bmp);
};