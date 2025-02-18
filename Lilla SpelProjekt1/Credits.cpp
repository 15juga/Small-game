#include "Credits.h"

CreditScene::CreditScene(SCN& nextScn):nextScn(nextScn)
{
}

CreditScene::~CreditScene()
{
	button.buttonData.clear();
}

void CreditScene::init(ID3D11Device*& device, ID3D11DeviceContext*& deviceContext, constantBuffer<MatrixVertex>& ConstantBuffer)
{
}

void CreditScene::draW()
{
	rtd->BeginDraw();
	rtd->DrawBitmap(MenuBmp, menu);

	for (auto& b : button.buttonData) {

		button.Draw(rtd, b.second);
	}

	for (auto& b : UI.textData) {

		UI.Draw(rtd, b.second);
	}

	rtd->EndDraw();
}

void CreditScene::update(float deltaTime)
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

void CreditScene::killState()
{
}

void CreditScene::StartCredits(ID2D1RenderTarget*& RTD, LPCWSTR path, ID2D1Bitmap*& bmp)
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

void CreditScene::Initialize(HWND hwnd, int width, int height, ID2D1RenderTarget*& RTD)
{
	this->rtd = RTD;
	this->handle = hwnd;
	menu.right = width;
	menu.bottom = height;
	StartCredits(rtd, L"Assets/menu.png", MenuBmp);
	UI.Initialize(hwnd, width, height, RTD);

	button.AddButton(RTD, L"Assets/BackB.png", "BackB", 200.f, 100.f, 300.f, 100.f);

	UI.AddText(rtd, L"Alyssa Chow", L"Gabriola", 50.f, "Alyssa", width / 2.f, height / 2.f - 50, 400.f, 100.f, D2D1::ColorF::White);
	UI.AddText(rtd, L"Christian Falk", L"Gabriola", 50.f, "Christian", width / 2.f, height / 2.f, 400.f, 100.f, D2D1::ColorF::White);
	UI.AddText(rtd, L"Linus Dorell", L"Gabriola", 50.f, "Dorell", width / 2.f, height / 2.f + 50, 400.f, 100.f, D2D1::ColorF::White);
	UI.AddText(rtd, L"Linus Hellberg", L"Gabriola", 50.f, "Hellberg", width / 2.f, height / 2.f + 100, 400.f, 100.f, D2D1::ColorF::White);
	UI.AddText(rtd, L"Julian Agonoy", L"Gabriola", 50.f, "Julian", width / 2.f, height / 2.f + 150, 400.f, 100.f, D2D1::ColorF::White);
}
