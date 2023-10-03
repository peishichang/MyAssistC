#include "myHead.h"

HDC hdcMem ;
HBITMAP hBitmap;
HDC hdc;
void thread_ShowMW(HWND hwndMW) //刷新主窗口的函数（新线程）
{
    while (true)
    {
        if(GetTickCount64() % 100 == 0 )
        {
            SetWindowPos(hwndMW, HWND_TOPMOST, 0, 0, 100, 100, SWP_NOMOVE | SWP_NOSIZE);
            ShowWindow(hwndMW, SW_NORMAL);
            UpdateWindow(hwndMW);
        }
    }
}
LRESULT CALLBACK WndProc(HWND hwndMW, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
        case WM_CREATE:
        {
            // 创建窗口的设备上下文
            hdc = GetDC(hwndMW);
            //hdc = GetDC(NULL);    //新方法，在新方法里面尝试。

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
            blend.SourceConstantAlpha = 30; // 透明度（0-255，255为完全不透明）

            RECT rcClient;
            GetClientRect(hwndMW, &rcClient);
            FillRect(hdcMem, &rcClient, (HBRUSH)(COLOR_WINDOW + 1));

            // 使用UpdateLayeredWindow函数设置窗口为透明窗口
            POINT ptDst = { 0, 0 };
            SIZE sizeWnd = { width, height };
            POINT ptSrc = { 0, 0 };
            UpdateLayeredWindow(hwndMW, hdc, NULL, &sizeWnd, hdcMem, &ptSrc, 0, &blend, ULW_ALPHA);
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
