#include <windows.h>
#include <time.h>

#include "defines.h"
#include "utils.h"

char *g_szApplicationName = "The Groovy GDI - Polygons";
char *g_szWindowClassName = "MyWindowClass";

LRESULT CALLBACK WindowProc(HWND hwnd,
							UINT msg,
							WPARAM wParam,
							LPARAM lParam)
{
	static HPEN BluePen		= CreatePen(PS_SOLID, 1, RGB(0, 0, 255));
	static HPEN OldPen		= NULL;

	static HBRUSH RedBrush	= CreateSolidBrush(RGB(255, 0, 0));
	static HBRUSH OldBrush	= NULL;

	static int cxClient, cyClient;

	static POINT verts[NUM_VERTS];
	static int iNumVerts = NUM_VERTS;

	switch (msg) {
		case WM_CREATE:
			RECT rect;

			GetClientRect(hwnd, &rect);

			cxClient = rect.right;
			cyClient = rect.bottom;

			srand((unsigned)time(NULL));

			for (int v = 0; v < iNumVerts; v++) {
				verts[v].x = RandInt(0, cxClient);
				verts[v].y = RandInt(0, cyClient);
			}

			break;
		case WM_KEYUP:
			switch (wParam) {
				case VK_SPACE:
					for (int v = 0; v < iNumVerts; v++) {
						verts[v].x = RandInt(0, cxClient);
						verts[v].y = RandInt(0, cyClient);
					}
					InvalidateRect(hwnd, NULL, TRUE);
					UpdateWindow(hwnd);
					break;
				case VK_ESCAPE:
					PostQuitMessage(0);
					break;
			}
			break;
		case WM_PAINT:	
			PAINTSTRUCT ps;
			BeginPaint(hwnd, &ps);

			OldPen = (HPEN)SelectObject(ps.hdc, BluePen);
			OldBrush = (HBRUSH)SelectObject(ps.hdc, RedBrush);

			Polygon(ps.hdc, verts, iNumVerts);

			SelectObject(ps.hdc, OldPen);
			SelectObject(ps.hdc, OldBrush);

			EndPaint(hwnd, &ps);
			break;
		case WM_SIZE:
			cxClient = LOWORD(lParam);
			cyClient = HIWORD(lParam);

			for (int v = 0; v < iNumVerts; v++) {
				verts[v].x = RandInt(0, cxClient);
				verts[v].y = RandInt(0, cyClient);
			}
			break;
		case WM_DESTROY:
			DeleteObject(BluePen);
			DeleteObject(OldPen);

			DeleteObject(RedBrush);
			DeleteObject(OldBrush);

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
