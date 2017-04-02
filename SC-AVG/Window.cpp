#include "stdafx.h"
#include "Window.h"
#include "Util.h"
#include "..//Resources/resource.h"

NOTIFYICONDATA Window::shellData;
HICON          Window::hIcon;
HMODULE        Window::hResource;
HACCEL         Window::hAccelTable;

void Window::initHandles(void)
{
	hResource = LoadLibraryEx(_RESOURCES, NULL, LOAD_LIBRARY_AS_DATAFILE);
	if (!hResource)
		Util::ErrorExit(L"LoadLibraryEx");
	hAccelTable = LoadAccelerators(hResource, MAKEINTRESOURCE(IDC_SCREENAVERAGER));
	if (!hAccelTable)
		Util::ErrorExit(L"LoadLibraryEx");
}

void Window::destroyHandles(void)
{
	DestroyAcceleratorTable(hAccelTable);
	FreeLibrary(hResource);
}

void Window::loadIcons(HWND hDlg)
{
	//Find and Load Icon
	hIcon = LoadIcon(hResource, MAKEINTRESOURCE(MAIN_ICON));
	if (!hIcon)
		Util::ErrorExit(L"LoadIcons");

	shellData.cbSize = sizeof(shellData);
	shellData.dwInfoFlags = NIIF_INFO;
	shellData.hIcon = hIcon;
	shellData.hWnd = hDlg;
	wcscpy_s(shellData.szTip, L"Monitor: All");
	shellData.uCallbackMessage = WM_TRAYMESSAGE;
	shellData.uFlags = shellData.uFlags | NIF_MESSAGE | NIF_ICON | NIF_TIP;
	shellData.uID = 100;
	shellData.uVersion = NOTIFYICON_VERSION;

	//Set Icons
	SendMessage(hDlg, WM_SETICON, ICON_BIG, (LPARAM)Window::hIcon);
}

void Window::unloadIcons(HWND hDlg)
{
	DestroyIcon(hIcon);
}
