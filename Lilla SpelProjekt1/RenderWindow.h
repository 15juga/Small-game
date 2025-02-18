#pragma once
#include "ErrorLogger.h"

class WindowContainer;

class RenderWindow
{
public:

	//bool initialize(WindowContainer * pWindowContainer, HINSTANCE hInstance, string window_title, string window_class, int width, int height);
	bool initialize(HINSTANCE hInstance, std::string window_title, std::string window_class, int width, int height);
	bool processMessages();
	HWND GetHWND()const;

	void Exit(HWND hwnd);

	~RenderWindow();

private:
	void RegisterWindowClass();
	HWND handle = NULL;
	HINSTANCE hInstance = NULL;
	std::string window_title = " ";
	std::wstring window_title_wide = L" ";
	std::string window_class = " ";
	std::wstring window_class_wide = L" ";
	int width = 0;
	int height = 0;
};
