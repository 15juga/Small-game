#pragma once
#include <d2d1_1.h>
#include <assert.h>
#include <wincodec.h>
#include "SimpleMath.h"
#include <unordered_map>

using namespace SimpleMath;

#define ASSERT_HR(hr) assert(SUCCEEDED(hr))

struct ButtonData
{
	D2D1_RECT_F rect;
	ID2D1Bitmap* bmp;
	string ButtonName;
};

class Buttons
{
private:

	HWND handle;
	float GameWidth;
	float GameHeight;

	unordered_map<string, ButtonData> tempButton;
	ID2D1SolidColorBrush* brush;

public:
	unordered_map<string, ButtonData> buttonData;

	void Initialize(HWND hwnd, int width, int height, ID2D1RenderTarget*& RTD)
	{
		ASSERT_HR(RTD->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Black), &brush));
		GameWidth = width;
		GameHeight = height;
		handle = hwnd;
	}

	void AddButton(ID2D1RenderTarget*& RTD, LPCWSTR path, const string& name, float xPos, float yPos, float width, float height)
	{
		ButtonData tempData;
		IWICImagingFactory* WFactory = NULL;
		IWICBitmapDecoder* BCoder = NULL;
		IWICFormatConverter* FormatConverter = NULL;
		IWICBitmapFrameDecode* FrameDecode = NULL;

		ASSERT_HR(CoCreateInstance(CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER, __uuidof(IWICImagingFactory), (void**)(&WFactory)));
		ASSERT_HR(WFactory->CreateDecoderFromFilename(path, NULL, GENERIC_READ, WICDecodeMetadataCacheOnLoad, &BCoder));
		ASSERT_HR(WFactory->CreateFormatConverter(&FormatConverter));
		ASSERT_HR(BCoder->GetFrame(0, &FrameDecode));
		ASSERT_HR(FormatConverter->Initialize(FrameDecode, GUID_WICPixelFormat32bppPBGRA, WICBitmapDitherTypeNone, NULL, 0.f, WICBitmapPaletteTypeMedianCut));
		ASSERT_HR(RTD->CreateBitmapFromWicBitmap(FormatConverter, NULL, &tempData.bmp));

		tempData.ButtonName = name;
		buttonData.emplace(tempData.ButtonName, tempData); //Istället för pushback
		WFactory->Release();
		BCoder->Release();
		FormatConverter->Release();
		FrameDecode->Release();

		SetButtonPos(xPos, yPos, buttonData.at(name).rect);
		SetButtonSize(width, height, buttonData.at(name).rect);
		tempButton = buttonData;
	}

	void SetButtonPos(float x, float y, D2D1_RECT_F& rect)
	{
		rect.left = x;
		rect.top = y;
		rect.right = x;
		rect.bottom = y;
	}

	void SetButtonSize(float width, float height, D2D1_RECT_F& rect)
	{
		//left: Top left vertex X Pos
		//top:  Top left vertex Y Pos
		//right: Bottom right vertex X Pos
		//bottom: Bottom right vertex Y Pos

		//Skapa en RECT från mittpunkten
		//Mittpunkten ändras varje gång du ändrar en punkts position så positionerna måste sparas innan du ändrar dem
		float left = GetButtonCenter(rect).x - (width / 2.f);
		float right = GetButtonCenter(rect).x + (width / 2.f);
		float top = GetButtonCenter(rect).y - (height / 2.f);
		float bottom = GetButtonCenter(rect).y + (height / 2.f);

		rect.left = left;
		rect.right = right;
		rect.top = top;
		rect.bottom = bottom;
	}

	Vector2 GetButtonSize(D2D1_RECT_F& rect)
	{
		float buttonWidth = rect.right - rect.left;
		float buttonHeight = rect.bottom - rect.top;

		return Vector2(buttonWidth, buttonHeight);
	}

	Vector2 GetButtonCenter(D2D1_RECT_F& rect)
	{
		float xCenter = rect.right - (GetButtonSize(rect).x / 2.f);
		float yCenter = rect.bottom - (GetButtonSize(rect).y / 2.f);

		//Den här hämtar knappens locala mittpunkt
		//center.x = GetButtonSize(button).x / 2.f;
		//center.y = GetButtonSize(button).y / 2.f;

		return Vector2(xCenter, yCenter);
	}

	bool Intersect(D2D1_RECT_F& rect)
	{
		POINT P;
		bool intersected = false;
		GetCursorPos(&P);
		ScreenToClient(handle, &P);
		
		float x = P.x;
		float y = P.y;

		//OutputDebugStringA(("Mouse X: " + to_string(P.x) + "	Mouse Y: " + to_string(P.y) + "\n").c_str());

		if (x < rect.right && x > rect.left && y > rect.top && y < rect.bottom)
		{
			intersected = true;
		}
		else {
			intersected = false;
		}

		return intersected;
	}

	void Update(ButtonData& button, string name)
	{
		int addX = 20.f;
		int addY = 5.f;
		ButtonData temp = tempButton.at(name);

		if (Intersect(button.rect))
		{
			float buttonWidth = GetButtonSize(tempButton.at(name).rect).x + addX;
			float buttonHeight = GetButtonSize(tempButton.at(name).rect).y + addY;

			SetButtonSize(buttonWidth, buttonHeight, button.rect);
		}
		else {

			//Behöver fixa logiken, fungerar bara på en knapp inte på flera eftersom positionen är static. Knapparna som kommer efter sätts till första knappens position
			button.rect = temp.rect;
		}
	}

	void Draw(ID2D1RenderTarget*& RTD, ButtonData& bData)
	{
		RTD->DrawBitmap(bData.bmp, bData.rect);

		//int x = GetButtonCenter(buttonData.at("StartB").rect).x;
		//int y = GetButtonCenter(buttonData.at("StartB").rect).y;
		//OutputDebugStringA(("Pos Y: " + to_string(y) + "\n").c_str());

		//RTD->DrawLine(D2D1::Point2F(x, 0.f),
		//	D2D1::Point2F(x, GameHeight),
		//	brush, 1.f);

		//RTD->DrawLine(D2D1::Point2F(0.f, y),
		//	D2D1::Point2F(GameWidth, y),
		//	brush, 1.f);
	}
};