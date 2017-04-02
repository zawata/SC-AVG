#pragma once

#define _RESOURCES L"Resources.dll"

#define WM_TRAYMESSAGE (WM_USER + 1)

class Window
{
public:
	static NOTIFYICONDATA shellData;
	static HICON hIcon;
	static HMODULE hResource;
	static HACCEL hAccelTable;

	static void initHandles(void);
	static void destroyHandles(void);

	void LoadResources();

	static void loadIcons(HWND hDlg);
	static void unloadIcons(HWND hDlg);


};

