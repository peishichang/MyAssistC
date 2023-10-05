#pragma once
 
#ifndef MY_HEAD
#define MY_HEAD

    #ifndef MY_KEYBOARD_STATE
    #define MY_KEYBOARD_STATE
        #define NORMAL_KBST 0
        #define COMMAND_KBST 1
        #define SPACE_KBST 2
        #define CAPITAL_KBST 3
    #endif
    #ifndef MY_KEYBOARD_STATE
    #define MY_KEYBOARD_STATE
        #define CN 0x804    //中文输入法
        #define EN 0x409    //英文输入法
        #define HOLD_KEY_TIME 200
    #endif

    #include <bits/stdc++.h>
    #include <windows.h>
    #include <winuser.h>
    #include <tchar.h>

    using namespace std;

    LRESULT CALLBACK MouseProc(int nCode, WPARAM wParam, LPARAM lParam);
    LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam);
    LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
    void getSignal(HWND hwndMW);
    void sendKey(uint64_t codek);

#endif