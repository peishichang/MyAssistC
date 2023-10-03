#include "myHead.h"

volatile HWND hwndMW;
volatile BYTE inputState;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{

	HINSTANCE hM = GetModuleHandle(NULL), hK = GetModuleHandle(NULL);
	HHOOK g_Hook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProc, hK, 0);	//WH_KEYBOARD_LL 13 
    HHOOK g_Hook2 = SetWindowsHookEx(WH_MOUSE_LL, MouseProc, hM, 0);	//WH_MOUSE_LL 14 
    
    
    const char* CLASS_NAME = "MyClass";

    WNDCLASS wc = { 0 };
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    RegisterClass(&wc);

    hwndMW = CreateWindowEx(WS_EX_LAYERED, CLASS_NAME, "MyAssistC", WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP,
        0, 0, 100, 100, NULL, NULL, hInstance, NULL);

    ShowWindow(hwndMW, SW_NORMAL);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    UnhookWindowsHookEx(g_Hook);
    UnhookWindowsHookEx(g_Hook2);
    return 0;
}