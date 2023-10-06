#include "myHead.h"

extern uint8_t inputState;
extern int32_t widthSc;
extern int32_t heightSc;
extern int32_t widthMW;
extern int32_t heightMW;

char* debugText;        //����һ�������ڴ������������Ϣ���ַ���������

HDC hdcMem;             //����һ���ڴ�HDC�����������ڴ��ڻ���ͼ��
HBITMAP hBitmap;        //����һ��λͼ����
HDC hdc;                //����һ��hdc������ ���ڴ����Ļdc 
HBRUSH hbrush0;         //��Ļ��ˢ����ͬ��ɫ�ò�ͬ��ˢ��û�б�ŵ��Ǵ��ݻ�ˢ�ı���
HBRUSH hbrush1;
HBRUSH hbrush2;
HBRUSH hbrush3;
HBRUSH hbrush;
//ˢ�������ڼ�����״̬�ĺ��������̣߳�
void thread_ShowMW(HWND hwndMW) 
{
    debugText = new char[100];                  //ΪDEBUGText�����ڴ�
    strcpy(debugText,"");                       //����debugText������
    HDC hdc = GetDC(NULL);                      //�õ���ĻDC  ������ȡ��Ļ�ֱ���
	widthSc = GetDeviceCaps(hdc, HORZRES);      // ��  
	heightSc = GetDeviceCaps(hdc, VERTRES);     // ��   
	ReleaseDC(NULL, hdc);                       // �ͷ�DC
    // �������ڵ��豸������
    hdc = GetDC(hwndMW);
    // ����͸��λͼ
    hBitmap = CreateCompatibleBitmap(hdc, widthMW, heightMW);
    hdcMem = CreateCompatibleDC(hdc);
    SelectObject(hdcMem, hBitmap);
    // ����͸������
    BLENDFUNCTION blend = { 0 };
    blend.BlendOp = AC_SRC_OVER;
    blend.BlendFlags = 0;
    blend.AlphaFormat = AC_SRC_ALPHA;
    blend.SourceConstantAlpha = 100; // ͸���ȣ�0-255��255Ϊ��ȫ��͸����
    RECT rcClient;
    hbrush0 = (HBRUSH)(CreateSolidBrush(RGB(255,255,255)));     //������ͬ��ɫ�ı�ˢ
    hbrush1 = (HBRUSH)(CreateSolidBrush(RGB(0,255,0)));
    hbrush2 = (HBRUSH)(CreateSolidBrush(RGB(0,0,255)));
    hbrush3 = (HBRUSH)(CreateSolidBrush(RGB(255,0,0)));
    hbrush = hbrush1;
    GetClientRect(hwndMW, &rcClient);
    FillRect(hdcMem, &rcClient, hbrush);                    //�ò�ͬ��ɫ�ı�ˢ���ƾ��ο�
    // ʹ��UpdateLayeredWindow�������ô���Ϊ͸������
    POINT ptDst = { widthSc-widthMW, heightSc - heightMW }; //����λ��
    SIZE sizeWnd = { widthMW, heightMW };                   //���ڴ�С
    POINT ptSrc = { 0, 0 };                                 //��ͼ��λ��
    UpdateLayeredWindow(hwndMW, hdc, &ptDst, &sizeWnd, hdcMem, &ptSrc, 0, &blend, ULW_ALPHA);
    
    while (true)
    {   //�������������״̬ѡ���ˢ�����������Ƿ���ʾ
        if (inputState == COMMAND_KBST)
        {
            hbrush = hbrush1;
            ShowWindow(hwndMW, SW_NORMAL);  //SW_HIDE���Խ���������
        }
        else if(inputState == SPACE_KBST)
        {
            hbrush = hbrush2;
            ShowWindow(hwndMW, SW_NORMAL);
        }
        else if(inputState == CAPITAL_KBST)
        {
            hbrush = hbrush3;
            ShowWindow(hwndMW, SW_NORMAL);
        }
        else if(inputState == NORMAL_KBST)
        {
            hbrush = hbrush0;
            ShowWindow(hwndMW, SW_NORMAL);
        }
        FillRect(hdcMem, &rcClient, hbrush);          //��������״̬��Ӧ��ɫ�ı�ˢ������
        if (debugText != 0)
        {
            // string str1 = to_string(inputState); debugText��ֵ����
            // strcpy(debugText, str1.data());
            TextOut(hdcMem, 0, 0, LPCSTR(debugText), strlen(debugText));//�ڴ������������Ϣ����
        }
        UpdateLayeredWindow(hwndMW, hdc, NULL, &sizeWnd, hdcMem, &ptSrc, 0, &blend, ULW_ALPHA);
        SetWindowPos(hwndMW, HWND_TOPMOST, widthSc-widthMW, heightSc-heightMW, widthMW, heightMW, SWP_NOMOVE | SWP_NOSIZE);
        UpdateWindow(hwndMW);
        Sleep(10);
    }
}
LRESULT CALLBACK WndProc(HWND hwndMW, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
        case WM_CREATE:
        {
            thread th2 (thread_ShowMW, hwndMW);
            th2.detach();
            break;
        }
        case WM_DESTROY:
            PostQuitMessage(0);
            // �ͷ���Դ
            DeleteDC(hdcMem);
            DeleteObject(hBitmap);
            ReleaseDC(hwndMW, hdc);
            DeleteObject(hbrush);
            DeleteObject(hbrush1);
            DeleteObject(hbrush2);
            break;
        default:
            return DefWindowProc(hwndMW, msg, wParam, lParam);
            break;
    }
    return 0;
}