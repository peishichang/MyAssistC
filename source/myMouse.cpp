#include "myHead.h"

extern HWND hwndMW;

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
	POINT pt = ms->pt;	//pt.x pt.y  LONG型，鼠标位置信息。
	switch (wParam)
	{
	case WM_LBUTTONDOWN:
	case WM_RBUTTONDOWN:
		break;
	default:
		break;
	}
	//return 1;	// 吃掉消息
	return CallNextHookEx(NULL, nCode, wParam, lParam);

}