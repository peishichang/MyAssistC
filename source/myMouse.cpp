#include "myHead.h"

LRESULT CALLBACK MouseProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	/*
	typedef struct tagMOUSEHOOKSTRUCT {
		POINT   pt;					// Point数据
		HWND    hwnd;				// 接收鼠标消息的窗体的句柄
		UINT    wHitTestCode;		// 指定点击测试值
		ULONG_PTR dwExtraInfo;		// 指定和该消息相关联的附加信息。
	} MOUSEHOOKSTRUCT, FAR* LPMOUSEHOOKSTRUCT, * PMOUSEHOOKSTRUCT;
	*/


	MOUSEHOOKSTRUCT* ms = (MOUSEHOOKSTRUCT*)lParam;
	POINT pt = ms->pt;

	char state[20] = "未识别";

	if (wParam == WM_LBUTTONDOWN)	// 其余回到文档去找
	{
		cout << "左键按下" << endl;
	}
	else if (wParam == WM_LBUTTONUP)	
	{
		cout << "左键抬起" << endl;
	}
	else if (wParam == WM_MOUSEMOVE)
	{
		cout << "移动" << endl;
	}
	else if (wParam == WM_RBUTTONDOWN)
	{
		cout << "右键按下" << endl;
	}
	else if (wParam == WM_RBUTTONUP)
	{
		cout << "右键抬起" << endl;
	}



	//return 1;	// 吃掉消息
	return CallNextHookEx(NULL, nCode, wParam, lParam);

}