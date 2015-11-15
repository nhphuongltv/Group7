// ProjectG3DLL.cpp : Defines the exported functions for the DLL application.
//
// @Author: Nguyen Hoang Phuong 

#include "stdafx.h"
#include <iostream>
#define MYWM_SPEAK 10000
#define MYWM_STOP 10001

HHOOK hHookKey;
HINSTANCE hInst;
bool isCtrl, isSolving;
TCHAR *str;
HWND hAppWnd;

LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam);

EXPORT void InstallHook(HWND hWnd){
	hAppWnd = hWnd;
	hHookKey = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProc, hInst, 0);
	isCtrl = isSolving = false;
}

void UnInstallHook(){
	UnhookWindowsHookEx(hHookKey);
}

void OnCopyAndSpeak()
{
	INPUT ip;
	ip.type = INPUT_KEYBOARD;
	ip.ki.wScan = 0;
	ip.ki.time = 0;
	ip.ki.dwExtraInfo = 0;

	ip.ki.wVk = VK_CONTROL;
	ip.ki.dwFlags = 0; // 0 for key press
	SendInput(1, &ip, sizeof(INPUT));

	// Press the "V" key
	ip.ki.wVk = 'C';
	ip.ki.dwFlags = 0; // 0 for key press
	SendInput(1, &ip, sizeof(INPUT));

	//// Release the "V" key
	//ip.ki.wVk = 'C';
	//ip.ki.dwFlags = KEYEVENTF_KEYUP;
	//SendInput(1, &ip, sizeof(INPUT));

	//// Release the "Ctrl" key
	//ip.ki.wVk = VK_CONTROL;
	//ip.ki.dwFlags = KEYEVENTF_KEYUP;
	//SendInput(1, &ip, sizeof(INPUT));

	OpenClipboard(NULL);
	HGLOBAL hglb = GetClipboardData(CF_UNICODETEXT);
	TCHAR *pchData = (TCHAR*)GlobalLock(hglb);
	CloseClipboard();

	if (pchData){
		str = new TCHAR[wcslen(pchData) + 10];
		wcscpy(str, pchData);
		GlobalUnlock(hglb);

		//MessageBox(hAppWnd, str, L"Get Text", MB_OK);

		std::wcout << str << std::endl;
		

		delete[] str;
	}
}

LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (nCode < 0)
		return CallNextHookEx(NULL, nCode, wParam, lParam);

	KBDLLHOOKSTRUCT *info = (KBDLLHOOKSTRUCT*)lParam;
	if (wParam == WM_KEYDOWN && GetAsyncKeyState(VK_CONTROL) && info->vkCode == 'E')
	{
		WCHAR* text = L"Cause my heart breaks a little, when I hear your name!"; 
		SendMessage(hAppWnd, MYWM_SPEAK, 0, (LPARAM)text);
		std::cout << "Speak!" << std::endl;
		return 1;

		//OnCopyAndSpeak();
		//return 1;
	}
	return CallNextHookEx(hHookKey, nCode, wParam, lParam);
}



//LRESULT EXPORT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
//{
//	if (nCode < 0)
//		return CallNextHookEx(NULL, nCode, wParam, lParam);
//
//	if (isSolving)
//		return CallNextHookEx(NULL, nCode, wParam, lParam);
//
//	KBDLLHOOKSTRUCT *info = (KBDLLHOOKSTRUCT*)lParam;
//
//	if (wParam == WM_KEYDOWN){
//		if (info->vkCode == VK_LCONTROL || info->vkCode == VK_RCONTROL)
//			isCtrl = true, PostMessage(GetFocus(), WM_COPY, 0, 0);
//		if ((info->vkCode == VK_LMENU || info->vkCode == VK_RMENU) && isCtrl){
//			isSolving = true;
//
//			GUITHREADINFO lpgui;
//			HWND hWnd = NULL;
//			lpgui.cbSize = sizeof(GUITHREADINFO);
//			if (GetGUIThreadInfo(NULL, &lpgui))
//				hWnd = lpgui.hwndFocus;
//
//			/*
//			HWND FgHwnd = GetForegroundWindow();
//			DWORD lpdwProcessId;
//			DWORD td = GetWindowThreadProcessId(FgHwnd, &lpdwProcessId);
//			AttachThreadInput(td, GetCurrentThreadId(), TRUE);
//			HWND hWnd = GetFocus();
//			*/
//
//			SendMessage(hWnd, WM_COPY, 0, 0);
//			OpenClipboard(NULL);
//			HGLOBAL hglb = GetClipboardData(CF_UNICODETEXT);
//			TCHAR *pchData = (TCHAR*)GlobalLock(hglb);
//			CloseClipboard();
//
//			if (pchData){
//				str = new TCHAR[wcslen(pchData) + 10];
//				wcscpy(str, pchData);
//				GlobalUnlock(hglb);
//
//				MessageBox(GetForegroundWindow(), str, L"Get Text", MB_OK);
//				delete[] str;
//			}
//
//			isSolving = false;
//			return CallNextHookEx(NULL, nCode, wParam, lParam);
//		}
//	}
//
//	if (wParam == WM_KEYUP)
//	if (info->vkCode == VK_LCONTROL || info->vkCode == VK_RCONTROL)
//		isCtrl = false;
//
//	return CallNextHookEx(NULL, nCode, wParam, lParam);
//}