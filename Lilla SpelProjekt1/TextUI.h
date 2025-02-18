#pragma once
#include <dwrite.h>
#include <d2d1_1.h>
#include <string>
#include <assert.h>
#include <unordered_map>
#include "SimpleMath.h"
#include <sstream>
#include <chrono>

using namespace DirectX;
using namespace SimpleMath;

#define ASSERT_HR(hr) assert(SUCCEEDED(hr))

struct TextData
{
	std::wstring text;
	UINT32 TextLength;
	IDWriteTextFormat* pFormat;
	D2D1_RECT_F rect;
	ID2D1SolidColorBrush* brush;
	std::string name;
};


class TextUI
{
private:
	HWND handle;
	float GameWidth;
	float GameHeight;

	int index = 10;
	int size = 3;

public:

	std::unordered_map<std::string, TextData> textData;

	void Initialize(HWND hwnd, int width, int height, ID2D1RenderTarget*& RTD)
	{
		GameWidth = width;
		GameHeight = height;
		handle = hwnd;
	}

	void AddText(ID2D1RenderTarget*& RTD, std::wstring text, LPCWSTR font, float fontSize, std::string name, float xPos, float yPos, float width, float height, D2D1::ColorF colour)
	{
		TextData data;
		IDWriteFactory* wFactory = NULL;
		ASSERT_HR(RTD->CreateSolidColorBrush(colour, &data.brush));
		ASSERT_HR(DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), reinterpret_cast<IUnknown**>(&wFactory)));

		data.text = text;
		data.TextLength = (UINT32)wcslen(data.text.c_str());

		data.name = name;
		ASSERT_HR(wFactory->CreateTextFormat(font, NULL, DWRITE_FONT_WEIGHT_REGULAR, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, fontSize, L"en-us-sv", &data.pFormat));
		ASSERT_HR(data.pFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER));
		ASSERT_HR(data.pFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER));

		textData.emplace(data.name, data);
		SetTextPos(xPos, yPos, textData.at(name).rect);
		SetTextSize(width, height, textData.at(name).rect);
		wFactory->Release();
		//pFormat->Release();
	}

	void TimeText(std::string name, LPCWSTR text, float timer)
	{
		if (timer >= index) {
			size += 1;
			index *= 10;
		}

		textData.at(name).text = text;
		textData.at(name).TextLength = size;
	}
	
	void ChangePoints(std::string name, LPCWSTR text)
	{
		wstring length = text;
		textData.at(name).text = text;
		textData.at(name).TextLength = length.length();
	}

	void SetTextPos(float x, float y, D2D1_RECT_F& rect)
	{
		rect.left = x;
		rect.top = y;
		rect.right = x;
		rect.bottom = y;
	}

	void SetTextSize(float width, float height, D2D1_RECT_F& rect)
	{
		//left: Top left vertex X Pos
		//top:  Top left vertex Y Pos
		//right: Bottom right vertex X Pos
		//bottom: Bottom right vertex Y Pos

		//Skapa en RECT från mittpunkten
		//Mittpunkten ändras varje gång du ändrar en punkts position så positionerna måste sparas innan du ändrar dem
		float left = GetTextCenter(rect).x - (width / 2.f);
		float right = GetTextCenter(rect).x + (width / 2.f);
		float top = GetTextCenter(rect).y - (height / 2.f);
		float bottom = GetTextCenter(rect).y + (height / 2.f);

		rect.left = left;
		rect.right = right;
		rect.top = top;
		rect.bottom = bottom;
	}

	Vector2 GetTextSize(D2D1_RECT_F& rect)
	{
		float buttonWidth = rect.right - rect.left;
		float buttonHeight = rect.bottom - rect.top;

		return Vector2(buttonWidth, buttonHeight);
	}

	Vector2 GetTextCenter(D2D1_RECT_F& rect)
	{
		float xCenter = rect.right - (GetTextSize(rect).x / 2.f);
		float yCenter = rect.bottom - (GetTextSize(rect).y / 2.f);

		//Den här hämtar knappens locala mittpunkt
		//center.x = GetButtonSize(button).x / 2.f;
		//center.y = GetButtonSize(button).y / 2.f;

		return Vector2(xCenter, yCenter);
	}

	void Update(TextData& button, std::string name)
	{

	}

	void Draw(ID2D1RenderTarget*& RTD, TextData& tData)
	{
		RTD->DrawText(tData.text.c_str(), tData.TextLength, tData.pFormat, tData.rect, tData.brush);
	}
};