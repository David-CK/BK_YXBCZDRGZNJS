/*
 * Include the library winmm.lib
*/

#include <windows.h>

#include "defines.h"

char *g_szApplicationName = "HelloWorld4";
char *g_szWindowClassName = "MyWindowClass";

LRESULT CALLBACK WindowProc(HWND hwnd,
							UINT msg,
							WPARAM wParam,
							LPARAM lParam)
{
	switch (msg) {
		case WM_CREATE:
			PlaySoundA("window_open.wav", NULL, SND_FILENAME | SND_ASYNC);
			break;
		case WM_KEYUP:
			switch (wParam) {
				case VK_ESCAPE:
					PostQuitMessage(0);
					break;
			}
		case WM_PAINT:
			PAINTSTRUCT ps;
			BeginPaint(hwnd, &ps);
			EndPaint(hwnd, &ps);
			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
	}
	return DefWindowProcA(hwnd, msg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance,
					HINSTANCE hPrevInstance,
					LPSTR szCmdLine,
					int iCmdShow)
{
	HWND					hWnd;
	WNDCLASSEXA				winclass;

	winclass.cbSize			= sizeof(WNDCLASSEXA);
	winclass.style			= CS_HREDRAW | CS_VREDRAW;
	winclass.lpfnWndProc	= WindowProc;
	winclass.cbClsExtra		= 0;
	winclass.cbWndExtra		= 0;
	winclass.hInstance		= hInstance;
	winclass.hIcon			= LoadIcon(NULL, IDI_APPLICATION);
	winclass.hCursor		= LoadCursor(NULL, IDC_ARROW);
	winclass.hbrBackground	= (HBRUSH)GetStockObject(WHITE_BRUSH);
	winclass.lpszMenuName	= NULL;
	winclass.lpszClassName	= g_szWindowClassName;
	winclass.hIconSm		= LoadIcon(NULL, IDI_APPLICATION);

	if (!RegisterClassExA(&winclass)) {
		MessageBoxA(NULL, "Class Registration Failed!", "Error", 0);
		return 0;
	}

	hWnd = CreateWindowExA(NULL,
							g_szWindowClassName,
							g_szApplicationName,
							WS_OVERLAPPEDWINDOW,
							0,
							0,
							WINDOW_WIDTH,
							WINDOW_HEIGHT,
							NULL,
							NULL,
							hInstance,
							NULL);
	if (!hWnd) {
		MessageBoxA(NULL, "CreatWindowEx Failed!", "Error!", 0);
	}

	ShowWindow(hWnd, iCmdShow);

	UpdateWindow(hWnd);

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg.wParam;
}
