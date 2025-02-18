#include "SCN_Gameover.h"

GameOver::GameOver(SCN& nextScn) :
	nextScn(nextScn)
{
}

GameOver::~GameOver()
{
	//button.buttonData.clear();
}

void GameOver::init(ID3D11Device*& device, ID3D11DeviceContext*& deviceContext, constantBuffer<MatrixVertex>& ConstantBuffer)
{
	
}

void GameOver::draW()
{
	rtd->BeginDraw();
	rtd->DrawBitmap(OverBmp, over);
	for (auto& b : button.buttonData) {

		button.Draw(rtd, b.second);
	}
	rtd->EndDraw();
}

void GameOver::SetGameOver(ID2D1RenderTarget*& RTD, LPCWSTR path, ID2D1Bitmap*& bmp)
{
	IWICImagingFactory* WFactory = NULL;
	IWICBitmapDecoder* BCoder = NULL;
	IWICFormatConverter* FormatConverter = NULL;
	IWICBitmapFrameDecode* FrameDecode = NULL;

	ASSERT_HR(CoCreateInstance(CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER, __uuidof(IWICImagingFactory), (void**)(&WFactory)));
	ASSERT_HR(WFactory->CreateDecoderFromFilename(path, NULL, GENERIC_READ, WICDecodeMetadataCacheOnLoad, &BCoder));
	ASSERT_HR(WFactory->CreateFormatConverter(&FormatConverter));
	ASSERT_HR(BCoder->GetFrame(0, &FrameDecode));
	ASSERT_HR(FormatConverter->Initialize(FrameDecode, GUID_WICPixelFormat32bppPBGRA, WICBitmapDitherTypeNone, NULL, 0.f, WICBitmapPaletteTypeMedianCut));
	ASSERT_HR(RTD->CreateBitmapFromWicBitmap(FormatConverter, NULL, &bmp));

	WFactory->Release();
	BCoder->Release();
	FormatConverter->Release();
	FrameDecode->Release();
}

void GameOver::Initialize(HWND hwnd, int width, int height, ID2D1RenderTarget*& RTD)
{
	this->rtd = RTD;
	this->handle = hwnd;
	over.right = width;
	over.bottom = height;
	SetGameOver(rtd, L"Assets/menu.png", OverBmp);

	button.AddButton(rtd, L"Assets/menuB.png", "MenuB", width / 2.f, (height / 2.f), 400.f, 150.f);
	button.AddButton(rtd, L"Assets/QuitB.png", "QuitB", width / 2.f, (height / 2.f) + 200.f, 400.f, 150.f);

	button.Initialize(hwnd, width, height, rtd);
}

void GameOver::update(float deltaTime)
{
	for (auto& b : button.buttonData)
	{
		button.Update(b.second, b.first);
	}

	if (Input::KeyDown(RI_MOUSE_LEFT_BUTTON_DOWN))
	{
		if (button.Intersect(button.buttonData.at("MenuB").rect))
		{
			this->nextScn = SCN::MENU;
		}

		if (button.Intersect(button.buttonData.at("QuitB").rect))
		{
			DestroyWindow(handle);
		}
		//OutputDebugStringA(("CLICKED:	" + to_string(deltaTime) + "\n").c_str());
	}
}

void GameOver::killState()
{
}




