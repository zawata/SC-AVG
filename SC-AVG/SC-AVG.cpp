// ScreenAverager.cpp : Defines the entry point for the application.
//

//#include <strsafe.h>

#include "stdafx.h"
#include "../Resources/resource.h"
#include "window.h"
#include "util.h"

INT_PTR CALLBACK DIAG_MAIN_CB(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK DIAG_GET_CB(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);

int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine,  int nCmdShow)
{
	Window::initHandles();

	HWND hDlg = CreateDialogParam(Window::hResource, MAKEINTRESOURCE(DIAG_MAIN), 0, DIAG_MAIN_CB, 0);
	ShowWindow(hDlg, nCmdShow);

	MSG msg;
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, Window::hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int)msg.wParam;
}

bool set;

INT_PTR CALLBACK DIAG_MAIN_CB(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
		case WM_INITDIALOG:
			Window::loadIcons(hDlg);
			break;
		case WM_COMMAND:
			switch (LOWORD(wParam))
			{
// Menu Items
				case ID_FILE_EXIT:
					SendMessage(hDlg, WM_DESTROY, 0, 0);
					break;

				case ID_ABOUT_ABOUT:
					break;

// Window Controls
				case IDC_MONCHECK:
				{
					HWND h_SETBUTTON = GetDlgItem(hDlg, IDC_SETBUTTON);
					HWND h_MONCHECK = GetDlgItem(hDlg, IDC_MONCHECK);
					HWND h_STATUSTEXT = GetDlgItem(hDlg, IDC_STATUSTEXT);
					if (Button_GetCheck(h_MONCHECK))
					{
						Static_SetText(h_STATUSTEXT, L"Use All Monitors");
						Button_Enable(h_SETBUTTON, false);
					}
					else
					{
						Static_SetText(h_STATUSTEXT, L"Use Monitor 0");
						Button_Enable(h_SETBUTTON, true);
					}
					break;
				}
				case IDC_SETBUTTON:
					HWND hWnd = CreateDialogParam(Window::hResource, MAKEINTRESOURCE(DIAG_GET), hDlg, DIAG_GET_CB, 0);
					if (!hWnd)
						Util::ErrorExit(L"LoadIcons");
					ShowWindow(hWnd, SW_SHOW);
					break;
			}
			break;

		case WM_SYSCOMMAND:
			switch (wParam)
			{
				case SC_CLOSE:
				case SC_MINIMIZE:
					ShowWindow(hDlg, SW_HIDE);
					//SetWindowLongPtr(hDlg, GWL_EXSTYLE, GetWindowLongPtr(hDlg, GWL_EXSTYLE) | ~WS_EX_APPWINDOW);
					//ShowWindow(hDlg, SW_SHOW);
					Shell_NotifyIcon(NIM_ADD, &Window::shellData);
					Shell_NotifyIcon(NIM_SETVERSION, &Window::shellData);
					break;
			}
			break;

		case WM_PARENTNOTIFY:
			switch (LOWORD(wParam))
			{
				case WM_XBUTTONDOWN:
					set = false;
				case WM_DESTROY:
					if (set)
					{
						MessageBox(hDlg, L"Alert", L"Get Closed", IDOK);
						HWND hget = (HWND)lParam;
					}
					//
			}

		case WM_TRAYMESSAGE:
			switch (lParam)
			{
				case WM_LBUTTONDBLCLK:
					//ShowWindow(hDlg, SW_HIDE);
					//SetWindowLongPtr(hDlg, GWL_EXSTYLE, GetWindowLongPtr(hDlg, GWL_EXSTYLE) & WS_EX_APPWINDOW);
					ShowWindow(hDlg, SW_RESTORE);
					SetForegroundWindow(hDlg);
					Shell_NotifyIcon(NIM_DELETE, &Window::shellData);
					break;
				default:
					return DefWindowProc(hDlg, uMsg, wParam, lParam);
			}
			break;

		case WM_CLOSE:
			break;

		case WM_DESTROY:
			//DestroyIcon(hIcon);
			DestroyWindow(hDlg);
			PostQuitMessage(0);
			return TRUE;
	}
	return FALSE;
}

INT_PTR CALLBACK DIAG_GET_CB(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
		case WM_COMMAND:
			switch (LOWORD(wParam))
			{
				case IDC_OK:
					SendMessage(hDlg, WM_CLOSE, 0, 0);
			}
		case WM_MOVE:
		{
			HWND h_MONITOR = GetDlgItem(hDlg, IDC_MONITOR);
			//LPCSTR buf = (L"Monitor:")
			Static_SetText(h_MONITOR, L"Monitor:");
			break;
		}
		case WM_CLOSE:
		case WM_DESTROY:
			DestroyWindow(hDlg);
			return TRUE;
	}
	return FALSE;
}