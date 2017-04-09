#include "stdafx.h"

#pragma once

#define _R 0
#define _G 1
#define _B 2
#define _A 3

#define _MULTIPLIER 4.047f

class Monitor
{
private:
	class Device
	{
	private:
		Device() {};
		static struct MonitorDevice
		{
			HDC targetScreen;
			UINT16 width;
			UINT16 height;
		} monitorDevice;
		static bool lockvar;

		static void lock();
		static void unlock();

	public:
		static void setMonitor(HDC hMonitor, LPMONITORINFO monitorinfo);
		static void getWidth(UINT16 *in);
		static void getHeight(UINT16 *in);
		static void getDeviceContext(HDC *in);
	};

	static struct AveragingInfo
	{
		HDC hDest;
		BITMAPINFO bmi;
	} averagingInfo;

public:
	static void initialize();

	static void setDeviceContext(HWND hDlg);
	static BYTE* average();

	static UINT16 getWidth();
	static UINT16 getHeight();
};