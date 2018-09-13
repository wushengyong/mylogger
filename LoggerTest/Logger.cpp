#include "stdafx.h"
#include "Logger.h"
#include <tchar.h>
#include <vector>

class CAutoLogLock
{
	LPCRITICAL_SECTION m_lpCriticalSection;
public:
	CAutoLogLock(LPCRITICAL_SECTION lpCriticalSection){
		m_lpCriticalSection = lpCriticalSection;
		if (NULL != m_lpCriticalSection){
			::EnterCriticalSection(m_lpCriticalSection);
		}
	}
	~CAutoLogLock()
	{
		if (NULL != m_lpCriticalSection){
			::LeaveCriticalSection(m_lpCriticalSection);
		}
	}
};

/// 第一个选项
static const TCHAR* g_DebugLevel_Str[] = {
	LOG_LEVEL_DEBUG_STR,
	LOG_LEVEL_INFO_STR,
	LOG_LEVEL_WARNING_STR,
	LOG_LEVEL_ERROR_STR,
	NULL
};

CLogger::CLogger()
{
#ifdef _DEBUG
	m_nLogLevel = LOG_LEVEL_DEBUG;
#else
	m_nLogLevel = 0;     /// 不输出日志
#endif
	m_nReloadSettings = DEFAULT_RELOAD_SETTINGS_TIME;
	m_LastLoadSettingTime = 0;

	m_strLogDir = LOG_DIR;
	m_strCurConfPath = GetConfFilePath();
	::InitializeCriticalSection(&m_Mutex);
}


CLogger::~CLogger()
{
	::DeleteCriticalSection(&m_Mutex);
}

/// 设置日志的名称
void CLogger::SetFileName(const tstring& strFileName)
{
	if (strFileName.empty()) return; // 不允许设置空的日志文件名
	m_strLogFileName = strFileName;
}

// 根据服务器的房间名称跟TableID生成文件名称
tstring CLogger::FileNameFromServer(const tstring& strServerName, WORD wTableID)
{
	TCHAR szStr[MAX_PATH] = { 0 };
	_sntprintf_s(szStr, MAX_PATH, TEXT("[Table%03d][%s]"), wTableID, strServerName.c_str());
	return szStr;
}

/// 获取正确的日志路径
tstring CLogger::GetCurrentLogFilePath() const
{
	time_t CurTime = 0;
	tm LocalTime = { 0 };
	TCHAR szFileName[MAX_PATH] = { 0 };

	time(&CurTime);
	localtime_s(&LocalTime, &CurTime);

	_sntprintf_s(szFileName, MAX_PATH, TEXT("%s\\[%04d-%02d-%02d]%s%s"),
		m_strLogDir.c_str(),
		LocalTime.tm_year + 1900, LocalTime.tm_mon + 1, LocalTime.tm_mday,
		m_strLogFileName.c_str(),
		LOG_SUFFIX);

	return szFileName;
}

void CLogger::LoadSettings()
{
	time_t CurTime = time(NULL);

	if (m_LastLoadSettingTime != 0 &&
		(m_nReloadSettings == 0 || CurTime - m_LastLoadSettingTime < m_nReloadSettings)){
		return; /// 时间间隔不到，不需要加载
	}

	TCHAR szLogSettings[MAX_PATH] = { 0 };
	/// 获取日志级别
	::GetPrivateProfileString(LOG_SETTING_APP_NAME, LOG_LEVEL_NAME, TEXT(""), szLogSettings, MAX_PATH, m_strCurConfPath.c_str());
	if (_tcslen(szLogSettings) != 0) {
		for (int nLevel = 0; g_DebugLevel_Str[nLevel] != NULL; nLevel++)
		{
			if (_tcsicmp(g_DebugLevel_Str[nLevel], szLogSettings) == 0) {
				m_nLogLevel = nLevel + 1;
				break;
			}
		}
	}
	/// 获取重新读取设置的时间
	int nSecs = (int)::GetPrivateProfileInt(LOG_SETTING_APP_NAME, LOG_RELOAD_TIME, -1, m_strCurConfPath.c_str());
	if (nSecs >= 0) {
		m_nReloadSettings = nSecs;
	}
	/// 获取输出的日志的目录
	::GetPrivateProfileString(LOG_SETTING_APP_NAME, LOG_DIR_NAME, LOG_DIR, szLogSettings, MAX_PATH, m_strCurConfPath.c_str());
	m_strLogDir = szLogSettings;

	m_LastLoadSettingTime = CurTime;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// 日志的辅助函数
#define IMPL_LOG_FUNC_SIMPLE(FuncName,Level) \
void CLogger::##FuncName##(const TCHAR* strFormat, ...){ \
	va_list args; \
	va_start(args, strFormat); \
	Log(NULL, -1, Level, strFormat, args); \
	va_end(args);\
}

