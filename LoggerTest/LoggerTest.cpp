// LoggerTest.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "Logger.h"

CLogger logger;

void TestLogger()
{
	DWORD dwThreadID = ::GetThreadId(::GetCurrentThread());
	for (int i = 0; i < 60; ++i) {
		logger.LOG_DEBUG(TEXT("��[%d]�Σ��߳�ID[%d]"), i, dwThreadID);
		logger.LOG_INFO(TEXT("��[%d]�Σ��߳�ID[%d]"), i, dwThreadID);
		logger.LOG_WARNING(TEXT("��[%d]�Σ��߳�ID[%d]"), i, dwThreadID);
		logger.LOG_ERROR(TEXT("��[%d]�Σ��߳�ID[%d]"), i, dwThreadID);
	}
}
int _tmain(int argc, _TCHAR* argv[])
{
	logger.SetFileName(SERVER_LOG_FILE_NAME(TEXT("������-ȫ��"), 0));
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

