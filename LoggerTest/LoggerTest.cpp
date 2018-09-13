// LoggerTest.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

CLogger logger;

int _tmain(int argc, _TCHAR* argv[])
{
	logger.SetFileName(SERVER_LOG_FILE_NAME(TEXT("斗地主-全民场"), 0));
	
	std::wstring str;

	for (int i = 0; i < 10000; ++i)
	{
		str += TEXT("你");
	}
	logger.LOG_INFO(str.c_str());
	return 0;
}

