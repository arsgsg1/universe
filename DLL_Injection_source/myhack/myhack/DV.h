#pragma once

////////////////////////////////////////////////////////////////////////////////
// 설정
////////////////////////////////////////////////////////////////////////////////

// 이름
#define KLOGGER_NAME			"KDF"
#define KLOGGER_NAME_UNICODE	L"KDF"

// Debug/Release 빌드 상관없이 디버그 로깅 활성화
#define KLOGGER_FORCE_ENABLE

// DebugView에 디버그 메시지 출력
#define KLOGGER_LOG_TO_DEBUG_VIEW

// 파일에 디버그 메시지 출력
// EXE: C:\경로\EXE파일.exe.log
// DLL: C:\경로\DLL파일.dll.log
//#define KLOGGER_LOG_TO_FILE

// 가능하면 단일 파일에 기록
// 로그파일.log
// 로그파일.연월일시분초밀리초.log
//#define KLOGGER_PREFER_SINGLE_LOG_FILE

// 로그 파일 난독화
//#define KLOGGER_OBFUSCATE_LOG_FILE

////////////////////////////////////////////////////////////////////////////////

#if defined(_DEBUG) || defined(KLOGGER_FORCE_ENABLE)
#	define DV(format, ...) g_kLogger.LogFormatWithFunctionName(format, __FUNCTION__, __VA_ARGS__)
#	define dv(format, ...) g_kLogger.LogFormat(format, __VA_ARGS__)
#	define DVU(format, ...) g_kLogger.LogFormatWithFunctionName(format, __FUNCTIONW__, __VA_ARGS__)
#	define dvu(format, ...) g_kLogger.LogFormat(format, __VA_ARGS__)
#	define DBGPrint(format, ...) g_kLogger.LogFormatWithFunctionName(format, __FUNCTION__, __VA_ARGS__)
#	define DBGPrintU(format, ...) g_kLogger.LogFormatWithFunctionName(format, __FUNCTIONW__, __VA_ARGS__)
#	define DV_TRACK_FUNCTION() CKLoggerFunctionTracker __kLogger_function_tracker(__FUNCTION__)
#	define DV_BEGIN_FUNCTION() DV_TRACK_FUNCTION()
#	define DV_END_FUNCTION()
#else
#	define DV(format, ...) (format, __VA_ARGS__)
#	define dv(format, ...) (format, __VA_ARGS__)
#	define DVU(format, ...) (format, __VA_ARGS__)
#	define dvu(format, ...) (format, __VA_ARGS__)
#	define DBGPrint(format, ...) g_kLogger.LogFormat(format, __VA_ARGS__)
#	define DBGPrintU(format, ...) g_kLogger.LogFormat(format, __VA_ARGS__)
#	define DV_TRACK_FUNCTION()
#	define DV_BEGIN_FUNCTION()
#	define DV_END_FUNCTION()
#endif

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <tchar.h>

#include <string>

class CKLogger
{
public:
	CKLogger();
	virtual ~CKLogger();

	void Log(const std::string& sMessage);
	void LogFormat(const char* pszFormat, ...);
	void LogFormatWithFunctionName(const char* pszFormat, ...);
	void Log(const std::wstring& sMessage);
	void LogFormat(const wchar_t* pszFormat, ...);
	void LogFormatWithFunctionName(const wchar_t* pszFormat, ...);
	void LogToDebugView(const std::string& sMessage);
	void LogToDebugViewFormat(const char* pszFormat, ...);
	void LogToDebugView(const std::wstring& sMessage);
	void LogToDebugViewFormat(const wchar_t* pszFormat, ...);
	void LogToFile(const std::string& sMessage);
	void LogToFileFormat(const char* pszFormat, ...);
	void LogToFile(const std::wstring& sMessage);
	void LogToFileFormat(const wchar_t* pszFormat, ...);
private:
	void Init();
	bool InitFileLog();
	void Cleanup();
	void CleanupFileLog();

	std::string FormatString(const char* pszFormat, ...);
	std::string FormatStringV(const char* pszFormat, va_list args);
	std::wstring FormatString(const wchar_t* pszFormat, ...);
	std::wstring FormatStringV(const wchar_t* pszFormat, va_list args);
	std::wstring MultiByteToUnicode(const std::string& sMultiByteString);

	void LockLogFile();
	void UnlockLogFile();
	void WriteFileLog(LPVOID pBuffer, DWORD dwBufferSize);

	HMODULE GetCurrentModuleHandle();
	std::wstring GetCurrentModulePath(HMODULE hModule);
	std::wstring GetCurrentTimeString();
	std::wstring GetCurrentTimeStringFile();
private:
	std::wstring m_sProcessID;
	std::string m_sHeader;
	std::wstring m_sUnicodeHeader;

	HANDLE m_hLogFile;
	CRITICAL_SECTION m_lckLogFile;
};

class CKLoggerFunctionTracker
{
public:
	CKLoggerFunctionTracker(const char* pszFunction);
	virtual ~CKLoggerFunctionTracker();
private:
	const char* m_pszFunction;
};

extern CKLogger g_kLogger;
