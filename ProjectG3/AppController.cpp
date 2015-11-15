#include "stdafx.h"
#include "AppController.h"
#include "ProjectG3.h"
#include <CommCtrl.h>
#include <windowsx.h>
#define MAX_LOADSTRING 100

AppController::AppController()
{
}


AppController::~AppController()
{
	
}

AppController* AppController::getInstance()
{
	static AppController* _instance = new AppController();
	return _instance;
}

void AppController::Initialize(HWND hWnd, HINSTANCE hInst)
{
	this->hWnd = hWnd;
	this->hInst = hInst;
}

void AppController::initApp()
{
	loadConfig();
	speaker.Initialize(); //Initialize speaker (Text To Speech)
	InstallHook(); //Install hook
	CreateControl();

	setComboBoxData(speaker.GetVoices()); //Set voice to combobox

	ComboBox_SetCurSel(hVoiceComboBox, voiceIdx); //Select first option

	speaker.SetRate(rate - 10);
	speaker.SetVolume(volume);
	speaker.SetVoice(voiceIdx);

	speaker.SpeakAsync(L"Hello world!");
}

void AppController::exitApp()
{
	saveConfig(); //Save voice, volume, rate config
	RemoveHook();
}

void AppController::loadConfig()
{
	FILE* pFile = fopen(CONFIG_FILE_NAME, "r");
	if (pFile == NULL) //If the file is not exists!
	{
		voiceIdx = DEFAULT_VOICE_IDX, rate = DEFAULT_RATE, volume = DEFAULT_VOLUME; //Set the default value
		return;
	}

	fscanf(pFile, "%d %d %d", &voiceIdx, &volume, &rate);
	fclose(pFile);
}

void AppController::saveConfig()
{
	FILE* pFile = fopen(CONFIG_FILE_NAME, "w");
	fprintf(pFile, "%d %d %d", voiceIdx, volume, rate);
	fclose(pFile);
}

void AppController::CreateControl()
{
	CreateWindow(L"Static", L"Voice:", WS_CHILD | WS_VISIBLE, 20, 20, 75, 20, hWnd, NULL, hInst, NULL);
	hVoiceComboBox = CreateWindow(L"COMBOBOX", L"", CBS_DROPDOWNLIST | WS_VSCROLL | WS_HSCROLL | WS_CHILD | WS_VISIBLE, 100, 20, 200, 100, hWnd, (HMENU)IDC_COMBOBOX_VOICE, hInst, NULL);

	CreateWindow(L"Static", L"Volume:", WS_CHILD | WS_VISIBLE, 20, 60, 75, 20, hWnd, NULL, hInst, NULL);
	hVolSlideBar = CreateWindow(TRACKBAR_CLASS, L"", WS_CHILD | WS_VISIBLE | TBS_AUTOTICKS | TBS_HORZ, 100, 55, 200, 30, hWnd, (HMENU)IDC_SLIDEBAR_VOLUME, NULL, NULL);
	SendMessage(hVolSlideBar, TBM_SETRANGE, (WPARAM)TRUE, (LPARAM)MAKELONG(0, 100));
	SendMessage(hVolSlideBar, TBM_SETPOS, (WPARAM)TRUE, (LPARAM)100);

	CreateWindow(L"Static", L"Speed:", WS_CHILD | WS_VISIBLE, 20, 100, 75, 20, hWnd, NULL, hInst, NULL);
	hSpeedSlideBar = CreateWindow(TRACKBAR_CLASS, L"", WS_CHILD | WS_VISIBLE | TBS_AUTOTICKS | TBS_HORZ, 100, 95, 200, 30, hWnd, (HMENU)IDC_SLIDEBAR_SPEED, NULL, NULL);
	SendMessage(hSpeedSlideBar, TBM_SETRANGE, (WPARAM)TRUE, (LPARAM)MAKELONG(0, 20));
	SendMessage(hSpeedSlideBar, TBM_SETPOS, (WPARAM)TRUE, (LPARAM)10);
}

void AppController::InstallHook()
{
	typedef void(*Func)(HWND);
	Func func;

	hInstDLL = LoadLibrary(L"ProjectG3DLL.dll");
	if (hInstDLL){
		func = (Func)GetProcAddress(hInstDLL, "InstallHook");
		if (func)
			func(hWnd);
		else
			MessageBox(hWnd, L"Can't find InstallHook function", L"InstallHook Error", MB_OK);
	}
	else
	{
		MessageBox(hWnd, L"Load ProjectG3DLL.dll failed!", L"InstallHook Error", MB_OK);
	}
}

void AppController::RemoveHook()
{
	if (hInstDLL)
		FreeLibrary(hInstDLL);
}

void AppController::setComboBoxData(std::vector<WCHAR*> voices)
{
	for (size_t i = 0; i < voices.size(); ++i)
		SendMessage(hVoiceComboBox, CB_ADDSTRING, (WPARAM)MAX_LOADSTRING, (LPARAM)(voices[i]));
}

void AppController::Speak(WCHAR* text)
{
	if (FAILED(speaker.SpeakAsync(text)))
	{
		MessageBox(hWnd, L"Speak text failed!", L"Speak Error", MB_OK);
	}
}

void AppController::Stop()
{
	speaker.Stop();
}

void AppController::OnChangeComboBoxVoice()
{
	voiceIdx = ComboBox_GetCurSel(hVoiceComboBox);
	speaker.SetVoice(voiceIdx);
}

void AppController::OnChangeTrackBar(HWND hTrackBar, int pos)
{
	if (hTrackBar == hVolSlideBar)
	{
		volume = pos;
		speaker.SetVolume(volume);
	}
	else if (hTrackBar == hSpeedSlideBar)
	{
		speaker.SetRate(rate - 10); //-10 cause rate from -10 to 10
	}
}