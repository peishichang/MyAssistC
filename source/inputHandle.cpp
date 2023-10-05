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
            if (vimFlag)
            {
                DWORD codeks[] = {VK_END, DWORD('A'), VK_DIVIDE, VK_DIVIDE};
                thread thSendKeys (sendKeyS, codeks, 4);
                thSendKeys.detach();
            }
            switchInputMethod(CN); 
            break;
        
        default:
            break;
        }
    }
    
}