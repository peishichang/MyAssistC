#include "myHead.h"

#define HBRG RGB(181, 230, 181)
#define HBRY RGB(250, 249, 222)

extern short capState;

void thread_ShowHDC(HDC hdc) //刷新主窗口的函数（新线程）
{
    int32_t timeT;
    HBRUSH hBrush1 = CreateSolidBrush(HBRG);
    HBRUSH hBrush2 = (HBRUSH)GetStockObject(NULL_BRUSH);
    HBRUSH hBrush;
    HBRUSH oldHBrush;
    
    while (true)
    {
        if ( GetKeyState(VK_CAPITAL) != capState)
        {
            capState = !capState;
            timeT = GetTickCount();
            if (capState)
                hBrush = hBrush1;
            else
                hBrush = hBrush2;
            oldHBrush = (HBRUSH)SelectObject(hdc, hBrush);
            Rectangle(hdc, 0, 0, 2560, 10);
            SelectObject(hdc, oldHBrush);       //恢复旧刷
        }
        if(capState && GetTickCount() - timeT > 500)
        {
            timeT = GetTickCount();
            oldHBrush = (HBRUSH)SelectObject(hdc, hBrush);
            Rectangle(hdc, 0, 0, 2560, 10);
            SelectObject(hdc, oldHBrush);       //恢复旧刷

        }
    }
    DeleteObject(hBrush1); 
    DeleteObject(hBrush2); 
    DeleteObject(oldHBrush); 
}

void DrawABox()
{   
    //HDC hdc = GetWindowDC(GetDesktopWindow());
    HDC hdc = GetDC(NULL); //获取屏幕DC
    thread thhdc (thread_ShowHDC, hdc);
    thhdc.detach();
    HBRUSH hbrush = CreateSolidBrush(NULL_BRUSH);
    HBRUSH hbrush_old = (HBRUSH)SelectObject(hdc, hbrush);
    Rectangle(hdc, 0, 0, 2560, 10);

} 