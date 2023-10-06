#pragma once


#define NORMAL_KBST 0
#define COMMAND_KBST 1
#define SPACE_KBST 2
#define CAPITAL_KBST 3
#define CN 0x804    //�������뷨
#define EN 0x409    //Ӣ�����뷨
#define HOLD_KEY_TIME 200

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
void sendKeyM(DWORD codekm, DWORD codek);
void spaceHandle(WPARAM wParam, DWORD codek);
void switchInputMethod(uint32_t lang);