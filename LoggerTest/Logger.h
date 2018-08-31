#pragma once
// Add BY WUSHENGYONG
// Date : 20180831
// DESCRIPTION : 
//        20180831 First Add

#include <Windows.h>
#include <string>
#include <time.h>



///////////////////////////////////////////////////////////////////////////////////////////////////////////
/// ����
#define LOG_SETTING_CONF_FILE                       TEXT("D:\\360��ȫ���������\\LoggerTest\\LogConf.ini")
#define LOG_SETTING_APP_NAME                      TEXT("OxLand")
#define LOG_SUFFIX                                           TEXT(".log")
#define DEFAULT_RELOAD_SETTINGS_TIME         (60)
#define LOG_DIR                                                TEXT(".\\Log\\")
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// ������־�ļ�������
#define SERVER_LOG_FILE_NAME(szServerName,wTableID) (CLogger::FileNameFromServer(szServerName, wTableID))

#ifdef _UNICODE
typedef std::wstring tstring;
#define __FUNCTION_NAME__ __FUNCTIONW__
#else
typedef std::string tstring;
#define __FUNCTION_NAME__ __FUNCTION__
#endif

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// �����ļ��ļ�ֵ
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
	// ������־������
	void SetFileName(const tstring& strFileName);

	void Debug(const TCHAR* strFormat, ...);             // DEBUG��Ϣ���
	void Info(const TCHAR* strFormat, ...);              // ��ʾ��Ϣ���
	void Warning(const TCHAR* strFormat, ...);           // ������Ϣ���
	void Error(const TCHAR* strFormat, ...);             // ������Ϣ���

	void DebugDetail(const TCHAR* strFuncName,int nLine, const TCHAR* strFormat, ...);       
	void InfoDetail(const TCHAR* strFuncName,	int nLine, const TCHAR* strFormat, ...);       
	void WarningDetail(const TCHAR* strFuncName,int nLine,const TCHAR* strFormat, ...);      
	void ErrorDetail(const TCHAR* strFuncName,int nLine,const TCHAR* strFormat, ...);     

#define LOG_DEBUG(strFormat,...)   DebugDetail(__FUNCTION_NAME__, __LINE__, strFormat, __VA_ARGS__);
#define LOG_INFO(strFormat,...)    InfoDetail(__FUNCTION_NAME__, __LINE__, strFormat, __VA_ARGS__);
#define LOG_WARNING(strFormat,...) WarningDetail(__FUNCTION_NAME__, __LINE__, strFormat, __VA_ARGS__);
#define LOG_ERROR(strFormat,...)   ErrorDetail(__FUNCTION_NAME__, __LINE__, strFormat, __VA_ARGS__);

public:
	// ���ݷ������ķ������Ƹ�TableID�����ļ�����
	static tstring FileNameFromServer(const tstring& strServerName, WORD wTableID);

protected:
	tstring       GetCurrentLogFilePath() const;                                               /// ��ȡ��ǰӦ�������־���ļ�����
	void          LoadSettings();                                                                        /// ���ػ�������������Ϣ
	void          Log(const TCHAR* pFuncName, int nLine, int nLevel, 
		              const TCHAR* pFormat, va_list args);                       /// ��������־�������

protected:
	tstring       m_strLogDir;                      /// ��־Ŀ¼
	tstring       m_strLogFileName;            /// ��־�ļ����ļ���
	int             m_nLogLevel;                     /// �����־�ļ���
	int             m_nReloadSettings;           /// ���¼������õ�ʱ������0��ʾ������
	time_t        m_LastLoadSettingTime;    /// �ϴμ������õ�ʱ���

	CRITICAL_SECTION  m_Mutex;                 /// ������
};


