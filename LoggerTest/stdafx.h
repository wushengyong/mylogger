// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include "targetver.h"

#include <stdio.h>
#include <tchar.h>



#define _ATL_ATTRIBUTES
#define _AFX_ALL_WARNINGS
#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS

//////////////////////////////////////////////////////////////////////////////////

//MFC 文件
#include <AfxWin.h>
#include <AfxExt.h>
#include <AfxCmn.h>
#include <AfxDisp.h>
#include <Windows.h>
// TODO:  在此处引用程序需要的其他头文件
#define LOG_SETTING_APP_NAME TEXT("OxLand")
#include "Logger.h"