#include "myHead.h"

extern HWND hwndMW;

LRESULT CALLBACK MouseProc(int nCode, WPARAM wParam, LPARAM lParam)
{

	MOUSEHOOKSTRUCT* ms = (MOUSEHOOKSTRUCT*)lParam;
	POINT pt = ms->pt;	//pt.x pt.y  LONG�ͣ����λ����Ϣ��
	switch (wParam)
	{
	case WM_LBUTTONDOWN:
	case WM_RBUTTONDOWN:
		break;
	default:
		break;
	}
	//return 1;	// �Ե���Ϣ
	return CallNextHookEx(NULL, nCode, wParam, lParam);

}