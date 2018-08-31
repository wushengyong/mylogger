#pragma once
// Add BY WUSHENGYONG
// Date : 20180831
// DESCRIPTION : 
//        20180831 First Add

#include <Windows.h>
#include <string>
#include <time.h>



///////////////////////////////////////////////////////////////////////////////////////////////////////////
/// 配置
#define LOG_SETTING_CONF_FILE                       TEXT("D:\\360安全浏览器下载\\LoggerTest\\LogConf.ini")
#define LOG_SETTING_APP_NAME                      TEXT("OxLand")
#define LOG_SUFFIX                                           TEXT(".log")
#define DEFAULT_RELOAD_SETTINGS_TIME         (60)
#define LOG_DIR                                                TEXT(".\\Log\\")
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// 生成日志文件的名称
#define SERVER_LOG_FILE_NAME(szServerName,wTableID) (CLogger::FileNameFromServer(szServerName, wTableID))

#ifdef _UNICODE
typedef std::wstring tstring;
#define __FUNCTION_NAME__ __FUNCTIONW__
#else
typedef std::string tstring;
#define __FUNCTION_NAME__ __FUNCTION__
#endif

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// 配置文件的键值
#define LOG_LEVEL_NAME                  TEXT("Level")
#define LOG_RELOAD_TIME                 TEXT("Reload")
#define LOG_DIR_NAME                      TEXT("LogDir")
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define LOG_LEVEL_DEBUG_STR          TEXT("DEBUG")
#define LOG_LEVEL_INFO_STR             TEXT("INFO")
#define LOG_LEVEL_WARNING_STR     TEXT("WARN")
#define LOG_LEVEL_ERROR_STR          TEXT("ERROR")

#define LOG_LEVEL_DEBUG                 1
#define LOG_LEVEL_INFO                    2
#define LOG_LEVEL_WARNING            3
#define LOG_LEVEL_ERROR                 4


class CLogger
{
public:
	CLogger();
	~CLogger();

public:
	// 设置日志的名称
	void SetFileName(const tstring& strFileName);

	void Debug(const TCHAR* strFormat, ...);             // DEBUG信息输出
	void Info(const TCHAR* strFormat, ...);              // 提示信息输出
	void Warning(const TCHAR* strFormat, ...);           // 警告信息输出
	void Error(const TCHAR* strFormat, ...);             // 错误信息输出

	void DebugDetail(const TCHAR* strFuncName,int nLine, const TCHAR* strFormat, ...);       
	void InfoDetail(const TCHAR* strFuncName,	int nLine, const TCHAR* strFormat, ...);       
	void WarningDetail(const TCHAR* strFuncName,int nLine,const TCHAR* strFormat, ...);      
	void ErrorDetail(const TCHAR* strFuncName,int nLine,const TCHAR* strFormat, ...);     

#define LOG_DEBUG(strFormat,...)   DebugDetail(__FUNCTION_NAME__, __LINE__, strFormat, __VA_ARGS__);
#define LOG_INFO(strFormat,...)    InfoDetail(__FUNCTION_NAME__, __LINE__, strFormat, __VA_ARGS__);
#define LOG_WARNING(strFormat,...) WarningDetail(__FUNCTION_NAME__, __LINE__, strFormat, __VA_ARGS__);
#define LOG_ERROR(strFormat,...)   ErrorDetail(__FUNCTION_NAME__, __LINE__, strFormat, __VA_ARGS__);

public:
	// 根据服务器的房间名称跟TableID生成文件名称
	static tstring FileNameFromServer(const tstring& strServerName, WORD wTableID);

protected:
	tstring       GetCurrentLogFilePath() const;                                               /// 获取当前应该输出日志的文件名称
	void          LoadSettings();                                                                        /// 加载环境变量配置信息
	void          Log(const TCHAR* pFuncName, int nLine, int nLevel, 
		              const TCHAR* pFormat, va_list args);                       /// 真正的日志输出函数

protected:
	tstring       m_strLogDir;                      /// 日志目录
	tstring       m_strLogFileName;            /// 日志文件的文件名
	int             m_nLogLevel;                     /// 输出日志的级别
	int             m_nReloadSettings;           /// 重新加载配置的时间间隔，0表示不加载
	time_t        m_LastLoadSettingTime;    /// 上次加载配置的时间点

	CRITICAL_SECTION  m_Mutex;                 /// 互斥体
};


