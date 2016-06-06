#include <windows.h>

#include "defines.h"

char *g_szApplicationName = "The Groovy GDI - Lines2";
char *g_szWindowClassName = "MyWindowClass";

LRESULT CALLBACK WindowProc(HWND hwnd,
							UINT msg,
							WPARAM wParam,
							LPARAM lParam)
{
	static HPEN BluePen		= CreatePen(PS_SOLID, 1, RGB(0, 0, 255));
	static HPEN RedPen		= CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
	static HPEN GreenPen	= CreatePen(PS_SOLID, 1, RGB(0, 255, 0));
	static HPEN PurplePen	= CreatePen(PS_SOLID, 1, RGB(255, 0, 255));
	static HPEN OldPen		= NULL;

	static int cxClient, cyClient;

	switch (msg) {
		case WM_CREATE:
			RECT rect;

			GetClientRect(hwnd, &rect);

			cxClient = rect.right;
			cyClient = rect.bottom;
			break;
		case WM_PAINT:	
			PAINTSTRUCT ps;
			BeginPaint(hwnd, &ps);

			{
				OldPen = (HPEN)SelectObject(ps.hdc, RedPen);

				const int NumLinesPerSide = 10;
				int yStep = cyClient / NumLinesPerSide;
				int xStep = cxClient / NumLinesPerSide;

				for (int mult = 1; mult < NumLinesPerSide; mult++) {
					SelectObject(ps.hdc, RedPen);
					MoveToEx(ps.hdc, xStep * mult, 0, 0);
					LineTo(ps.hdc, 0, cyClient - yStep * mult);	

					SelectObject(ps.hdc, PurplePen);
					MoveToEx(ps.hdc, xStep * mult, cyClient, 0);
					LineTo(ps.hdc, cxClient, cyClient - yStep * mult);

					SelectObject(ps.hdc, GreenPen);
					MoveToEx(ps.hdc, xStep * mult, 0, 0);
					LineTo(ps.hdc, cxClient, yStep * mult);

					SelectObject(ps.hdc, BluePen);
					MoveToEx(ps.hdc, xStep * mult, cyClient, 0);
					LineTo(ps.hdc, 0, yStep * mult);
				}
				SelectObject(ps.hdc, OldPen);
			}

			EndPaint(hwnd, &ps);
			break;
		case WM_SIZE:
			cxClient = LOWORD(lParam);
			cyClient = HIWORD(lParam);
			break;
		case WM_DESTROY:
			DeleteObject(RedPen);
			DeleteObject(GreenPen);
			DeleteObject(BluePen);
			DeleteObject(PurplePen);
			DeleteObject(OldPen);

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
