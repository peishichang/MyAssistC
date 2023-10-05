#include "myHead.h"

HWND hwndMW;                    //程序主窗口句柄
volatile int32_t widthSc = 1920;
volatile int32_t heightSc = 1080;
volatile int32_t widthMW = 300;
volatile int32_t heightMW = 30;

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    //设置键盘钩子的参数
	HINSTANCE hM = GetModuleHandle(NULL), hK = GetModuleHandle(NULL);
	HHOOK g_Hook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProc, hK, 0);	//WH_KEYBOARD_LL 13 
    HHOOK g_Hook2 = SetWindowsHookEx(WH_MOUSE_LL, MouseProc, hM, 0);	//WH_MOUSE_LL 14 
    //创建子窗口的参数
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

    //启动信号获取线程
    thread th3 (getSignal, hwndMW);
    th3.detach();

    //以下五句是调出调试信息窗口，正式版本要注释掉
    FILE *stream1;
	AllocConsole();
	freopen_s(&stream1, "CON", "r", stdin);
	freopen_s(&stream1, "CON", "w", stdout);
	freopen_s(&stream1, "CON", "w", stderr);

    //消息循环，窗口和键盘钩子都需要消息
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