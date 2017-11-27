#include "StdAfx.h"
#include "DV.h"

#include <cstdio>
#include <cstdint>
#include <cstdarg>
#include <vector>

CKLogger g_kLogger;

CKLogger::CKLogger()
{
	Init();
}

CKLogger::~CKLogger()
{
	Cleanup();
}

void CKLogger::Log(const std::string& sMessage)
{
#ifdef KLOGGER_LOG_TO_DEBUG_VIEW
	LogToDebugView(sMessage);
#endif

#ifdef KLOGGER_LOG_TO_FILE
	LogToFile(sMessage);
#endif
}

void CKLogger::LogFormat(const char* pszFormat, ...)
{
	va_list args;

	va_start(args, pszFormat);
	Log(FormatStringV(pszFormat, args).c_str());
	va_end(args);
}

void CKLogger::LogFormatWithFunctionName(const char* pszFormat, ...)
{
	std::string sFormatBuffer = "%s ";
	sFormatBuffer.append(pszFormat);

	va_list args;

	va_start(args, pszFormat);
	Log(FormatStringV(sFormatBuffer.c_str(), args).c_str());
	va_end(args);
}

void CKLogger::Log(const std::wstring& sMessage)
{
#ifdef KLOGGER_LOG_TO_DEBUG_VIEW
	LogToDebugView(sMessage);
#endif

#ifdef KLOGGER_LOG_TO_FILE
	LogToFile(sMessage);
#endif
}

void CKLogger::LogFormat(const wchar_t* pszFormat, ...)
{
	va_list args;

	va_start(args, pszFormat);
	Log(FormatStringV(pszFormat, args).c_str());
	va_end(args);
}

void CKLogger::LogFormatWithFunctionName(const wchar_t* pszFormat, ...)
{
	std::wstring sFormatBuffer = L"%s ";
	sFormatBuffer.append(pszFormat);

	va_list args;

	va_start(args, pszFormat);
	Log(FormatStringV(sFormatBuffer.c_str(), args).c_str());
	va_end(args);
}

void CKLogger::LogToDebugView(const std::string& sMessage)
{
	std::string sOutMessage = m_sHeader;
	sOutMessage.append(sMessage);

	if (IsDebuggerPresent())
		sOutMessage.append("\r\n");

	OutputDebugStringA(sOutMessage.c_str());
}

void CKLogger::LogToDebugViewFormat(const char* pszFormat, ...)
{
	va_list args;

	va_start(args, pszFormat);
	LogToDebugView(FormatStringV(pszFormat, args).c_str());
	va_end(args);
}

void CKLogger::LogToDebugView(const std::wstring& sMessage)
{
	std::wstring sOutMessage = m_sUnicodeHeader;
	sOutMessage.append(sMessage);

	if (IsDebuggerPresent())
		sOutMessage.append(L"\r\n");

	OutputDebugStringW(sOutMessage.c_str());
}

void CKLogger::LogToDebugViewFormat(const wchar_t* pszFormat, ...)
{
	va_list args;

	va_start(args, pszFormat);
	LogToDebugView(FormatStringV(pszFormat, args).c_str());
	va_end(args);
}

void CKLogger::LogToFile(const std::string& sMessage)
{
	LogToFile(MultiByteToUnicode(sMessage).c_str());
}

void CKLogger::LogToFileFormat(const char* pszFormat, ...)
{
	va_list args;

	va_start(args, pszFormat);
	LogToFile(FormatStringV(pszFormat, args).c_str());
	va_end(args);
}

void CKLogger::LogToFile(const std::wstring& sMessage)
{
	if (m_hLogFile == INVALID_HANDLE_VALUE)
		return;

	LockLogFile();

	std::wstring sCurrentTimeString = GetCurrentTimeString();
	WriteFileLog((LPVOID)sCurrentTimeString.c_str(), (DWORD)(sCurrentTimeString.size() * sizeof(std::wstring::value_type)));

	WriteFileLog((LPVOID)m_sProcessID.c_str(), (DWORD)(m_sProcessID.size() * sizeof(std::wstring::value_type)));

	WriteFileLog((LPVOID)m_sUnicodeHeader.c_str(), (DWORD)(m_sUnicodeHeader.size() * sizeof(std::wstring::value_type)));

	WriteFileLog((LPVOID)sMessage.c_str(), (DWORD)(sMessage.size() * sizeof(std::wstring::value_type)));
	WriteFileLog(L"\r\n", 4);
	
	UnlockLogFile();
}

