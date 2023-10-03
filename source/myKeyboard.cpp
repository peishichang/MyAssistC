#include "myHead.h"
#define CN 0x804    //输入法中文代码
#define EN 0x409    //输入法英文代码

extern HWND hwndMW;
extern BYTE inputState;

// 切换输入法函数
void switchInputMethod(uint32_t lang)
{
    HWND hwnd = GetForegroundWindow();                     //获取前景程序 hwnd
    SendMessage(hwnd, WM_INPUTLANGCHANGEREQUEST,0, lang);  //0x804 中文，   0x409英文
}


LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	/*
	typedef struct tagKBDLLHOOKSTRUCT {
	DWORD     vkCode;		// 按键代号
	DWORD     scanCode;		// 硬件扫描代号，同 vkCode 也可以作为按键的代号。
	DWORD     flags;		// 事件类型，一般按键按下为 0 抬起为 128。
	DWORD     time;			// 消息时间戳
	ULONG_PTR dwExtraInfo;	// 消息附加信息，一般为 0。
	}KBDLLHOOKSTRUCT,*LPKBDLLHOOKSTRUCT,*PKBDLLHOOKSTRUCT;
	*/


	KBDLLHOOKSTRUCT* ks = (KBDLLHOOKSTRUCT*)lParam;		//消息附加内容，包含低级键盘输入事件信息

	DWORD code = ks->vkCode;                    //键盘代号
    DWORD t = ks->time;                         //消息的时间
    uint32_t RetFlag = CallNextHookEx(NULL, nCode, wParam, lParam);//如果屏蔽消息就设为1


    if (wParam == WM_KEYDOWN and inputState !=0 )
    {
        if (inputState == 1)
        {
            RetFlag = 1;
            switch (code)
            {
                case VK_CAPITAL:
                    RetFlag = CallNextHookEx(NULL, nCode, wParam, lParam);//如果屏蔽消息就设为1
                    break;
                case VK_OEM_3:      //`~
                    inputState = 2;
                break;
                case VK_F3:     
                    switchInputMethod(CN);
                    break;
                case VK_F4:
                    switchInputMethod(EN);
                    break;
                default:
                    break;
            }
        }
        else
        {
            switch (code)
            {
                case VK_OEM_3:      //`~
                    inputState = 1;
                    RetFlag = 1;
                break;
                default:
                    break;
            }
        }
    }
    else if (wParam == WM_KEYUP)
    {
        ;
    }
    return RetFlag;             //当键盘输入不是组合输入时，返回CallNextHookEx(NULL, nCode, wParam, lParam); 否则返回1
}