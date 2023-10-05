#include "myHead.h"

extern BYTE inputState;
extern int widthSc;
extern int heightSc;
extern int widthMW;
extern int heightMW;
HDC hdcMem;
HBITMAP hBitmap;
HDC hdc;
HBRUSH hbrush1;
HBRUSH hbrush2;
HBRUSH hbrush;
SHORT capState;
//刷新主窗口及各种状态的函数（新线程）
void thread_ShowMW(HWND hwndMW) 
{
    HDC hdc = GetDC(NULL);                      //得到屏幕DC  进而获取屏幕分辨率
	widthSc = GetDeviceCaps(hdc, HORZRES);      // 宽  
	heightSc = GetDeviceCaps(hdc, VERTRES);     // 高   
	ReleaseDC(NULL, hdc);                       // 释放DC
    // 创建窗口的设备上下文
    hdc = GetDC(hwndMW);
    // 创建透明位图
    hBitmap = CreateCompatibleBitmap(hdc, widthMW, heightMW);
    hdcMem = CreateCompatibleDC(hdc);
    SelectObject(hdcMem, hBitmap);
    // 绘制透明背景
    BLENDFUNCTION blend = { 0 };
    blend.BlendOp = AC_SRC_OVER;
    blend.BlendFlags = 0;
    blend.AlphaFormat = AC_SRC_ALPHA;
    blend.SourceConstantAlpha = 100; // 透明度（0-255，255为完全不透明）
    RECT rcClient;
    hbrush1 = (HBRUSH)(CreateSolidBrush(RGB(0,255,0)));
    hbrush2 = (HBRUSH)(CreateSolidBrush(RGB(0,0,255)));
    hbrush = hbrush1;
    GetClientRect(hwndMW, &rcClient);
    FillRect(hdcMem, &rcClient, hbrush);
    // 使用UpdateLayeredWindow函数设置窗口为透明窗口
    POINT ptDst = { widthSc-widthMW, heightSc - heightMW };
    SIZE sizeWnd = { widthMW, heightMW };
    POINT ptSrc = { 0, 0 };
    UpdateLayeredWindow(hwndMW, hdc, &ptDst, &sizeWnd, hdcMem, &ptSrc, 0, &blend, ULW_ALPHA);
    
    while (true)
    {
        capState = GetKeyState(VK_CAPITAL);
        if(capState == 0)
            inputState = 0;
        else if(capState == 1 and inputState == 0)
            inputState = 1;
        if (inputState == 1)
        {
            hbrush = hbrush1;
            ShowWindow(hwndMW, SW_NORMAL);
        }
        else if(inputState == 2)
        {
            hbrush = hbrush2;
            ShowWindow(hwndMW, SW_NORMAL);
        }
        else
        {
            hbrush = hbrush1;
            ShowWindow(hwndMW, SW_HIDE);
        }
        if(GetTickCount64() % 50 == 0)
        {   
            FillRect(hdcMem, &rcClient, hbrush); 
            UpdateLayeredWindow(hwndMW, hdc, NULL, &sizeWnd, hdcMem, &ptSrc, 0, &blend, ULW_ALPHA);
            SetWindowPos(hwndMW, HWND_TOPMOST, widthSc-widthMW, heightSc-heightMW, widthMW, heightMW, SWP_NOMOVE | SWP_NOSIZE);
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
            DeleteObject(hbrush);
            DeleteObject(hbrush1);
            DeleteObject(hbrush2);
            break;
        default:
            return DefWindowProc(hwndMW, msg, wParam, lParam);
            break;
    }
    return 0;
}