void CKLogger::LogToFileFormat(const wchar_t* pszFormat, ...)
{
	va_list args;

	va_start(args, pszFormat);
	LogToFile(FormatStringV(pszFormat, args).c_str());
	va_end(args);
}

void CKLogger::Init()
{
	m_sProcessID = FormatString(L"[%u] ", GetCurrentProcessId());
	m_sHeader = FormatString("[%s] ", KLOGGER_NAME);
	m_sUnicodeHeader = FormatString(L"[%s] ", KLOGGER_NAME_UNICODE);

#ifdef KLOGGER_LOG_TO_FILE
	if (!InitFileLog())
		m_hLogFile = INVALID_HANDLE_VALUE;
#endif
}

bool CKLogger::InitFileLog()
{
	std::wstring sCurrentFilePath = GetCurrentModulePath(GetCurrentModuleHandle());
	std::wstring sLogFilePath;
	
#ifdef KLOGGER_PREFER_SINGLE_LOG_FILE
	sLogFilePath = sCurrentFilePath;
	sLogFilePath.append(L".log");

	m_hLogFile = CreateFileW(sLogFilePath.c_str(), GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (m_hLogFile == INVALID_HANDLE_VALUE)
	{
#endif
		sLogFilePath = sCurrentFilePath;
		sLogFilePath.append(L".");
		sLogFilePath.append(GetCurrentTimeStringFile());
		sLogFilePath.append(L".log");

		m_hLogFile = CreateFileW(sLogFilePath.c_str(), GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		if (m_hLogFile == INVALID_HANDLE_VALUE)
			return false;
#ifdef KLOGGER_PREFER_SINGLE_LOG_FILE
	}
#endif

	if (GetLastError() != ERROR_FILE_EXISTS)
	{
		WriteFileLog("\xff\xfe", 2);
	}

	SetFilePointer(m_hLogFile, 0, NULL, FILE_END);

	InitializeCriticalSection(&m_lckLogFile);

	return true;
}

void CKLogger::Cleanup()
{
#ifdef KLOGGER_LOG_TO_FILE
	CleanupFileLog();
#endif
}

void CKLogger::CleanupFileLog()
{
	if (m_hLogFile != INVALID_HANDLE_VALUE)
	{
		CloseHandle(m_hLogFile);
		m_hLogFile = INVALID_HANDLE_VALUE;
	}

	DeleteCriticalSection(&m_lckLogFile);
}

std::string CKLogger::FormatString(const char* pszFormat, ...)
{
	va_list args;

	va_start(args, pszFormat);
	std::string sResult = FormatStringV(pszFormat, args);
	va_end(args);

	return sResult;
}

std::string CKLogger::FormatStringV(const char* pszFormat, va_list args)
{
	std::string sBuffer;

	int nBufferSize = _vscprintf(pszFormat, args);

	if (nBufferSize < 0)
		return sBuffer;

	sBuffer.resize(nBufferSize);
	vsprintf_s((char*)sBuffer.c_str(), sBuffer.size() + 1, pszFormat, args);

	return sBuffer;
}

std::wstring CKLogger::FormatString(const wchar_t* pszFormat, ...)
{
	va_list args;

	va_start(args, pszFormat);
	std::wstring sResult = FormatStringV(pszFormat, args);
	va_end(args);

	return sResult;
}

std::wstring CKLogger::FormatStringV(const wchar_t* pszFormat, va_list args)
{
	std::wstring sBuffer;

	int nBufferSize = _vscwprintf(pszFormat, args);

	if (nBufferSize < 0)
		return sBuffer;

	sBuffer.resize(nBufferSize);
	vswprintf_s((wchar_t*)sBuffer.c_str(), sBuffer.size() + 1, pszFormat, args);

	return sBuffer;
}

std::wstring CKLogger::MultiByteToUnicode(const std::string& sMultiByteString)
{
	std::wstring sUnicodeString;

	int nUnicodeLength = MultiByteToWideChar(CP_ACP, 0, sMultiByteString.c_str(), (int)sMultiByteString.size(), NULL, 0);
	if (nUnicodeLength == 0)
		return sUnicodeString;

	sUnicodeString.resize(nUnicodeLength, L'\0');

	if (MultiByteToWideChar(CP_ACP, 0, sMultiByteString.c_str(), (int)sMultiByteString.size(), (LPWSTR)sUnicodeString.c_str(), nUnicodeLength) == 0)
	{
		sUnicodeString.clear();
		return sUnicodeString;
	}

	return sUnicodeString;
}

void CKLogger::LockLogFile()
{
	EnterCriticalSection(&m_lckLogFile);
}

void CKLogger::UnlockLogFile()
{
	LeaveCriticalSection(&m_lckLogFile);
}

void CKLogger::WriteFileLog(LPVOID pBuffer, DWORD dwBufferSize)
{
#ifdef KLOGGER_OBFUSCATE_LOG_FILE
	DWORD nKeyIndex = SetFilePointer(m_hLogFile, 0, NULL, FILE_CURRENT);
	if (nKeyIndex == INVALID_SET_FILE_POINTER)
		return;

	nKeyIndex = nKeyIndex % sizeof(g_pbObfuscateKey);

	LPBYTE pbBuffer = (LPBYTE)pBuffer;
	std::vector<BYTE> vBuffer(pbBuffer, pbBuffer + dwBufferSize);
	
	for (size_t i = 0; i < vBuffer.size(); i++)
	{
		vBuffer[i] = vBuffer[i] ^ g_pbObfuscateKey[nKeyIndex];

		if (++nKeyIndex >= sizeof(g_pbObfuscateKey))
			nKeyIndex = 0;
	}

	pBuffer = vBuffer.data();
#endif

	DWORD dwWritten = 0;
	WriteFile(m_hLogFile, pBuffer, dwBufferSize, &dwWritten, NULL);
}

HMODULE CKLogger::GetCurrentModuleHandle()
{
	HANDLE hProcess = GetCurrentProcess();
	MEMORY_BASIC_INFORMATION mbi = { 0 };

	LPBYTE pTargetModule = (LPBYTE)&g_kLogger;

	for (LPBYTE pAddress = NULL; VirtualQueryEx(hProcess, pAddress, &mbi, sizeof(mbi)) != 0; pAddress += mbi.RegionSize)
	{
		if (mbi.State != MEM_COMMIT)
			continue;

		if (!(mbi.Type & MEM_IMAGE))
			continue;

		if (pTargetModule < mbi.BaseAddress || pTargetModule > ((LPBYTE)mbi.BaseAddress + mbi.RegionSize))
			continue;

		return (HMODULE)mbi.AllocationBase;
	}

	return NULL;
}

std::wstring CKLogger::GetCurrentModulePath(HMODULE hModule)
{
	wchar_t szPath[MAX_PATH + 1];
	szPath[GetModuleFileNameW(hModule, szPath, MAX_PATH)] = TEXT('\0');

	return szPath;
}

std::wstring CKLogger::GetCurrentTimeString()
{
	SYSTEMTIME currentTime;
	wchar_t szCurrentTime[64];
	GetLocalTime(&currentTime);

	swprintf_s(szCurrentTime, L"%04u:%02u:%02u %02u:%02u:%02u.%03u ", currentTime.wYear, currentTime.wMonth, currentTime.wDay, currentTime.wHour, currentTime.wMinute, currentTime.wSecond, currentTime.wMilliseconds);

	return szCurrentTime;
}

std::wstring CKLogger::GetCurrentTimeStringFile()
{
	SYSTEMTIME currentTime;
	wchar_t szCurrentTime[64];
	GetLocalTime(&currentTime);

	swprintf_s(szCurrentTime, L"%04u%02u%02u%02u%02u%02u%03u", currentTime.wYear, currentTime.wMonth, currentTime.wDay, currentTime.wHour, currentTime.wMinute, currentTime.wSecond, currentTime.wMilliseconds);

	return szCurrentTime;
}

CKLoggerFunctionTracker::CKLoggerFunctionTracker(const char* pszFunction)
	: m_pszFunction(pszFunction)
{
	g_kLogger.LogFormatWithFunctionName("Enter", m_pszFunction);
}

CKLoggerFunctionTracker::~CKLoggerFunctionTracker()
{
	g_kLogger.LogFormatWithFunctionName("Leave", m_pszFunction);
}
