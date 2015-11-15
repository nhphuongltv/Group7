#pragma once
#include "TTSLib.h" //Lib text to speech win32 api c++ by Hiep IT

#define CONFIG_FILE_NAME "config.txt"
#define DEFAULT_VOICE_IDX 0
#define DEFAULT_VOLUME 100
#define DEFAULT_RATE 10

//Singleton Pattern
class AppController
{
private:
	AppController();
public:
	static AppController* getInstance();
	~AppController();
public:
	HWND hVoiceComboBox, hVolSlideBar, hSpeedSlideBar;
	HWND hWnd;
	HINSTANCE hInst, hInstDLL;
	TTSLib speaker;

	int voiceIdx, volume, rate;
public:
	//Initialize first to store hWnd, hInst to process later
	void Initialize(HWND hWnd, HINSTANCE hInst);

	void initApp();
	void exitApp(); //Do something before exiting the app

	void loadConfig();
	void saveConfig();

	void CreateControl();
	void InstallHook();
	void RemoveHook();

	void setComboBoxData(std::vector<WCHAR*> voices);

public:
	void Speak(WCHAR* text);
	void Stop();
public:
	void OnChangeComboBoxVoice();
	void OnChangeTrackBar(HWND hTrackBar, int pos);
};

