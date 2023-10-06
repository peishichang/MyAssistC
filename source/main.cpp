#include "myHead.h"
/// @brief 
HWND hwndMW;                    //���������ھ��
volatile int32_t widthSc = 1920;
volatile int32_t heightSc = 1080;
volatile int32_t widthMW = 300;
volatile int32_t heightMW = 30;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    //���ü��̹��ӵĲ���
	HINSTANCE hM = GetModuleHandle(NULL), hK = GetModuleHandle(NULL);
	HHOOK g_Hook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProc, hK, 0);	//WH_KEYBOARD_LL 13 
    //HHOOK g_Hook2 = SetWindowsHookEx(WH_MOUSE_LL, MouseProc, hM, 0);	//WH_MOUSE_LL 14 //��깳�ӣ���ʱ�ò���ע�͵�����ʡ��Դ
    //�����Ӵ��ڵĲ���
    const char* CLASS_NAME = "MyClass";
    WNDCLASS wc = { 0 };
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    RegisterClass(&wc);
    //WS_EX_LAYERED ���ڰ�͸��ͼ��, WS_EX_TOOLWINDOW����������������ͼ��,WS_POPUP�������ڣ���洰�ڣ�
    hwndMW = CreateWindowEx(WS_EX_LAYERED | WS_EX_TOOLWINDOW, CLASS_NAME, "MyAssistC", WS_POPUP,
        widthSc- widthMW, heightSc - heightMW, widthMW, heightMW, NULL, NULL, hInstance, NULL);
    ShowWindow(hwndMW, SW_NORMAL);

    //�����źŻ�ȡ�߳�
    thread th3 (getSignal, hwndMW);
    th3.detach();
    //��Ϣѭ�������ںͼ��̹��Ӷ���Ҫ��Ϣ
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    UnhookWindowsHookEx(g_Hook);
    //UnhookWindowsHookEx(g_Hook2);                 //��깳�ӣ���������Ч
    return 0;
}