// LoggerTest.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"

CLogger logger;

int _tmain(int argc, _TCHAR* argv[])
{
	logger.SetFileName(SERVER_LOG_FILE_NAME(TEXT("������-ȫ��"), 0));
	
	std::wstring str;

	for (int i = 0; i < 10000; ++i)
	{
		str += TEXT("��");
	}
	logger.LOG_INFO(str.c_str());
	return 0;
}

