#pragma once
 
#ifndef MY_HEAD
#define MY_HEAD

#include <bits/stdc++.h>
#include <windows.h>
#include <winuser.h>
#include <tchar.h>

using namespace std;

LRESULT CALLBACK MouseProc(int nCode, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);


#endif