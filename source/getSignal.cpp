#include "myHead.h"

extern uint8_t inputState;

volatile HWND hwndFW;           //ǰ̨����ھ��
volatile int16_t capState;      //��ȡ���̴�Сд״̬

CHAR str1[MAX_PATH] = {0};
volatile bool vimFlag = false;  //�Ƿ���VIMģʽ�б༭

void getSignal(HWND hwndMW)
{
    while(true)
    {
        capState = GetKeyState(VK_CAPITAL);
        if ((!capState and inputState == CAPITAL_KBST) or (capState and inputState != CAPITAL_KBST))
        {
            sendKey(VK_CAPITAL);
        }
        hwndFW = GetForegroundWindow();         //��ȡ��ǰ����ھ��
        GetWindowTextA(hwndFW, str1, MAX_PATH); //��ȡ��ǰ����ڵı��⣬�ŵ�str1
        auto position {string(str1).find("Visual Studio Code")};    //�жϵ�ǰ���������Ƿ���VISUAL STUDIO CODE
        if(position != string::npos)
            vimFlag = true;
        else
            vimFlag = false; 
        Sleep(10);
    }
}