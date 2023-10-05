#include "myHead.h"
#define CN 0x804    //输入法中文代码
#define EN 0x409    //输入法英文代码

extern HWND hwndMW;
extern BYTE inputState;
BOOL sentKeyFlag = false;
// 切换输入法函数
void switchInputMethod(uint32_t lang)
{
    HWND hwnd = GetForegroundWindow();                     //获取前景程序 hwnd
    SendMessage(hwnd, WM_INPUTLANGCHANGEREQUEST,0, lang);  //0x804 中文，   0x409英文
}
//钩子勾到键盘消息的回调函数
LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	KBDLLHOOKSTRUCT* ks = (KBDLLHOOKSTRUCT*)lParam;		//消息附加内容，包含低级键盘输入事件信息
	DWORD code = ks->vkCode;                            //键盘代号
    DWORD timek = ks->time;                             //消息的时间
    uint32_t RetFlag = CallNextHookEx(NULL, nCode, wParam, lParam);//如果屏蔽消息就设为1
    //wParam == WM_KEYDOWN or WM_KEYDOWN 按键被按下或弹起
    if (wParam == WM_KEYDOWN and inputState !=0 and sentKeyFlag == false)
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
    return RetFlag;             //当键盘输入不是组合输入时，返回CallNextHookEx(NULL, nCode, wParam, lParam); 否则返回1
}