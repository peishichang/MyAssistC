#include "myHead.h"

extern short inputState;

void thread_ShowHDC(HDC hdc) //刷新主窗口的函数（新线程）
{
    int32_t timeT;
    DWORD rgbl = RGB(0, 0 , 0);
    while (true)
    {
        if (GetKeyState(VK_CAPITAL) == 0)
        {
            inputState = 0;
        }
        else
        {
            inputState = 1;
            rgbl = RGB(181, 230, 181);
        }
        timeT = GetTickCount();
        if(timeT % 500 == 0 and timeT % 1000 !=0 and inputState !=0)
        {
            HBRUSH hbrush = CreateSolidBrush(rgbl);
            HBRUSH hbrush_old = (HBRUSH)SelectObject(hdc, hbrush);
            Rectangle(hdc, 0, 0, 2560, 10);
        }
        else if(timeT % 1000 == 0)
        {
            HBRUSH hbrush = CreateSolidBrush(NULL_BRUSH);
            HBRUSH hbrush_old = (HBRUSH)SelectObject(hdc, hbrush);
            Rectangle(hdc, 0, 0, 2560, 10);
        }
    }
}

void DrawABox()
{   
    //HDC hdc = GetWindowDC(GetDesktopWindow());
    HDC hdc = GetDC(NULL); //获取屏幕DC
    thread thhdc (thread_ShowHDC, hdc);
    thhdc.detach();
    HBRUSH hbrush = CreateSolidBrush(NULL_BRUSH);
    HBRUSH hbrush_old = (HBRUSH)SelectObject(hdc, hbrush);
    Rectangle(hdc, 0, 0, 2560, 5);

} 