#define IMPL_LOG_FUNC_DETAIL(FuncName, Level) \
void CLogger::##FuncName##(const TCHAR* strFuncName,int nLine, const TCHAR* strFormat, ...){ \
	va_list args; \
	va_start(args, strFormat); \
	Log(strFuncName, nLine, Level, strFormat, args); \
	va_end(args); \
}

IMPL_LOG_FUNC_SIMPLE(Debug, LOG_LEVEL_DEBUG)
IMPL_LOG_FUNC_SIMPLE(Info, LOG_LEVEL_INFO)
IMPL_LOG_FUNC_SIMPLE(Warning, LOG_LEVEL_WARNING)
IMPL_LOG_FUNC_SIMPLE(Error, LOG_LEVEL_ERROR)

IMPL_LOG_FUNC_DETAIL(DebugDetail, LOG_LEVEL_DEBUG)
IMPL_LOG_FUNC_DETAIL(InfoDetail, LOG_LEVEL_INFO)
IMPL_LOG_FUNC_DETAIL(WarningDetail, LOG_LEVEL_WARNING)
IMPL_LOG_FUNC_DETAIL(ErrorDetail, LOG_LEVEL_ERROR)


//////////////////////////////////////////////////////////////////////////////////////////////////////
/// 真正的日志输出函数
void CLogger::Log(const TCHAR* pFuncName, int nLine, int nLevel, const TCHAR* pFormat, va_list args)
{
	CAutoLogLock Lock(&m_Mutex);
	LoadSettings(); /// 加载配置
	if (m_nLogLevel == 0 || nLevel < m_nLogLevel) return; /// 级别不需要写日志

	if (m_strLogFileName.empty()) return; /// 没有设置文件名，不打印日志

	TCHAR szLogBuff[MAX_PATH] = { 0 };
	_vsntprintf(szLogBuff, MAX_PATH - 1, pFormat, args);

	/// 格式化输出信息

	time_t CurTime = 0;
	tm LocalTime = { 0 };

	time(&CurTime);
	localtime_s(&LocalTime, &CurTime);

	TCHAR szLogFmtBuff[MAX_PATH * 2] = { 0 };

	const TCHAR* pLevelStr = TEXT("Unknown");

	if (nLevel >= LOG_LEVEL_DEBUG && nLevel <= LOG_LEVEL_ERROR){
		pLevelStr = g_DebugLevel_Str[nLevel - LOG_LEVEL_DEBUG];
	}

	if (nLine >= 0) {
		_sntprintf_s(szLogFmtBuff, MAX_PATH * 2, TEXT("[%04d-%02d-%02d %02d:%02d:%02d][%-5s][%s][L%d] %s\n"),
			LocalTime.tm_year + 1900, LocalTime.tm_mon + 1, LocalTime.tm_mday,
			LocalTime.tm_hour, LocalTime.tm_min, LocalTime.tm_sec,
			pLevelStr, (NULL == pFuncName ? TEXT("") : pFuncName), nLine, szLogBuff);
	}
	else {
		_sntprintf_s(szLogFmtBuff, MAX_PATH * 2, TEXT("[%04d-%02d-%02d %02d:%02d:%02d][%-5s] %s\n"),
			LocalTime.tm_year + 1900, LocalTime.tm_mon + 1, LocalTime.tm_mday,
			LocalTime.tm_hour, LocalTime.tm_min, LocalTime.tm_sec, pLevelStr, szLogBuff);
	}

	/// 写入到文件中
	tstring strFilePath = GetCurrentLogFilePath();


	FILE* pLogFileHandle = ::_tfsopen(strFilePath.c_str(), TEXT("a,ccs=UTF-8"), _SH_SECURE);

	if (pLogFileHandle != NULL)
	{
		::_fputts(szLogFmtBuff, pLogFileHandle);
		::fclose(pLogFileHandle);
	}
}

/// 判断配置文件是相对路径还是决定路径
tstring CLogger::GetConfFilePath()
{
	TCHAR szFilePath[MAX_PATH] = { 0 };
	::GetModuleFileName(NULL, szFilePath, MAX_PATH);
	TCHAR* pSubPath = NULL;
	if ((pSubPath = _tcsrchr(szFilePath, TEXT('\\'))) != NULL)
	{
		++pSubPath;
		_sntprintf(pSubPath, szFilePath + MAX_PATH - pSubPath, TEXT("%s"), LOG_SETTING_CONF_FILE);
	}
	return szFilePath;
}
