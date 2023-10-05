#include "myHead.h"

extern HWND hwndFW;             //前台窗口程序句柄
extern bool vimFlag;

volatile uint8_t inputState;
volatile uint8_t lastInputState;
volatile bool sentKeyFlag = false;
volatile LRESULT RetFlag = 1;
DWORD spaceDownTime = 0;
DWORD spaceUpTime = 0;
DWORD capDownTime = 0;
DWORD capUpTime = 0;
const DWORD holdKeyTime = 200;
bool spaceReactFlag = false;
bool capDownFlag = false;
//切换输入法函数
void switchInputMethod(uint32_t lang)
{
    SendMessage(hwndFW, WM_INPUTLANGCHANGEREQUEST,0, lang);  //0x804中文，0x409英文
}
//发送按键函数
void sendKeyS(DWORD codeks[], int len)
{
    sentKeyFlag = true;
    for (int i = 0; i < len; i++)
    {
        keybd_event(codeks[i],0,0,0);        //按下
        keybd_event(codeks[i],0,2,0);        //弹起
    }
    
    sentKeyFlag = false;
}
void sendKey(uint64_t codek)
{
    sentKeyFlag = true;
    keybd_event(codek,0,0,0);        //按下
    keybd_event(codek,0,2,0);        //弹起
    sentKeyFlag = false;
}
//键盘钩子回调函数
LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    RetFlag = CallNextHookEx(NULL, nCode, wParam, lParam);//键盘消息处理，置1吞掉消息，返回这一句是将消失返回消息池
    if (sentKeyFlag == false){          //判断是否是程序本身发出键盘消息，而非物理键盘的消息 
	KBDLLHOOKSTRUCT* ks = (KBDLLHOOKSTRUCT*)lParam;
	DWORD codek = ks->vkCode;                            //键盘代码
    DWORD timek = ks->time;                             //消息时间
    if (wParam == WM_KEYDOWN)
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
    if (wParam == WM_KEYUP)
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
            switchInputMethod(EN);
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
    //根据键盘现在的状态，分配按键处理函数。
    switch (inputState)
    {
    case SPACE_KBST:
        spaceHandle(wParam, codek);
        RetFlag = 1;
        break;
    
    default:
        break;
    }
    }
    return RetFlag;
}