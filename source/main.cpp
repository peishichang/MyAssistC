#include "myHead.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    const char* CLASS_NAME = "MyClass";

    WNDCLASS wc = { 0 };
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    RegisterClass(&wc);

    HWND hWnd = CreateWindowEx(WS_EX_LAYERED, CLASS_NAME, "MyAssistC", WS_OVERLAPPEDWINDOW,
        0, 0, 100, 100, NULL, NULL, hInstance, NULL);

    ShowWindow(hWnd, SW_NORMAL);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return 0;
}