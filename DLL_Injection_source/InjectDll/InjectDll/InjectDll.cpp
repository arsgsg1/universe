// InjectDll.cpp: 콘솔 응용 프로그램의 진입점을 정의합니다.
//

#include "stdafx.h"
#include <Windows.h>
#include <tchar.h>
#include "DV.h"

#define IE_HOOK_TARGET L"Internet Explorer_Server"
#define DLL_PATH L"C:\\vs_workspace\\universe\\DLL_Injection_source\\InjectDll\\Debug\\myhack.dll"

BOOL InjectDll(DWORD dwPID, LPCTSTR szDllPath)
{
	HANDLE hProcess = NULL, hThread = NULL;
	HMODULE hMod = NULL;
	LPVOID pRemoteBuf = NULL;
	DWORD dwBufSize = (DWORD)(_tcslen(szDllPath) + 1) * sizeof(TCHAR);
	LPTHREAD_START_ROUTINE pThreadProc;

	//#1. dwPID를 이용하여 대상 프로세스 핸들을 구한다.
	if (!(hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPID)))
	{
		_tprintf(L"OpenProcess(%d) failed!!! [%d]\n", dwPID, GetLastError());
		dv("OpenProcess(%d) failed!!! [%d]\n", dwPID, GetLastError());
		return FALSE;
	}
	else {

	}
	//#2. 대상 프로세스 메모리에 szDllPath 크기만의 메모리를 할당한다.
	pRemoteBuf = VirtualAllocEx(hProcess, NULL, dwBufSize, MEM_COMMIT, PAGE_READWRITE);
	if (NULL == pRemoteBuf)
	{
		dv("VirtualAllocEx() error!!! (%d)");
	}
	//#3. 할당 받은 메모리에 myhack.dll 경로 ("c:\\myhack.dll")을 쓴다.
	if (0 == WriteProcessMemory(hProcess, pRemoteBuf, (LPVOID)szDllPath, dwBufSize, NULL))
	{
		dv("WriteProcessMemory() error (%d)", GetLastError());
	}
	
	//#4. LoadLibrary() API 주소를 구한다.
	hMod = GetModuleHandle(L"kernel32.dll");
	pThreadProc = (LPTHREAD_START_ROUTINE)GetProcAddress(hMod, "LoadLibraryW");
	if (NULL == pThreadProc)
	{
		dv("GetProcAddress() error (%d)", GetLastError());
	}

	//#5. 프로세스에 스레드를 실행
	hThread = CreateRemoteThread(hProcess, NULL, 0, pThreadProc, pRemoteBuf, 0, NULL);
	if (NULL == hThread)
	{
		dv("CreateRemoteThread() error (%d)", GetLastError());
	}
	WaitForSingleObject(hThread, INFINITE);

	CloseHandle(hThread);
	CloseHandle(hProcess);

	return TRUE;
}

BOOL CALLBACK EnumChildProc(HWND hWnd, LPARAM lParam)
{
	TCHAR buf[_MAX_PATH];
	GetClassName(hWnd, (LPTSTR)&buf, _MAX_PATH);
	if (0 == wcscmp(IE_HOOK_TARGET, buf))
	{
		DWORD hPid = NULL;
		DWORD mainThreadId = GetWindowThreadProcessId(hWnd, &hPid);	//프로세스의 id를 얻고자할 윈도우의 핸들, 반환받을 PID의 포인터, 리턴값 : 메인 스레드의 ID
		if (0 == mainThreadId)
		{
			dv(L"GetWindowThreadProcessId() error (%d)", GetLastError());
		}

		if (InjectDll(hPid, DLL_PATH)) {
			_tprintf(L"InjectDll(Process id : %d) (mainThreadId: %d) (Path : %s)success!!!\n", hPid, mainThreadId, DLL_PATH);
			dv(L"InjectDll(Process id : %d) (mainThreadId: %d) (Path : %s)success!!!\n", hPid, mainThreadId, DLL_PATH);
		}else {
			_tprintf(L"InjectDll(\"%s\" failed!!!\n", DLL_PATH);
		}

		return FALSE;
	}
	return TRUE;
}

int _tmain()
{
	//HWND hWnd = FindWindow(L"explorer", NULL);
	HWND hWnd = FindWindow(L"IEFrame", NULL);
	DWORD hPid = NULL;	//해당 프로세스의 id를 넣을 변수
	GetWindowThreadProcessId(hWnd, &hPid);	//프로세스의 id를 얻고자할 윈도우의 핸들, 반환받을 PID의 포인터, 리턴값 : 메인 스레드의 ID
	EnumChildWindows(hWnd, EnumChildProc, 0);
    return 0;
}

