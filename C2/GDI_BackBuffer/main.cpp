#include <windows.h>
#include <time.h>

#include "defines.h"
#include "utils.h"

char *g_szApplicationName = "Bouncing Balls - Without Backbuffer";
char *g_szWindowClassName = "MyWindowClass";

struct SBall
{
	int posX;
	int posY;

	int velX;
	int velY;

	SBall() {}
};

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

	static SBall *balls = new SBall[NUM_BALLS];

	switch (msg) {
		case WM_CREATE:
			RECT rect;

			GetClientRect(hwnd, &rect);

			cxClient = rect.right;
			cyClient = rect.bottom;

			srand((unsigned)time(NULL));

			for (int i = 0; i < NUM_BALLS; i++) {
				balls[i].posX = RandInt(0, cxClient);
				balls[i].posY = RandInt(0, cyClient);
				balls[i].velX = RandInt(0, MAX_VELOCITY);
				balls[i].velY = RandInt(0, MAX_VELOCITY);
			}
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

			OldPen = (HPEN)SelectObject(ps.hdc, BluePen);
			OldBrush = (HBRUSH)SelectObject(ps.hdc, RedBrush);

			for (int i = 0; i < NUM_BALLS; i++) {
				if ((balls[i].posX >= cxClient) || (balls[i].posX < 0)) {
					balls[i].velX = -balls[i].velX;
				}

				if ((balls[i].posY >= cyClient) || (balls[i].posY < 0)) {
					balls[i].velY = -balls[i].velY;
				}

				balls[i].posX += balls[i].velX;
				balls[i].posY += balls[i].velY;

				Ellipse(ps.hdc,
						balls[i].posX - RADIUS,
						balls[i].posY - RADIUS,
						balls[i].posX + RADIUS,
						balls[i].posY + RADIUS);
			}

			SelectObject(ps.hdc, OldPen);
			SelectObject(ps.hdc, OldBrush);

			EndPaint(hwnd, &ps);

			Sleep(10);
			break;

		case WM_SIZE:
			cxClient = LOWORD(lParam);
			cyClient = HIWORD(lParam);
			break;

		case WM_DESTROY:
			DeleteObject(BluePen);
			DeleteObject(OldPen);

			DeleteObject(RedBrush);
			DeleteObject(OldBrush);

			delete[] balls;

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

	bool bDone = false;
	MSG msg;
	
	while (!bDone) {
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			if (msg.message == WM_QUIT) {
				bDone = true;
			}
			else {
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		InvalidateRect(hWnd, NULL, TRUE);
		UpdateWindow(hWnd);
	}

	UnregisterClassA(g_szWindowClassName, winclass.hInstance);

	return msg.wParam;
}
