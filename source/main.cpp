#include "myHead.h"

volatile HWND hwndMW;
volatile short capState;

int main(int argc, char const *argv[])
{
	HINSTANCE hM = GetModuleHandle(NULL), hK = GetModuleHandle(NULL);
	HHOOK g_Hook1 = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProc, hK, 0);	//WH_KEYBOARD_LL 13 
    HHOOK g_Hook2 = SetWindowsHookEx(WH_MOUSE_LL, MouseProc, hM, 0);	//WH_MOUSE_LL 14 

    DrawABox();

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }	
    UnhookWindowsHookEx(g_Hook1);
    UnhookWindowsHookEx(g_Hook2);
    return 0;
}

