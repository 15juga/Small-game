#pragma once
#include "RenderWindow.h"
#include "Keyboard.h"
#include "Mouse.h"

class WindowContainer {

public:
	WindowContainer();
	LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

protected:
	RenderWindow renderWindow;

private:

};