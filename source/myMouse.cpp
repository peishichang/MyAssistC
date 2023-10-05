#include "myHead.h"

extern HWND hwndMW;

LRESULT CALLBACK MouseProc(int nCode, WPARAM wParam, LPARAM lParam)
{

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