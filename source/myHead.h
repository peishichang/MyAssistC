#pragma once
 
#ifndef MY_HEAD
#define MY_HEAD

#include <windows.h>
#include <winuser.h>
#include <string>
#include<tchar.h>

using namespace std;

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);


#endif