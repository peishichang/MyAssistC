#include "myHead.h"

extern uint8_t inputState;
extern int32_t widthSc;
extern int32_t heightSc;
extern int32_t widthMW;
extern int32_t heightMW;

char* debugText;        //定义一个用于在窗口输出调试信息的字符串变量。

HDC hdcMem;             //定义一个内存HDC变量，用于在窗口绘制图像
HBITMAP hBitmap;        //定义一个位图变量
HDC hdc;                //定义一个hdc变量， 用于存放屏幕dc 
HBRUSH hbrush0;         //屏幕画刷，不同颜色用不同画刷，没有编号的是传递画刷的变量
HBRUSH hbrush1;
HBRUSH hbrush2;
HBRUSH hbrush3;
HBRUSH hbrush;
//刷新主窗口及各种状态的函数（新线程）
void thread_ShowMW(HWND hwndMW) 
{
    debugText = new char[100];                  //为DEBUGText申请内存
    strcpy(debugText,"");                       //设置debugText的内容
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
    hbrush0 = (HBRUSH)(CreateSolidBrush(RGB(255,255,255)));     //创建不同颜色的笔刷
    hbrush1 = (HBRUSH)(CreateSolidBrush(RGB(0,255,0)));
    hbrush2 = (HBRUSH)(CreateSolidBrush(RGB(0,0,255)));
    hbrush3 = (HBRUSH)(CreateSolidBrush(RGB(255,0,0)));
    hbrush = hbrush1;
    GetClientRect(hwndMW, &rcClient);
    FillRect(hdcMem, &rcClient, hbrush);                    //用不同颜色的笔刷绘制矩形框
    // 使用UpdateLayeredWindow函数设置窗口为透明窗口
    POINT ptDst = { widthSc-widthMW, heightSc - heightMW }; //窗口位置
    SIZE sizeWnd = { widthMW, heightMW };                   //窗口大小
    POINT ptSrc = { 0, 0 };                                 //绘图的位置
    UpdateLayeredWindow(hwndMW, hdc, &ptDst, &sizeWnd, hdcMem, &ptSrc, 0, &blend, ULW_ALPHA);
    
    while (true)
    {   //根据输入输出的状态选择笔刷，决定窗口是否显示
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
        FillRect(hdcMem, &rcClient, hbrush);          //用于输入状态对应颜色的笔刷画矩形
        if (debugText != 0)
        {
            // string str1 = to_string(inputState); debugText赋值方法
            // strcpy(debugText, str1.data());
            TextOut(hdcMem, 0, 0, LPCSTR(debugText), strlen(debugText));//在窗口输出调试信息文字
        }
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