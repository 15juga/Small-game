#pragma once
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

#ifndef MOUSEINPUT_H_
#define MOUSEINPUT_H_

#include <dinput.h>

//PRE-PROCESSING DIRECTIVES
#define DIRECTINPUT_VERSION 0x0800


class MouseInput
{
private:

	IDirectInput8* directInput;
	IDirectInputDevice8* mouseDevice;

	//MEMBERS TO RECORD CURRENT STATE OF KEYBOARD AND MOUSE
	DIMOUSESTATE mouseState;

	LONG mouseX, mouseY;
	LONG deltaX = 0, deltaY = 0;
	int screenWidth, screenHeight;

	bool CreateMouse(HWND window);
	void ReadMouse();
	void ProcessInput();

public:
	MouseInput() :
		directInput(NULL),
		mouseDevice(NULL),
		mouseX(screenWidth),
		mouseY(screenHeight),
		deltaX(0),
		deltaY(0) {}

	bool Initalize(HINSTANCE hInstance, HWND window, int WIDTH, int HEIGHT);
	void ShutDown();
	void Frame();

	void GetMouseLocation(LONG& mouseX, LONG& mouseY);
	void GetDelta(LONG& x, LONG& y);
};

#endif // !MOUSEINPUT_H_

