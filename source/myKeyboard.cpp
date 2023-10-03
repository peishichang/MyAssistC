#include "myHead.h"


DWORD JDownTime0 = 0;
DWORD JDownTime1 = 0;
volatile bool JDownFlag = false;
volatile bool JSentFlag = false;                 //程序按下的空格标志，当此标志为假时为硬件空格，当此标志问真时为软件空格

DWORD SpaceDownTime0 = 0;
DWORD SpaceDownTime1 = 0;
bool SpaceDownFlag = false;
bool SpaceSentFlag = false;                     //程序按下的空格标志，当此标志为假时为硬件空格，当此标志问真时为软件空格

bool MultiKeysFlag = false;                     //程序使用组合键标志

//J处理函数
void thread_j(DWORD tj)
{
    while ( GetTickCount() - tj < 160)
    {
        ; 
    }
    if (JDownFlag == true)
    {
        JSentFlag = true;
        keybd_event('J',0,0,0);        //J 按下
        keybd_event('J',0,2,0);        //J 弹起
        JSentFlag = false;
        JDownFlag = false;
    }
}

// 切换输入法函数
void switchInputMethod(uint32_t lang)
{
    HWND hwnd = GetForegroundWindow();
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
	KBDLLHOOKSTRUCT* ks = (KBDLLHOOKSTRUCT*)lParam;		// 包含低级键盘输入事件信息

	DWORD code = ks->vkCode;
    DWORD t = ks->time;
    uint32_t KeyState = 0;
    uint32_t RetFlag = CallNextHookEx(NULL, nCode, wParam, lParam);

    if (wParam == WM_KEYDOWN)
        KeyState = 0;
    else if (wParam == WM_KEYUP)
        KeyState = 2;
    
    if (wParam == WM_KEYDOWN and code == VK_SPACE and SpaceSentFlag == false)
    {
        RetFlag = 1;
        SpaceDownFlag = true;
        if (SpaceDownTime0 == 0)
        {
            SpaceDownTime0 = t;
        }
    }
    else if (wParam == WM_KEYUP and code == VK_SPACE and SpaceSentFlag == false)
    {
        RetFlag = 1;
        SpaceDownFlag = false;
        SpaceDownTime1 = t - SpaceDownTime0;
        SpaceDownTime0 = 0;
        if (SpaceDownTime1 < 200 and MultiKeysFlag == false) 
        {
            SpaceSentFlag = true;
            keybd_event(VK_SPACE,0,0,0);        //space 按下
            keybd_event(VK_SPACE,0,2,0);        //space 弹起
            SpaceSentFlag = false;
        }
    }
    else if (SpaceDownFlag == false and wParam == WM_KEYDOWN and code == 'J' and JSentFlag == false)
    {
        RetFlag = 1;
        JDownTime1 = t - JDownTime0; 
        cout << JDownTime1 << endl;
        JDownTime0 = t;
    }
    else if (SpaceDownFlag == false and wParam == WM_KEYUP and code == 'J' and JSentFlag == false)
    {
        RetFlag = 1;
        thread th2 (thread_j, t);
        JDownFlag = true;
        th2.detach();
        if (JDownTime1 < 160)
        {
            keybd_event(VK_ESCAPE,0,0,0);        //esc 按下
            keybd_event(VK_ESCAPE,0,2,0);        //esc 弹起
            switchInputMethod(0x409);   //英文输入法
            JDownFlag = false;
        }
    }

    if(SpaceDownFlag == true)
    {
        if (code == 'J')       //key J
        {
            MultiKeysFlag = true;
            RetFlag = 1;
            keybd_event(VK_DOWN,0,KeyState,0);
        }
        else if (code == 'K')       //key K 
        {
            MultiKeysFlag = true;
            RetFlag = 1;
            keybd_event(VK_UP,0,KeyState,0);
        }
        else if ( code == 'L')       //key L 
        {
            MultiKeysFlag = true;
            RetFlag = 1;
            keybd_event(VK_RIGHT,0,KeyState,0);
        }
        else if (code == 'H')       //key H 
        {
            MultiKeysFlag = true;
            RetFlag = 1;
            keybd_event(VK_LEFT,0,KeyState,0);
        }
        else if (code == VK_SPACE)       //key H 
        {
            RetFlag = 1;
        }
        else 
        {
            MultiKeysFlag = false;
            RetFlag = CallNextHookEx(NULL, nCode, wParam, lParam);
        }
    }
    if(JDownFlag == true and JSentFlag == false and code != 'J')
    {
        JDownFlag = false;
        JSentFlag = true;
        keybd_event('J',0,0,0);        //J 按下
        keybd_event('J',0,2,0);        //J 弹起
        JSentFlag = false;
        RetFlag = CallNextHookEx(NULL, nCode, wParam, lParam);
    }
    return RetFlag;             //当键盘输入不是组合输入时，返回CallNextHookEx(NULL, nCode, wParam, lParam); 否则返回1
}