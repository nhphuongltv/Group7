// ProjectG3.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "ProjectG3.h"
#include <CommCtrl.h>

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name

// My variable
HWND hVoiceComboBox, hVolSlideBar, hSpeedSlideBar;

// My function
void CreateControl(HWND);
void setComboBoxData(TCHAR *strVoice[], int n);

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_PROJECTG3, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_PROJECTG3));

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_PROJECTG3));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_PROJECTG3);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // Store instance handle in our global variable

   hWnd = CreateWindow(szWindowClass, szTitle, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, 300, 100, 400, 200, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;
	TCHAR *str[] = {L"1", L"2"};
	switch (message)
	{
	case WM_CREATE:
		CreateControl(hWnd);
		setComboBoxData(str, 2);
		break;
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code here...
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

void CreateControl(HWND hWnd)
{
	CreateWindow(L"Static", L"Voice:", WS_CHILD | WS_VISIBLE, 20, 20, 75, 20, hWnd, NULL, hInst, NULL);
	hVoiceComboBox = CreateWindow(L"COMBOBOX", L"", CBS_DROPDOWN | WS_CHILD | WS_VISIBLE, 100, 20, 200, 100, hWnd, NULL, hInst, NULL);

	CreateWindow(L"Static", L"Volume:", WS_CHILD | WS_VISIBLE, 20, 60, 75, 20, hWnd, NULL, hInst, NULL);
	hVolSlideBar = CreateWindow(TRACKBAR_CLASS, L"", WS_CHILD | WS_VISIBLE | TBS_AUTOTICKS, 100, 55, 200, 30, hWnd, (HMENU)3, NULL, NULL);
	SendMessage(hVolSlideBar, TBM_SETRANGE, (WPARAM)TRUE, (LPARAM)MAKELONG(0, 100));
	SendMessage(hVolSlideBar, TBM_SETPOS, (WPARAM)TRUE, (LPARAM)50);

	CreateWindow(L"Static", L"Speed:", WS_CHILD | WS_VISIBLE, 20, 100, 75, 20, hWnd, NULL, hInst, NULL);
	hSpeedSlideBar = CreateWindow(TRACKBAR_CLASS, L"", WS_CHILD | WS_VISIBLE | TBS_AUTOTICKS, 100, 95, 200, 30, hWnd, (HMENU)3, NULL, NULL);
	SendMessage(hSpeedSlideBar, TBM_SETRANGE, (WPARAM)TRUE, (LPARAM)MAKELONG(0, 10));
	SendMessage(hSpeedSlideBar, TBM_SETPOS, (WPARAM)TRUE, (LPARAM)5);
}

void setComboBoxData(TCHAR *strVoice[], int n)
{
	for (int i = 0; i < n; i++)
	{
		SendMessage(hVoiceComboBox, CB_ADDSTRING, (WPARAM)MAX_LOADSTRING, (LPARAM)strVoice[i]);
	}
}