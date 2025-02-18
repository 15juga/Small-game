#include <chrono>
#include "Engine.h"

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "Xinput.lib")
#pragma comment(lib, "directXTK.lib")
#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "Dwrite.lib")

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR lpCmdLine,
	_In_ int nCmdShow)
{
	chrono::high_resolution_clock::time_point start, end;
	start = end = chrono::high_resolution_clock::now();
	int frameTime = (int)(((double)1 / (double)120) * (double)pow(10, 9));
	chrono::duration<long double> fTime = chrono::nanoseconds{ frameTime };
	chrono::duration<long double> deltaTime;
	Engine engine;
	engine.Initialize(hInstance, "Escape From Helios", "MyWindowClass", 1600, 900);
	while (engine.processMessages() == true)
	{
		end = chrono::high_resolution_clock::now();
		while (chrono::duration_cast<chrono::nanoseconds>(end - start) > fTime) {

			deltaTime = chrono::duration_cast<chrono::nanoseconds>(end - start);
			start = chrono::high_resolution_clock::now();
			engine.update((float)deltaTime.count());
			engine.RenderFrame();
			Input::Get().Update();
		}
	}
	engine.shutdown();
	return 0;
}