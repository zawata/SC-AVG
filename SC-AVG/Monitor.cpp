#include "stdafx.h"
#include "Monitor.h"

BOOL CALLBACK enumMonitorProc(HMONITOR hMonitor, HDC hdc, LPRECT rect, LPARAM lParam)
{
	HDC *hm = (HDC *)lParam;
	*hm = hdc;
	return false;
}

// To solve linking errors
struct Monitor::Device::MonitorDevice Monitor::Device::monitorDevice = { 0 };
struct Monitor::AveragingInfo Monitor::averagingInfo = { 0 };
bool Monitor::Device::lockvar = false;

void Monitor::initialize()
{
	averagingInfo = { 0 };

	HDC hdc;
	Device::getDeviceContext(&hdc);
	averagingInfo.hDest = CreateCompatibleDC(hdc);

	averagingInfo.bmi.bmiHeader.biSize = sizeof(averagingInfo.bmi.bmiHeader);
	averagingInfo.bmi.bmiHeader.biWidth = getWidth();
	averagingInfo.bmi.bmiHeader.biHeight = getHeight();
	averagingInfo.bmi.bmiHeader.biPlanes = 1;
	averagingInfo.bmi.bmiHeader.biBitCount = 32;
	averagingInfo.bmi.bmiHeader.biCompression = BI_RGB;
	averagingInfo.bmi.bmiHeader.biSizeImage = getWidth() * 4 * getHeight();
	averagingInfo.bmi.bmiHeader.biClrUsed = 0;
	averagingInfo.bmi.bmiHeader.biClrImportant = 0;
}

void Monitor::setDeviceContext(HWND hwnd)
{
    LPMONITORINFO monitorInfo = {0};
    monitorInfo->cbSize = sizeof(MONITORINFO);
    HMONITOR hm = MonitorFromWindow(hwnd, MONITOR_DEFAULTTOPRIMARY);
	GetMonitorInfo(hm, monitorInfo);
	HDC hMonitor;
	EnumDisplayMonitors(GetDC(NULL), &monitorInfo->rcMonitor, enumMonitorProc, (LPARAM)&hMonitor);
	Monitor::Device::setMonitor(hMonitor, monitorInfo);
}

BYTE *Monitor::average()
{
	HDC hdc;
	Device::getDeviceContext(&hdc);

    BYTE *pixels = NULL;
    BYTE *retval = {0};
    unsigned long long average[4] = {0};

	UINT16 width = getWidth();
	UINT16 height = getHeight();

    HBITMAP hbDesktop = CreateDIBSection(averagingInfo.hDest, &averagingInfo.bmi, DIB_RGB_COLORS, (void **)(&pixels), NULL, NULL);
    SelectObject(averagingInfo.hDest, hbDesktop);
    BitBlt(averagingInfo.hDest, 0, 0, width, height, hdc, 0, 0, SRCCOPY);

    //clock_t begin = clock();
    for (int i = 0; i < (width * height * 4); i += 4)
    {
	//std::cout << (int)pixels[i + R] << "-" << (int)pixels[i + G] << "-" << (int)pixels[i + B] << std::endl;
	average[_R] += pixels[i + _R];
	average[_G] += pixels[i + _G];
	average[_B] += pixels[i + _B];
	//average[_A] += pixels[i + _A];
    }
    //clock_t end = clock();
    //double elapsed_secs = (double(end - begin) * 1000) / CLOCKS_PER_SEC;
    retval[_R] /= (width * height << 2);
    retval[_G] /= (width * height << 2);
    retval[_B] /= (width * height << 2);

	return retval;
}

UINT16 Monitor::getWidth()
{
	UINT16 width;
	Device::getWidth(&width);
	return width;
}

UINT16 Monitor::getHeight()
{
	UINT16 height;
	Device::getHeight(&height);
	return height;
}

void Monitor::Device::lock()
{
    while (lockvar);
	lockvar = true;
}

void Monitor::Device::unlock()
{
    if (lockvar)
		lockvar = false;
}

void Monitor::Device::setMonitor(HDC hMonitor, LPMONITORINFO monitorinfo)
{
	UINT16 width = (UINT16)(monitorinfo->rcMonitor.right - monitorinfo->rcMonitor.left);
	UINT16 height = (UINT16)(monitorinfo->rcMonitor.bottom - monitorinfo->rcMonitor.top);

	lock();
    Device::monitorDevice.targetScreen = hMonitor;
	Device::monitorDevice.width = width;
	Device::monitorDevice.height = height;
	unlock();
}

void Monitor::Device::getWidth(UINT16 *in)
{
    lock();
    *in = Device::monitorDevice.width;
    unlock();
}

void Monitor::Device::getHeight(UINT16 *in)
{
	lock();
	*in = Device::monitorDevice.height;
	unlock();
}

void Monitor::Device::getDeviceContext(HDC *in)
{
    lock();
    *in = Device::monitorDevice.targetScreen;
    unlock();
}
