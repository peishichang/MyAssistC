#include "myHead.h"

extern BYTE inputState;
HDC hdcMem ;
HBITMAP hBitmap;
HDC hdc;
HBRUSH hbrush1;
HBRUSH hbrush2;
HBRUSH hbrush;
void thread_ShowMW(HWND hwndMW) //刷新主窗口的函数（新线程）
{
    // 创建窗口的设备上下文
    hdc = GetDC(hwndMW);

    // 创建透明位图
    int width = 100;
    int height = 100;
    hBitmap = CreateCompatibleBitmap(hdc, width, height);

    // 创建与位图相关联的设备上下文
    hdcMem = CreateCompatibleDC(hdc);
    SelectObject(hdcMem, hBitmap);

    // 绘制透明背景
    BLENDFUNCTION blend = { 0 };
    blend.BlendOp = AC_SRC_OVER;
    blend.BlendFlags = 0;
    blend.AlphaFormat = AC_SRC_ALPHA;
    blend.SourceConstantAlpha = 100; // 透明度（0-255，255为完全不透明）

    RECT rcClient;
    hbrush1 = (HBRUSH)(CreateSolidBrush(RGB(181,230,181)));
    hbrush2 = (HBRUSH)(CreateSolidBrush(RGB(0,0,255)));
    hbrush = hbrush1;
    GetClientRect(hwndMW, &rcClient);
    FillRect(hdcMem, &rcClient, hbrush);

    // 使用UpdateLayeredWindow函数设置窗口为透明窗口
    POINT ptDst = { 0, 0 };
    SIZE sizeWnd = { width, height };
    POINT ptSrc = { 0, 0 };
    UpdateLayeredWindow(hwndMW, hdc, NULL, &sizeWnd, hdcMem, &ptSrc, 0, &blend, ULW_ALPHA);
    while (true)
    {
        if (inputState == 1)
        {
            hbrush = hbrush1;
        }
        else if(inputState == 2)
        {
            hbrush = hbrush2;
        }
        if(GetTickCount64() % 50 == 0 )
        {
            FillRect(hdcMem, &rcClient, hbrush); 
            UpdateLayeredWindow(hwndMW, hdc, NULL, &sizeWnd, hdcMem, &ptSrc, 0, &blend, ULW_ALPHA);
            SetWindowPos(hwndMW, HWND_TOPMOST, 0, 0, 100, 100, SWP_NOMOVE | SWP_NOSIZE);
            UpdateWindow(hwndMW);
        }
    }
    delete(hBitmap);
    delete(hdcMem);
    delete(hbrush1);
    delete(hbrush2);
}
LRESULT CALLBACK WndProc(HWND hwndMW, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
        case WM_CREATE:
        {
            thread th2 (thread_ShowMW, hwndMW);
            th2.detach();
            break;
        }
        case WM_DESTROY:
            PostQuitMessage(0);
            // 释放资源
            DeleteDC(hdcMem);
            DeleteObject(hBitmap);
            ReleaseDC(hwndMW, hdc);
            break;
        default:
            return DefWindowProc(hwndMW, msg, wParam, lParam);
            break;
    }
    return 0;
}
