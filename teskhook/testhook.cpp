// testhook.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "testhook.h"
#include "hookdll.h"
#pragma comment(lib, "hookdll.lib")

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	InstallLaunchEv();

	::Sleep(300000);

	UnInstallLaunchEv();
	return 0;
}