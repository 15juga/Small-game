#include "SCN_Menu.h"

Menu::Menu(SCN& nextScn) :
	nextScn(nextScn)
{
}

Menu::~Menu()
{
	button.buttonData.clear();
}

void Menu::init(ID3D11Device*& device, ID3D11DeviceContext*& deviceContext, constantBuffer<MatrixVertex>& ConstantBuffer)
{
}

void Menu::StartMenu(ID2D1RenderTarget*& RTD, LPCWSTR path, ID2D1Bitmap*& bmp)
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

void Menu::Initialize(HWND hwnd, int width, int height, ID2D1RenderTarget*& RTD)
{
	this->rtd = RTD;
	this->handle = hwnd;
	menu.right = width;
	menu.bottom = height;
	StartMenu(rtd, L"Assets/menu.png", MenuBmp);

	button.Initialize(hwnd, width, height, rtd);
	button.AddButton(rtd, L"Assets/StartB.png", "StartB", width / 2.f, (height / 2.f) - 75.f, 300.f, 100.f);
	button.AddButton(rtd, L"Assets/CreditsB.png", "CreditsB", width / 2.f, (height / 2.f) + 50.f, 325.f, 100.f);
	button.AddButton(rtd, L"Assets/QuitB.png", "QuitB", width / 2.f, (height / 2.f) + 175.f, 300.f, 100.f);
}

void Menu::update(float deltaTime)
{

	if (Input::KeyDown('4'))
		this->nextScn = SCN::GAMEOVER;
	
	for (auto& b : button.buttonData) 
	{
		button.Update(b.second, b.first);
	}


	if (Input::KeyDown(RI_MOUSE_LEFT_BUTTON_DOWN))
	{
		if (button.Intersect(button.buttonData.at("StartB").rect))
		{
			this->nextScn = SCN::LEVEL1;
		}

		if (button.Intersect(button.buttonData.at("CreditsB").rect))
		{
			this->nextScn = SCN::CREDITS;
		}

		if (button.Intersect(button.buttonData.at("QuitB").rect))
		{
			DestroyWindow(handle);
		}
	}

}

void Menu::draW()
{
	rtd->BeginDraw();
	rtd->DrawBitmap(MenuBmp, menu);

	for (auto& b : button.buttonData) {

		button.Draw(rtd, b.second);
	}
	
	rtd->EndDraw();
}

void Menu::killState()
{
}
