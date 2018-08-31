// LoggerTest.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "Logger.h"

CLogger logger;

void TestLogger()
{
	DWORD dwThreadID = ::GetThreadId(::GetCurrentThread());
	for (int i = 0; i < 60; ++i) {
		logger.LOG_DEBUG(TEXT("第[%d]次，线程ID[%d]"), i, dwThreadID);
		logger.LOG_INFO(TEXT("第[%d]次，线程ID[%d]"), i, dwThreadID);
		logger.LOG_WARNING(TEXT("第[%d]次，线程ID[%d]"), i, dwThreadID);
		logger.LOG_ERROR(TEXT("第[%d]次，线程ID[%d]"), i, dwThreadID);
	}
}
int _tmain(int argc, _TCHAR* argv[])
{
	logger.SetFileName(SERVER_LOG_FILE_NAME(TEXT("斗地主-全民场"), 0));
	HANDLE hThreadHandle[4] = { 0 };
	for (int i = 0; i < 4; ++i)
	{
		hThreadHandle[i] = ::CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)TestLogger, NULL, 0, NULL);
	}

	for (int i = 0; i < 4; ++i)
	{
		::WaitForSingleObject(hThreadHandle[i], INFINITE);
		::CloseHandle(hThreadHandle[i]);
	}
	
	return 0;
}

