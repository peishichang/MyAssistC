#include "myHead.h"

volatile HWND hwndMW;
volatile BYTE inputState;
volatile int widthSc = 1920;
volatile int heightSc = 1080;
volatile int widthMW = 200;
volatile int heightMW = 30;

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
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
//WS_EX_LAYERED 用于半透明图层, WS_EX_TOOLWINDOW，用于隐藏任务栏图标,WS_POPUP弹窗窗口（广告窗口）
    hwndMW = CreateWindowEx(WS_EX_LAYERED | WS_EX_TOOLWINDOW, CLASS_NAME, "MyAssistC", WS_POPUP,
        widthSc- widthMW, heightSc - heightMW, widthMW, heightMW, NULL, NULL, hInstance, NULL);

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