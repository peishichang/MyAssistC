#include "myHead.h"

extern HWND hwndFW;             //前台程序口句柄
extern bool vimFlag;

volatile uint8_t inputState;
volatile uint8_t lastInputState;
volatile bool sentKeyFlag = false;
DWORD spaceDownTime = 0;
DWORD spaceUpTime = 0;
DWORD capDownTime = 0;
DWORD capUpTime = 0;
const DWORD holdKeyTime = 200;
bool spaceReactFlag = false;
bool capDownFlag = false;
// 切换输入法函数
void switchInputMethod(uint32_t lang)
{
    SendMessage(hwndFW, WM_INPUTLANGCHANGEREQUEST,0, lang);  //0x804 中文，   0x409英文
}
void sendKey(uint64_t codek)
{
    sentKeyFlag = true;
    keybd_event(codek,0,0,0);        //space 按下
    keybd_event(codek,0,2,0);        //space 弹起
    sentKeyFlag = false;
}
//钩子勾到键盘消息的回调函数
LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	KBDLLHOOKSTRUCT* ks = (KBDLLHOOKSTRUCT*)lParam;		//消息附加内容，包含低级键盘输入事件信息
	DWORD codek = ks->vkCode;                            //键盘代号
    DWORD timek = ks->time;                             //消息的时间
    LRESULT RetFlag = CallNextHookEx(NULL, nCode, wParam, lParam);//如果屏蔽消息就设为1
    //wParam == WM_KEYDOWN or WM_KEYDOWN 按键被按下或弹起
    if (wParam == WM_KEYDOWN and sentKeyFlag == false)
    {  
        switch (codek)
        {
        case VK_SPACE:
            RetFlag = 1;
            if (inputState != SPACE_KBST)
            {
                spaceDownTime = timek;
                lastInputState = inputState;
                inputState = SPACE_KBST;
            }
            break;
        case VK_CAPITAL:
            RetFlag = 1;
            if (!capDownFlag)
            {
                capDownTime = timek;
                capDownFlag = true;
            }
            
            break;
        default:
            break;
        }
    }
    if (wParam == WM_KEYUP and sentKeyFlag == false)
    {
        switch (codek)
        {
        case VK_SPACE:
            if (inputState == SPACE_KBST)
            {
                RetFlag = 1;
                spaceUpTime = timek;
                inputState = lastInputState;
                if (spaceUpTime - spaceDownTime < holdKeyTime and spaceReactFlag == false)
                {
                    sendKey(VK_SPACE);
                }
                else if (spaceReactFlag == true)
                {
                    spaceReactFlag = false;
                }
            }
            break;
        case VK_CAPITAL:
            RetFlag = 1;
            if (timek - capDownTime < holdKeyTime and inputState != COMMAND_KBST and inputState != SPACE_KBST and !vimFlag)
            {
                lastInputState = inputState;
                inputState = COMMAND_KBST;
                capDownFlag = false;
            }
            else if (timek - capDownTime < holdKeyTime and inputState != NORMAL_KBST and inputState != SPACE_KBST)
            {
                lastInputState = inputState;
                inputState = NORMAL_KBST;
                capDownFlag = false;
            }
            else if (timek - capDownTime >= holdKeyTime and inputState != CAPITAL_KBST and inputState != SPACE_KBST)
            {
                lastInputState = inputState;
                inputState = CAPITAL_KBST;
                //sendKey(VK_CAPITAL);
                capDownFlag = false;
            }
            else if(vimFlag)
            {
                sendKey(VK_ESCAPE);
                capDownFlag = false;
            }
            break;
        default:
            break;
        }
    }
    //wcout << inputState << endl; 
    return RetFlag;             //当键盘输入不是组合输入时，返回CallNextHookEx(NULL, nCode, wParam, lParam); 否则返回1
}