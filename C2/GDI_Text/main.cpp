#include <windows.h>

#include "defines.h"

char *g_szApplicationName = "The Groovy GDI - Text Output";
char *g_szWindowClassName = "MyWindowClass";

LRESULT CALLBACK WindowProc(HWND hwnd,
							UINT msg,
							WPARAM wParam,
							LPARAM lParam)
{
	static int cxClient, cyClient;

	switch (msg) {
		case WM_CREATE:
			RECT rect;

			GetClientRect(hwnd, &rect);

			cxClient = rect.right;
			cyClient = rect.bottom;
			break;

		case WM_KEYUP:
			switch (wParam) {
				case VK_ESCAPE:
					PostQuitMessage(0);
					break;
			}
			break;

		case WM_PAINT:	
			PAINTSTRUCT ps;
			BeginPaint(hwnd, &ps);

			{
				HBRUSH PatternBrush = CreateHatchBrush(HS_BDIAGONAL, RGB(0, 0, 255));
				HBRUSH OldBrush = (HBRUSH)SelectObject(ps.hdc, PatternBrush);

				Rectangle(ps.hdc, 0, cyClient / 2, cxClient, cyClient);

				SelectObject(ps.hdc, OldBrush);

				char *text = "1. I ain't got time to bleed.";

				TextOutA(ps.hdc, 5, 5, text, strlen(text));

				RECT TextBox;
				TextBox.top = 30;
				TextBox.left = 100;
				TextBox.bottom = 200;
				TextBox.right = cxClient - 100;

				text = "2. You take the blue pill and the story ends. You wake in your bed and believe whatever you want to believe. You take the red pill and you stay in Wonderland and I show you how deep the rabbit-hole goes.";

				DrawTextA(ps.hdc, text, strlen(text), &TextBox, DT_WORDBREAK);

				SetTextColor(ps.hdc, RGB(255, 0, 0));
				SetBkColor(ps.hdc, RGB(0, 0, 0));

				TextBox.top = 200;
				TextBox.left = 5;
				TextBox.bottom = 300;
				TextBox.right = cxClient - 200;

				text = "3. The greatest trick the devil ever pulled was convincing the world he didn't exist.";

				DrawTextA(ps.hdc, text, strlen(text), &TextBox, DT_WORDBREAK);

				SetBkMode(ps.hdc, TRANSPARENT);

				TextBox.top = 300;
				TextBox.left = 100;
				TextBox.bottom = cyClient;
				TextBox.right = cxClient - 50;

				text = "4. ... I'm 42 years old. In less than a year I'll be dead";

				DrawTextA(ps.hdc, text, strlen(text), &TextBox, DT_WORDBREAK);
			}
			EndPaint(hwnd, &ps);
			break;

		case WM_SIZE:
			cxClient = LOWORD(lParam);
			cyClient = HIWORD(lParam);
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
	HWND							hWnd;
	WNDCLASSEXA						winclass;

	winclass.cbSize					= sizeof(WNDCLASSEXA);
	winclass.style					= CS_HREDRAW | CS_VREDRAW;
	winclass.lpfnWndProc			= WindowProc;
	winclass.cbClsExtra				= 0;
	winclass.cbWndExtra				= 0;
	winclass.hInstance				= hInstance;
	winclass.hIcon					= LoadIcon(NULL, IDI_APPLICATION);
	winclass.hCursor				= LoadCursor(NULL, IDC_ARROW);
	winclass.hbrBackground			= (HBRUSH)GetStockObject(WHITE_BRUSH);
	winclass.lpszMenuName			= NULL;
	winclass.lpszClassName			= g_szWindowClassName;
	winclass.hIconSm				= LoadIcon(NULL, IDI_APPLICATION);

	if (!RegisterClassExA(&winclass)) {
		MessageBoxA(NULL, "Registration Failed!", "Error", 0);
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
		MessageBoxA(NULL, "CreateWindowEx Failed!", "Error", 0);
	}

	ShowWindow(hWnd, iCmdShow);
	UpdateWindow(hWnd);

	MSG msg;
	
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	UnregisterClassA(g_szWindowClassName, winclass.hInstance);

	return msg.wParam;
}
