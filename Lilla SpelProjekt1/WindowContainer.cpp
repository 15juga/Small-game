#include "WindowContainer.h"
#include "Camera.h"

WindowContainer::WindowContainer() {

	RAWINPUTDEVICE rid;

	rid.usUsagePage = 0x01;
	rid.usUsage = 0x02;
	rid.dwFlags = 0;
	rid.hwndTarget = NULL;

	if (RegisterRawInputDevices(&rid, 1, sizeof(rid)) == FALSE) {

		ErrorLogger::Log(GetLastError(), "Failed to register raw input device\n");
		exit(-1);
	}
}

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT uint, WPARAM wParam, LPARAM lParam);
LRESULT WindowContainer::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) 
{

    if (ImGui_ImplWin32_WndProcHandler(hwnd, uMsg, wParam, lParam))
        return true;

    switch (uMsg) {
    case WM_INPUT: {
        UINT dataSize = 0;
        GetRawInputData(reinterpret_cast<HRAWINPUT>(lParam), RID_INPUT, NULL, &dataSize, sizeof(RAWINPUTHEADER));

        if (dataSize > 0) {
            std::unique_ptr<BYTE[]> rawData = std::make_unique<BYTE[]>(dataSize);
            if (GetRawInputData(reinterpret_cast<HRAWINPUT>(lParam), RID_INPUT, rawData.get(), &dataSize, sizeof(RAWINPUTHEADER)) == dataSize) {
                RAWINPUT* raw = reinterpret_cast<RAWINPUT*>(rawData.get());
                if (raw->header.dwType == RIM_TYPEMOUSE && (raw->data.mouse.lLastX != 0 || raw->data.mouse.lLastY != 0)) {

                    Input::Get().mRaw.x = raw->data.mouse.lLastX;
                    Input::Get().mRaw.y = raw->data.mouse.lLastY;
                }
            }
        }
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    default: {
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    }
}
