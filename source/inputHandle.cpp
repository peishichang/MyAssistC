#include "myHead.h"
extern bool vimFlag;

void spaceHandle(WPARAM wParam, DWORD codek)
{
    if (wParam == WM_KEYDOWN)
    {
        switch (codek) {
        case 'H':
            sendKey(VK_LEFT);   
            break;
        case 'J':
            sendKey(VK_DOWN);
            break;
        case 'K':
            sendKey(VK_UP);
            break;
        case 'L':
            sendKey(VK_RIGHT);
            break;
        case 'N':
            if (true)
            {
                thread thSWInputM (switchInputMethod, CN);
                thSWInputM.detach();
            }
            break;
        case '1':
            if (vimFlag)
            {
                thread thSendKeys (sendKeyM, VK_CONTROL, VK_NUMPAD1);
                thSendKeys.detach();
            }
            break;
        case '2':
            if (vimFlag)
            {
                thread thSendKeys (sendKeyM, VK_CONTROL, VK_NUMPAD2);
                thSendKeys.detach();
            }
            break;
        case '3':
            if (vimFlag)
            {
                thread thSendKeys (sendKeyM, VK_CONTROL, VK_NUMPAD3);
                thSendKeys.detach();
            }
            break;
        case '4':
            if (vimFlag)
            {
                thread thSendKeys (sendKeyM, VK_CONTROL, VK_NUMPAD4);
                thSendKeys.detach();
            }
            break;
        case '5':
            if (vimFlag)
            {
                thread thSendKeys (sendKeyM, VK_CONTROL, VK_NUMPAD5);
                thSendKeys.detach();
            }
            break;
        case '6':
            if (vimFlag)
            {
                thread thSendKeys (sendKeyM, VK_CONTROL, VK_NUMPAD6);
                thSendKeys.detach();
            }
            break;
        default:
            break;
        }
    }
    
}