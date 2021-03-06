// InjectDll.cpp: 콘솔 응용 프로그램의 진입점을 정의합니다.
//

#include "stdafx.h"
#include <Windows.h>
#include <tchar.h>
#include "DV.h"

BOOL InjectDll(DWORD dwPID, LPCTSTR szDllPath)
{
	HANDLE hProcess = NULL, hThread = NULL;
	HMODULE hMod = NULL;
	LPVOID pRemoteBuf = NULL;
	DWORD dwBufSize = (DWORD)(_tcslen(szDllPath) + 1) * sizeof(TCHAR);
	LPTHREAD_START_ROUTINE pThreadProc;

	//#1. dwPID를 이용하여 대상 프로세스 notepad.exe의 핸들을 구한다.
	if (!(hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPID)))
	{
		_tprintf(L"OpenProcess(%d) failed!!! [%d]\n", dwPID, GetLastError());
		dv("OpenProcess(%d) failed!!! [%d]\n", dwPID, GetLastError());
		return FALSE;
	}
	else {

	}
	//#2. 대상 프로세스 notepad.exe 메모리에 szDllPath 크기만의 메모리를 할당한다.
	pRemoteBuf = VirtualAllocEx(hProcess, NULL, dwBufSize, MEM_COMMIT, PAGE_READWRITE);

	//#3. 할당 받은 메모리에 myhack.dll 경로 ("c:\\myhack.dll")을 쓴다.
	WriteProcessMemory(hProcess, pRemoteBuf, (LPVOID)szDllPath, dwBufSize, NULL);

	//#4. LoadLibrary() API 주소를 구한다.
	hMod = GetModuleHandle(L"kernel32.dll");
	pThreadProc = (LPTHREAD_START_ROUTINE)GetProcAddress(hMod, "LoadLibraryW");
	
	//#5. notepad.exe 프로세스에 스레드를 실행
	hThread = CreateRemoteThread(hProcess, NULL, 0, pThreadProc, pRemoteBuf, 0, NULL);

	WaitForSingleObject(hThread, INFINITE);

	CloseHandle(hThread);
	CloseHandle(hProcess);

	return TRUE;
}

int _tmain()
{
	HWND hWnd = FindWindow(L"Notepad", NULL);
	DWORD hPid = NULL;	//해당 프로세스의 id를 넣을 변수
	GetWindowThreadProcessId(hWnd, &hPid);	//프로세스의 id를 얻고자할 윈도우의 핸들, 반환받을 PID의 포인터, 리턴값 : 메인 스레드의 ID
	//inject dll
	TCHAR DLLPath[] = L"C:\\Users\\DELL\\source\\repos\\InjectDll\\Debug\\myhack.dll";
	
	if (InjectDll(hPid, DLLPath))
		_tprintf(L"InjectDll(Process id : %d) (Path : %s)success!!!\n", hPid, DLLPath);
	else
		_tprintf(L"InjectDll(\"%s\" failed!!!\n", DLLPath);

    return 0;
}

