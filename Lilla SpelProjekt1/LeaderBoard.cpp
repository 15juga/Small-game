#include "LeaderBoard.h"

LeaderBoardScene::LeaderBoardScene(SCN& nextScn):nextScn(nextScn)
{
}

LeaderBoardScene::~LeaderBoardScene()
{
	button.buttonData.clear();

}

void LeaderBoardScene::init(ID3D11Device*& device, ID3D11DeviceContext*& deviceContext, constantBuffer<MatrixVertex>& ConstantBuffer)
{
}

void LeaderBoardScene::draW()
{
	rtd->BeginDraw();
	rtd->DrawBitmap(MenuBmp, menu);

	for (auto& b : button.buttonData) {

		button.Draw(rtd, b.second);
	}

	rtd->EndDraw();
}

void LeaderBoardScene::update(float deltaTime)
{
	for (auto& b : button.buttonData)
	{
		button.Update(b.second, b.first);
	}

	if (Input::KeyDown(RI_MOUSE_LEFT_BUTTON_DOWN))
	{

		if (button.Intersect(button.buttonData.at("BackB").rect))
		{
			this->nextScn = SCN::MENU;
		}

		//OutputDebugStringA(("CLICKED:	" + to_string(deltaTime) + "\n").c_str());
	}
}

void LeaderBoardScene::killState()
{
}

void LeaderBoardScene::StartBoard(ID2D1RenderTarget*& RTD, LPCWSTR path, ID2D1Bitmap*& bmp)
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

void LeaderBoardScene::Initialize(HWND hwnd, int width, int height, ID2D1RenderTarget*& RTD)
{
	this->rtd = RTD;
	this->handle = hwnd;
	menu.right = width;
	menu.bottom = height;
	StartBoard(rtd, L"Assets/menu.png", MenuBmp);

	button.Initialize(hwnd, width, height, rtd);
	button.AddButton(RTD, L"Assets/BackB.png", "BackB", 200.f, 100.f, 300.f, 100.f);
}
