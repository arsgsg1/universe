// dllmain.cpp: DLL 응용 프로그램의 진입점을 정의합니다.
#include "stdafx.h"
#include <Windows.h>
#include <tchar.h>
#include "DV.h"

#define DEF_KEY			0x80000000
#define DEF_KEY_D		0x44
#define DEF_KEY_F		0x46
#define IE_HOOK_TARGET L"Internet Explorer_Server"
#define DLL_PATH L"C:\\vs_workspace\\universe\\DLL_Injection_source\\InjectDll\\Debug\\myhack.dll"


HMODULE g_hMod = NULL;
HHOOK g_hook = NULL;
DWORD g_mainThreadId = NULL;
WNDPROC OldEditProc = NULL;

LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (nCode >= 0)
	{
		//bit 31:0 = key press, 1 = key release
		
		if (!(lParam & DEF_KEY))
		{
			switch (wParam)
			{
			case VK_F2:
				dv("F2 키가 눌렸습니다!!");
				break;
			case DEF_KEY_D:
				dv("D 키가 눌렸습니다!!");
				break;
			default:
				break;
			}
		}
	}

	return CallNextHookEx(g_hook, nCode, wParam, lParam);
}

LRESULT CALLBACK EditSubProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{

	if (g_hook == NULL)
	{
		g_hook = SetWindowsHookEx(WH_KEYBOARD, KeyboardProc, g_hMod, g_mainThreadId);
		dv("SetWindowHookEx Status Code(%d)", GetLastError());
	}
	return CallWindowProc(OldEditProc, hWnd, iMessage, wParam, lParam);
	//서브 클래싱을 사용한 경우 반드시 원래의 윈도우 프로시저를 호출하여 메시지를 넘겨주어야 한다.
}

BOOL CALLBACK EnumChildProc(HWND hWnd, LPARAM lParam)
{
	TCHAR buf[_MAX_PATH];
	GetClassName(hWnd, (LPTSTR)&buf, _MAX_PATH);
	if (0 == wcscmp(IE_HOOK_TARGET, buf))
	{
		DWORD hPid = NULL;
		g_mainThreadId = GetWindowThreadProcessId(hWnd, &hPid);	//브라우저의 구조 중 후킹을 걸고자 하는 윈도우의 핸들과 PID를 전달한다.
		
		OldEditProc = (WNDPROC)SetWindowLong(hWnd, GWL_WNDPROC, (LONG)EditSubProc);	//후킹 달기
		if (OldEditProc == NULL)
		{
			dv("SetWindowLong() error... (%d)", GetLastError());
		}
		// 윈도우 프로시저를 서브클래싱 하는 API
		//Param1 : 서브클래싱을 시도할 윈도우의 핸들값
		//Param2 : 서브클래싱으로 어떤것을 시도할 것인지
		//Param3 : Param2에 따라 어떤 변수, 함수 등등을 넣을 것인지
		//반환값 : 원래의 윈도우 프로시저 함수의 주소, 나중에 원래 프로시저로 변경해주어야함!
		//현재 사용한 함수는 32bit전용, 32bit와 64bit 모두에서 호환되게 하려면 SetWindowLongPtr을 사용해야 한다.
	}
	return TRUE;
}

BOOL CALLBACK EnumWindowProc(HWND hWnd, LPARAM lParam)
{
	TCHAR buf[_MAX_PATH];
	if (IsWindowVisible(hWnd))
	{
		GetClassName(hWnd, (LPTSTR)&buf, _MAX_PATH);
		if (0 == wcscmp(L"IEFrame", buf))
		{
			EnumChildWindows(hWnd, EnumChildProc, 0);
		}
	}
	return TRUE;
}

void HookStop()
{
	if (g_hook)
	{
		UnhookWindowsHookEx(g_hook);
		dv("g_hook is destroyed...");
		g_hook = NULL;
	}
	else {
		dv("g_hook is null...");
	}
}

BOOL WINAPI DllMain( HINSTANCE hinstDLL,
                       DWORD  fdwReason,
                       LPVOID lpReserved
                     )	//hinstDLL: 상대방 프로세스의 핸들, fdwReason: 프로세스와 연결여부의 메시지, lpReserved: ?
{
	/*g_hMod = (HMODULE)hinstDLL;	//쓰레드 제어권을 얻기 위해 캐스팅하여 받는다.
	
	HWND hWnd = (HWND)hinstDLL;	//윈도우 대상 영역의 핸들 가져오기
	DWORD hPid = NULL;								//프로세스의 ID를 저장할 변수
	g_mainThreadId = GetWindowThreadProcessId(hWnd, &hPid);
	/*
		GetWindowThreadProcessId
		param1: 프로세스 ID와 메인스레드 정보를 가져올 윈도우 핸들
		param2: 프로세스 ID를 저장할 변수의 포인터
	*/
    switch (fdwReason)	//프로세스가 DLL을 호출하면 DLLMain 함수가 호출된다. 이 때 호출되면서 메시지가 파라미터에 담겨서 넘어오게 되는데, 각각의 분기마다 메시지를 나눠놓았다.
    {
    case DLL_PROCESS_ATTACH:	//DLL이 프로세스에 부착될때
		dv("myhack.dll Injection!!!");
		if (0 == EnumWindows(EnumWindowProc, 0))
		{
			dv("EnumWindows() error.... (%d)", GetLastError());
		}
		
		break;
    case DLL_THREAD_ATTACH:		//DLL이 쓰레드에 부착될때
    case DLL_THREAD_DETACH:		//DLL이 쓰레드에서 떨어질때	(free)
    case DLL_PROCESS_DETACH:	//DLL이 프로세스에서 떨어질 때 (free)
		HookStop();
        break;
    }
    return TRUE;
}

