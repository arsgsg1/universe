// dllmain.cpp: DLL 응용 프로그램의 진입점을 정의합니다.
#include "stdafx.h"
#include <Windows.h>
#include <tchar.h>
#include <urlmon.h>
#include <string.h>
#include "DV.h"

#pragma comment(lib, "urlmon.lib")

#define DEF_URL			(L"http://www.naver.com/index.html")
#define DEF_FILE_NAME	(L"index.html")
#define DEF_PROCESS_NAME "notepad.exe"
#define DEF_KEY			0x80000000

HMODULE g_hMod = NULL;
HHOOK g_hook = NULL;
DWORD g_mainThreadId = NULL;
WNDPROC OldEditProc = NULL;


LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	dv("KeyboardProc Working...");
	char szPath[MAX_PATH] = { 0, };
	char *p = NULL;

	if (nCode >= 0)
	{
		dv("KeyboardProc Working...");
		//bit 31:0 = key press, 1 = key release
		if (!(lParam & DEF_KEY))
		{
			dv("KeyboardProc Working...");
			GetModuleFileNameA(NULL, szPath, MAX_PATH);
			p = strrchr(szPath, '\\');

			//현재 프로세스 이름을 비교해서 만약 notepad.exe라면,
			//메세지는 응용 프로그램(혹은 다음 훅)으로 전달되지 않는다.
			if (!_stricmp(p + 1, DEF_PROCESS_NAME)) {
				dv("Key Press!!\n");
				dv("KeyboardProc Working...");
			}
		}
	}

	return CallNextHookEx(g_hook, nCode, wParam, lParam);
}

/*DWORD WINAPI ThreadProc(LPVOID lParam)
{

	//프로세스 ID에서 메인 스레드 ID 구하기
	HWND hWnd = FindWindow(L"Notepad", NULL);
	DWORD hPid = NULL;
	DWORD mainTid = GetWindowThreadProcessId(hWnd, &hPid);
	
	/*파라미터 타입 :
	1. 어떤 종류의 훅을 시도할건지(마우스, 키보드 등등)
	2. 후킹 프로시저
	3. 해당 DLL의 인스턴스 핸들
	4. 전역 훅인지 (0을 대입하면 전역훅) 지역 훅인지 (해당 스레드의 ID로 지역 후킹)
	g_hook = SetWindowsHookEx(WH_GETMESSAGE, KeyboardProc, g_hMod, mainTid);
	if (g_hook)
	{
		dv("SetWindowHookEx Success!! main Thread id(%d)", mainTid);
	}
	else
	{
		dv("SetWindowHookEx() error!! (%d)", GetLastError());
	}

	return 0;
}
*/
LRESULT CALLBACK EditSubProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	g_hook = SetWindowsHookEx(WH_KEYBOARD, KeyboardProc, g_hMod, g_mainThreadId);
	if (g_hook)
	{
		dv("SetWindowsHookEx() Success!! main ThreadID(%d)", g_mainThreadId);
	}
	else {
		dv("SetWindowsHookEx() failed....");
	}
	return CallWindowProc(OldEditProc, hWnd, iMessage, wParam, lParam);
}

void HookStop()
{
	if (g_hook)
	{
		UnhookWindowsHookEx(g_hook);
		g_hook = NULL;
	}
}

BOOL WINAPI DllMain( HINSTANCE hinstDLL,
                       DWORD  fdwReason,
                       LPVOID lpReserved
                     )	//hinstDLL: 상대방 프로세스의 핸들, fdwReason: 프로세스와 연결여부의 메시지, lpReserved: ?
{
	//2017-11-24    HANDLE hThread = NULL;	//해당 쓰레드를 받을자료형 선언
	g_hMod = (HMODULE)hinstDLL;	//쓰레드 제어권을 얻기 위해 캐스팅하여 받는다.
	
	HWND hWnd = FindWindow(L"Notepad", NULL);		//현재 가장 앞에 있는 윈도우의 핸들을 가져온다.
	DWORD hPid = NULL;
	g_mainThreadId = GetWindowThreadProcessId(hWnd, &hPid);
    switch (fdwReason)	//프로세스가 DLL을 호출하면 DLLMain 함수가 호출된다. 이 때 호출되면서 메시지가 파라미터에 담겨서 넘어오게 되는데, 각각의 분기마다 메시지를 나눠놓았다.
    {
    case DLL_PROCESS_ATTACH:	//DLL이 프로세스에 부착될때
		dv(L"myhack.dll Injection!!!");
		
		OldEditProc = (WNDPROC)SetWindowLong(hWnd, GWL_WNDPROC, (LONG)EditSubProc);
		// 윈도우 프로시저를 서브클래싱 하는 API
		//Param1 : 서브클래싱을 시도할 윈도우의 핸들값
		//Param2 : 서브클래싱으로 어떤것을 시도할 것인지
		//Param3 : Param2에 따라 어떤 변수, 함수 등등을 넣을 것인지
		
		/*
		hThread = CreateThread(NULL, 0, ThreadProc, NULL, 0, NULL);	//쓰레드를 핸들러에 담고, 쓰레드 메인 함수를 인자로 넣는다.
		if (g_hook)
		{
			UnhookWindowsHookEx(g_hook);
			g_hook = NULL;
		}
		CloseHandle(hThread);
		*///사용한 쓰레드는 닫아주어야만 한다. 운영체제의 소유이기 때문에 자원을 반납해야함
		break;
    case DLL_THREAD_ATTACH:		//DLL이 쓰레드에 부착될때
    case DLL_THREAD_DETACH:		//DLL이 쓰레드에서 떨어질때	(free)
    case DLL_PROCESS_DETACH:	//DLL이 프로세스에서 떨어질 때 (free)
        break;
    }
    return TRUE;
}

