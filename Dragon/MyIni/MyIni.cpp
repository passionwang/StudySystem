#include "stdafx.h"
#include "Myini.h"

CMyini::CMyini()
{

}
CMyini::~CMyini()
{

}
BOOL CMyini::GetIniFilePath()
{
	    // 得到exe执行路径.  
    TCHAR tcExePath[MAX_PATH] = {0};  
    ::GetModuleFileName(NULL, tcExePath, MAX_PATH);  
    // 设置ini路径到exe同一目录下  
#ifndef MY_INI_FILE  
#define MY_INI_FILE     (TEXT("GameIniFile.ini"))  
#endif  
    //_tcsrchr() 反向搜索获得最后一个'\\'的位置，并返回该位置的指针  
    TCHAR *pFind = _tcsrchr(tcExePath, '\\');  
    if (pFind == NULL)  
    {  
        return FALSE;  
    }  
    *pFind = '\0';  
      
    szIniPath = tcExePath;  
    szIniPath += "\\";  
    szIniPath += MY_INI_FILE;  
//	szIniPath.Format(L"D:/GameIniFile.ini");
	return TRUE;
}
BOOL CMyini::WriteIniFile(LPCTSTR lpAppName,LPCTSTR lpKeyName,LPCTSTR lpString)
{
	//--------------------------------------------------------  
    //BOOL WritePrivateProfileString(  
    //                              LPCTSTR lpAppName,      //节的名字，是一个以0结束的字符串  
    //                              LPCTSTR lpKeyName,      //键的名字，是一个以0结束的字符串。若为NULL，则删除整个节  
    //                              LPCTSTR lpString,       //键的值，是一个以0结束的字符串。若为NULL，则删除对应的键  
    //                              LPCTSTR lpFileName      //要写入的文件的文件名。若该ini文件与程序在同一个目录下，  
    //                              )                          也可使用相对路径,否则需要给出绝度路径。  
    //如果Ini不存在，它会自动在szIniPath上创建此INI文件.再执行写入.                             
    ::WritePrivateProfileString(lpAppName, 
								lpKeyName, 
								lpString,
								szIniPath);  
    //::WritePrivateProfileString(TEXT("Service"), 
				//				TEXT("Name"), 
				//				TEXT("AutoRun  Helper"),
				//				szIniPath);  
    //这个说明不同节之中可以存在完全相同的键.  
	return TRUE;
}
BOOL CMyini::ReadIniFile(LPCTSTR lpAppName,LPCTSTR lpKeyName,TCHAR szKeyValue[])
{
	//下面执行读取 ----------------------------------  
    if (!::PathFileExists(szIniPath))  
    {  
        return FALSE;  
    }  

    int nValue = 0;  
  
    //--------------------------------------------------------  
    //DWORD GetPrivateProfileString(  
    //                              LPCTSTR lpAppName,            // 节名  
    //                              LPCTSTR lpKeyName,            // 键名，读取该键的值  
    //                              LPCTSTR lpDefault,            // 若指定的键不存在，该值作为读取的默认值  
    //                              LPTSTR lpReturnedString,      // 一个指向缓冲区的指针，接收读取的字符串  
    //                              DWORD nSize,                  // 指定lpReturnedString指向的缓冲区的大小  
    //                              LPCTSTR lpFileName            // 读取信息的文件名。若该ini文件与程序在同一个目录下，  
    //                                                                也可使用相对路径,否则需要给出绝度路径  
    //UINT GetPrivateProfileInt(  
    //                              LPCTSTR lpAppName,            // 节名  
    //                              LPCTSTR lpKeyName,            // 键名，读取该键的值  
    //                              INT nDefault,                 // 若指定的键名不存在，该值作为读取的默认值  
    //                              LPCTSTR lpFileName            // 同上  
    //  
    //--------------------------------------------------------  
    ::GetPrivateProfileString(lpAppName, 
							  lpKeyName, 
							  NULL, 
							  szKeyValue, 
							  MAX_PATH, 
							  szIniPath);
    nValue = ::GetPrivateProfileInt(lpAppName, 
									lpKeyName, 
									0, 
									szIniPath);  
	return TRUE;
}
BOOL CMyini::GetIniFileString(LPCTSTR lpAppName,LPCTSTR lpKeyName,TCHAR szKeyValue[])
{
	//下面执行读取 ----------------------------------  
    if (!::PathFileExists(szIniPath))  
    {  
        return FALSE;  
    }  
  
    //--------------------------------------------------------  
    //DWORD GetPrivateProfileString(  
    //                              LPCTSTR lpAppName,            // 节名  
    //                              LPCTSTR lpKeyName,            // 键名，读取该键的值  
    //                              LPCTSTR lpDefault,            // 若指定的键不存在，该值作为读取的默认值  
    //                              LPTSTR lpReturnedString,      // 一个指向缓冲区的指针，接收读取的字符串  
    //                              DWORD nSize,                  // 指定lpReturnedString指向的缓冲区的大小  
    //                              LPCTSTR lpFileName            // 读取信息的文件名。若该ini文件与程序在同一个目录下，  
    //                                                                也可使用相对路径,否则需要给出绝度路径  
    //UINT GetPrivateProfileInt(  
    //                              LPCTSTR lpAppName,            // 节名  
    //                              LPCTSTR lpKeyName,            // 键名，读取该键的值  
    //                              INT nDefault,                 // 若指定的键名不存在，该值作为读取的默认值  
    //                              LPCTSTR lpFileName            // 同上  
    //  
    //--------------------------------------------------------  
    ::GetPrivateProfileString(lpAppName, 
							  lpKeyName, 
							  NULL, 
							  szKeyValue, 
							  MAX_PATH, 
							  szIniPath);
	return TRUE;
}
BOOL CMyini::GetIniFileInt(LPCTSTR lpAppName,LPCTSTR lpKeyName,int& nValue)
{
	//下面执行读取 ----------------------------------  
    if (!::PathFileExists(szIniPath))  
    {  
        return FALSE;  
    }   
  
    //--------------------------------------------------------  
    //DWORD GetPrivateProfileString(  
    //                              LPCTSTR lpAppName,            // 节名  
    //                              LPCTSTR lpKeyName,            // 键名，读取该键的值  
    //                              LPCTSTR lpDefault,            // 若指定的键不存在，该值作为读取的默认值  
    //                              LPTSTR lpReturnedString,      // 一个指向缓冲区的指针，接收读取的字符串  
    //                              DWORD nSize,                  // 指定lpReturnedString指向的缓冲区的大小  
    //                              LPCTSTR lpFileName            // 读取信息的文件名。若该ini文件与程序在同一个目录下，  
    //                                                                也可使用相对路径,否则需要给出绝度路径  
    //UINT GetPrivateProfileInt(  
    //                              LPCTSTR lpAppName,            // 节名  
    //                              LPCTSTR lpKeyName,            // 键名，读取该键的值  
    //                              INT nDefault,                 // 若指定的键名不存在，该值作为读取的默认值  
    //                              LPCTSTR lpFileName            // 同上  
    //  
    //--------------------------------------------------------  
   
    nValue = ::GetPrivateProfileInt(lpAppName, 
									lpKeyName, 
									0, 
									szIniPath);  
	return TRUE;
}
