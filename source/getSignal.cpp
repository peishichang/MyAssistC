#include "myHead.h"

extern uint8_t inputState;

volatile HWND hwndFW;           //前台程序口句柄
volatile int16_t capState;      //获取键盘大小写状态

CHAR str1[MAX_PATH] = {0};
volatile bool vimFlag = false;  //是否在VIM模式中编辑

void getSignal(HWND hwndMW)
{
    while(true)
    {
        capState = GetKeyState(VK_CAPITAL);
        if ((!capState and inputState == CAPITAL_KBST) or (capState and inputState != CAPITAL_KBST))
        {
            sendKey(VK_CAPITAL);
        }
        hwndFW = GetForegroundWindow();         //获取当前活动窗口句柄
        GetWindowTextA(hwndFW, str1, MAX_PATH); //获取当前活动窗口的标题，放到str1
        auto position {string(str1).find("Visual Studio Code")};    //判断当前工作窗口是否是VISUAL STUDIO CODE
        if(position != string::npos)
            vimFlag = true;
        else
            vimFlag = false; 
        Sleep(10);
    }
}