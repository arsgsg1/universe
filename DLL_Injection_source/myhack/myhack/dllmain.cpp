// dllmain.cpp: DLL 응용 프로그램의 진입점을 정의합니다.
#include "stdafx.h"
#include <Windows.h>
#include <tchar.h>
#include "DV.h"

#define DEF_KEY			0x80000000
#define DEF_KEY_D		0x44
#define DEF_KEY_F		0x46
#define IE_HOOK_TARGET	L"Internet Explorer_Server"
#define DLL_PATH		L"C:\\vs_workspace\\universe\\DLL_Injection_source\\InjectDll\\Debug\\myhack.dll"

HHOOK			g_hook = NULL;
WNDPROC			OldWinProc = NULL;
HINSTANCE		TargetHinst = NULL;
DWORD			TargetThreadId = NULL;

LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (nCode >= 0)
	{
		if (!(lParam & DEF_KEY))
		{
			switch (wParam)
			{
			case DEF_KEY_D:
				dv("D키가 눌렸습니다.");
				break;
			case VK_F2:
				dv("F2키가 눌렸습니다.");
				break;
			default:
				break;
			}
		}
	}
	return CallNextHookEx(g_hook, nCode, wParam, lParam);
}

LRESULT CALLBACK EditWinProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	if (g_hook == NULL)
	{
		g_hook = SetWindowsHookEx(WH_KEYBOARD, KeyboardProc, TargetHinst, TargetThreadId);
		if (!g_hook)
		{
			dv(L"SetWindowsHookEx() error!! (%d)", GetLastError());
		}
		dv("SetWindowHookEX().... Create Hook Func");
	}else {

	}
	return CallWindowProcW(OldWinProc, hWnd, iMessage, wParam, lParam);
}
void HookStop()
{
	if (g_hook)
	{
		if (!UnhookWindowsHookEx(g_hook))	//if success, non-zero value return
		{
			dv("UnhookWindowsHookEx() error (%d)", GetLastError());
		}else {
			dv("Unhook Success~~");
		}
	}
	else {
		dv(L"g_hook is null!!");
		return;
	}
}
BOOL WINAPI DllMain( HINSTANCE hinstDLL,
                       DWORD  fdwReason,
                       LPVOID lpReserved
                     )	//hinstDLL: 상대방 프로세스의 핸들, fdwReason: 프로세스와 연결여부의 메시지, lpReserved: ?
{
	TargetHinst = hinstDLL;
	HWND hWnd = NULL;			//DLL이 부착된 프로세스의 핸들
	DWORD hPid = NULL;					//프로세스의 id를 얻기 위한 변수
    switch (fdwReason)	//프로세스가 DLL을 호출하면 DLLMain 함수가 호출된다. 이 때 호출되면서 메시지가 파라미터에 담겨서 넘어오게 되는데, 각각의 분기마다 메시지를 나눠놓았다.
    {
    case DLL_PROCESS_ATTACH:	//DLL이 프로세스에 부착될때
		dv("myhack.dll Injection!!!");
		//대상 윈도우의 핸들과 process id 찾기
		hWnd = FindWindow(L"Notepad", 0);
		if (NULL == hWnd)
		{
			dv(L"FindWindow() error!! (%d)", GetLastError());
		}
		TargetHinst = hinstDLL;
		TargetThreadId =  GetWindowThreadProcessId(hWnd, &hPid);
		if (NULL == TargetThreadId)
		{
			dv(L"GetWindowThreadProcessId() error (%d)", GetLastError());
		}else {
			OldWinProc = (WNDPROC)SetWindowLong(hWnd, GWL_WNDPROC, (LONG)EditWinProc);
			if (NULL == OldWinProc)
				dv("SetWindowLong() error!!! (%d)", GetLastError());
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

