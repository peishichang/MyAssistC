#include "myHead.h"

extern uint8_t inputState;
extern int32_t widthSc;
extern int32_t heightSc;
extern int32_t widthMW;
extern int32_t heightMW;

HDC hdcMem;
HBITMAP hBitmap;
HDC hdc;
HBRUSH hbrush0;
HBRUSH hbrush1;
HBRUSH hbrush2;
HBRUSH hbrush3;
HBRUSH hbrush;
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
    hbrush0 = (HBRUSH)(CreateSolidBrush(RGB(255,255,255)));
    hbrush1 = (HBRUSH)(CreateSolidBrush(RGB(0,255,0)));
    hbrush2 = (HBRUSH)(CreateSolidBrush(RGB(0,0,255)));
    hbrush3 = (HBRUSH)(CreateSolidBrush(RGB(255,0,0)));
    hbrush = hbrush1;
    GetClientRect(hwndMW, &rcClient);
    FillRect(hdcMem, &rcClient, hbrush);
    // 使用UpdateLayeredWindow函数设置窗口为透明窗口
    POINT ptDst = { widthSc-widthMW, heightSc - heightMW };
    SIZE sizeWnd = { widthMW, heightMW };
    POINT ptSrc = { 0, 0 };
    UpdateLayeredWindow(hwndMW, hdc, &ptDst, &sizeWnd, hdcMem, &ptSrc, 0, &blend, ULW_ALPHA);
    
    int64_t timeMS = GetTickCount64();
    while (true)
    {
        if (inputState == COMMAND_KBST)
        {
            hbrush = hbrush1;
            ShowWindow(hwndMW, SW_NORMAL);  //SW_HIDE可以将窗体隐藏
        }
        else if(inputState == SPACE_KBST)
        {
            hbrush = hbrush2;
            ShowWindow(hwndMW, SW_NORMAL);
        }
        else if(inputState == CAPITAL_KBST)
        {
            hbrush = hbrush3;
            ShowWindow(hwndMW, SW_NORMAL);
        }
        else if(inputState == NORMAL_KBST)
        {
            hbrush = hbrush0;
            ShowWindow(hwndMW, SW_NORMAL);
        }
        FillRect(hdcMem, &rcClient, hbrush); 
        UpdateLayeredWindow(hwndMW, hdc, NULL, &sizeWnd, hdcMem, &ptSrc, 0, &blend, ULW_ALPHA);
        SetWindowPos(hwndMW, HWND_TOPMOST, widthSc-widthMW, heightSc-heightMW, widthMW, heightMW, SWP_NOMOVE | SWP_NOSIZE);
        UpdateWindow(hwndMW);
        Sleep(10);
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