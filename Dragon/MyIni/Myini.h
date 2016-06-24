#pragma once

#ifndef __INCLUDE_MYINI_H__
#define __INCLUDE_MYINI_H__

#include <iostream>
#include <windows.h>
#include <tchar.h>

//如果是MFC 包含cstring.h   非MFC 包含atlstr.h
#include <String.h>
using namespace std;

class CMyini
{
public:
	CMyini();
	~CMyini();
public:
	BOOL GetIniFilePath();
	BOOL WriteIniFile(LPCTSTR lpAppName,LPCTSTR lpKeyName,LPCTSTR lpString);
	BOOL ReadIniFile(LPCTSTR lpAppName,LPCTSTR lpKeyName,TCHAR szKeyValue[]);
	BOOL GetIniFileString(LPCTSTR lpAppName,LPCTSTR lpKeyName,TCHAR szKeyValue[]);
	BOOL GetIniFileInt(LPCTSTR lpAppName,LPCTSTR lpKeyName,int& nValue);
private:
	CString szIniPath;				//ini文件的所在路径
};


#endif	//__INCLUDE_MY_INI_H__