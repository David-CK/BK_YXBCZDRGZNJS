#include <windows.h>

int WINAPI WinMain(HINSTANCE hInstance,
					HINSTANCE hPrevInstance,
					LPSTR lpCmdLine,
					int nCmdShow)
{
	MessageBoxA(NULL, "Hello World! ", "MsgBox", 0);

	return 0;
}
