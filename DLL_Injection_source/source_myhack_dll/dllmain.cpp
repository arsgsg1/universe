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

HMODULE g_hMod = NULL;

DWORD WINAPI ThreadProc(LPVOID lParam)
{
	TCHAR szPath[_MAX_PATH + 10] = { 0, };

	if (!GetModuleFileName(g_hMod, szPath, MAX_PATH))
		return FALSE;

	TCHAR *p = _tcsrchr(szPath, '\\');

	if (!p)
		return FALSE;

	_tcscpy_s(p + 1, _MAX_PATH, DEF_FILE_NAME);
	
	URLDownloadToFile(NULL, DEF_URL, szPath, 0, NULL);
	memset(szPath, 0, sizeof(szPath));
	return 0;
}

BOOL WINAPI DllMain( HINSTANCE hinstDLL,
                       DWORD  fdwReason,
                       LPVOID lpReserved
                     )
{
	HANDLE hThread = NULL;	//해당 쓰레드를 받을자료형 선언
	g_hMod = (HMODULE)hinstDLL;	//쓰레드 제어권을 얻기 위해 캐스팅하여 받는다. HMODULE == HINSTANCE


    switch (fdwReason)	//프로세스가 DLL을 호출하면 DLLMain 함수가 호출된다. 이 때 호출되면서 메시지가 파라미터에 담겨서 넘어오게 되는데, 각각의 분기마다 메시지를 나눠놓았다.
    {
    case DLL_PROCESS_ATTACH:	//DLL이 프로세스에 부착될때
		dv(L"myhack.dll Injection!!!");
		hThread = CreateThread(NULL, 0, ThreadProc, NULL, 0, NULL);	//쓰레드를 핸들러에 담고, 쓰레드 메인 함수를 인자로 넣는다.
		CloseHandle(hThread);										//사용한 쓰레드는 닫아주어야만 한다. 운영체제의 소유이기 때문에 자원을 반납해야함
		break;
    case DLL_THREAD_ATTACH:		//DLL이 쓰레드에 부착될때
    case DLL_THREAD_DETACH:		//DLL이 쓰레드에서 떨어질때	(free)
    case DLL_PROCESS_DETACH:	//DLL이 프로세스에서 떨어질 때 (free)
        break;
    }
    return TRUE;
}

