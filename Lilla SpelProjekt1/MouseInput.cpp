#include "MouseInput.h"

bool MouseInput::Initalize(HINSTANCE hInstance, HWND window, int WIDTH, int HEIGHT)
{
	screenWidth = WIDTH;
	screenHeight = HEIGHT;

	HRESULT hr;

	//CREATING DIRECT INTERFACE
	hr = DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&directInput, NULL);
	if (FAILED(hr))
	{
		return false;
	}

	if (!CreateMouse(window))
	{
		return false;
	}

	return true;
}

bool MouseInput::CreateMouse(HWND window)
{
	HRESULT hr;

	//CREATE MOUSE
	hr = directInput->CreateDevice(GUID_SysMouse, &mouseDevice, NULL);
	if (FAILED(hr))
	{
		return false;
	}

	//SET FORMAT
	hr = mouseDevice->SetDataFormat(&c_dfDIMouse);
	if (FAILED(hr))
	{
		return false;
	}

	//SET COOPERATIVE LEVEL OF THE MOUSE TO SHARE WITH OTHER PROGRAMS
	hr = mouseDevice->SetCooperativeLevel(window, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	if (FAILED(hr))
	{
		return false;
	}

	//ACQUIRE MOUSE
	hr = mouseDevice->Acquire();

	return true;
}

void MouseInput::ShutDown()
{
	//RELEASE MOUSE
	if (mouseDevice)
	{
		mouseDevice->Unacquire();
		mouseDevice->Release();
		mouseDevice = 0;
	}


	//RELEASE INPUT
	if (directInput)
	{
		directInput->Release();
		directInput = 0;
	}
}

void MouseInput::Frame()
{
	ReadMouse();
	ProcessInput();
}

void MouseInput::ReadMouse()
{
	HRESULT hr;

	hr = mouseDevice->GetDeviceState(sizeof(DIMOUSESTATE), (LPVOID)&mouseState);
	if (FAILED(hr))
	{
		if ((hr == DIERR_INPUTLOST) || (hr == DIERR_NOTACQUIRED))
		{
			mouseDevice->Acquire();
		}

	}
	return;
}

void MouseInput::ProcessInput()
{
	//UPDATE MOUSE LOCATION EVERY FRAME
	mouseX += mouseState.lX* 0.001f;
	mouseY += mouseState.lY*0.001f;
	
	deltaX = mouseState.lX;
	deltaY = mouseState.lY;

	//MAKING SURE MOUSE IS WITHIN SCREEN BORDERS
	if (mouseX < 0) { mouseX = 0; }
	if (mouseY < 0) { mouseY = 0; }

	if (mouseX > screenWidth) { mouseX = screenWidth; }
	if (mouseY > screenHeight) { mouseY = screenHeight; }
}

void MouseInput::GetMouseLocation(LONG& mouseX, LONG& mouseY)
{
	this->mouseX = mouseX;
	this->mouseY = mouseY;
}

void MouseInput::GetDelta(LONG& x, LONG& y)
{
	x = this->deltaX;
	y = this->deltaY;
}
