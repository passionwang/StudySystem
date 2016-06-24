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
	    // �õ�exeִ��·��.  
    TCHAR tcExePath[MAX_PATH] = {0};  
    ::GetModuleFileName(NULL, tcExePath, MAX_PATH);  
    // ����ini·����exeͬһĿ¼��  
#ifndef MY_INI_FILE  
#define MY_INI_FILE     (TEXT("GameIniFile.ini"))  
#endif  
    //_tcsrchr() ��������������һ��'\\'��λ�ã������ظ�λ�õ�ָ��  
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
    //                              LPCTSTR lpAppName,      //�ڵ����֣���һ����0�������ַ���  
    //                              LPCTSTR lpKeyName,      //�������֣���һ����0�������ַ�������ΪNULL����ɾ��������  
    //                              LPCTSTR lpString,       //����ֵ����һ����0�������ַ�������ΪNULL����ɾ����Ӧ�ļ�  
    //                              LPCTSTR lpFileName      //Ҫд����ļ����ļ���������ini�ļ��������ͬһ��Ŀ¼�£�  
    //                              )                          Ҳ��ʹ�����·��,������Ҫ��������·����  
    //���Ini�����ڣ������Զ���szIniPath�ϴ�����INI�ļ�.��ִ��д��.                             
    ::WritePrivateProfileString(lpAppName, 
								lpKeyName, 
								lpString,
								szIniPath);  
    //::WritePrivateProfileString(TEXT("Service"), 
				//				TEXT("Name"), 
				//				TEXT("AutoRun  Helper"),
				//				szIniPath);  
    //���˵����ͬ��֮�п��Դ�����ȫ��ͬ�ļ�.  
	return TRUE;
}
BOOL CMyini::ReadIniFile(LPCTSTR lpAppName,LPCTSTR lpKeyName,TCHAR szKeyValue[])
{
	//����ִ�ж�ȡ ----------------------------------  
    if (!::PathFileExists(szIniPath))  
    {  
        return FALSE;  
    }  

    int nValue = 0;  
  
    //--------------------------------------------------------  
    //DWORD GetPrivateProfileString(  
    //                              LPCTSTR lpAppName,            // ����  
    //                              LPCTSTR lpKeyName,            // ��������ȡ�ü���ֵ  
    //                              LPCTSTR lpDefault,            // ��ָ���ļ������ڣ���ֵ��Ϊ��ȡ��Ĭ��ֵ  
    //                              LPTSTR lpReturnedString,      // һ��ָ�򻺳�����ָ�룬���ն�ȡ���ַ���  
    //                              DWORD nSize,                  // ָ��lpReturnedStringָ��Ļ������Ĵ�С  
    //                              LPCTSTR lpFileName            // ��ȡ��Ϣ���ļ���������ini�ļ��������ͬһ��Ŀ¼�£�  
    //                                                                Ҳ��ʹ�����·��,������Ҫ��������·��  
    //UINT GetPrivateProfileInt(  
    //                              LPCTSTR lpAppName,            // ����  
    //                              LPCTSTR lpKeyName,            // ��������ȡ�ü���ֵ  
    //                              INT nDefault,                 // ��ָ���ļ��������ڣ���ֵ��Ϊ��ȡ��Ĭ��ֵ  
    //                              LPCTSTR lpFileName            // ͬ��  
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
	//����ִ�ж�ȡ ----------------------------------  
    if (!::PathFileExists(szIniPath))  
    {  
        return FALSE;  
    }  
  
    //--------------------------------------------------------  
    //DWORD GetPrivateProfileString(  
    //                              LPCTSTR lpAppName,            // ����  
    //                              LPCTSTR lpKeyName,            // ��������ȡ�ü���ֵ  
    //                              LPCTSTR lpDefault,            // ��ָ���ļ������ڣ���ֵ��Ϊ��ȡ��Ĭ��ֵ  
    //                              LPTSTR lpReturnedString,      // һ��ָ�򻺳�����ָ�룬���ն�ȡ���ַ���  
    //                              DWORD nSize,                  // ָ��lpReturnedStringָ��Ļ������Ĵ�С  
    //                              LPCTSTR lpFileName            // ��ȡ��Ϣ���ļ���������ini�ļ��������ͬһ��Ŀ¼�£�  
    //                                                                Ҳ��ʹ�����·��,������Ҫ��������·��  
    //UINT GetPrivateProfileInt(  
    //                              LPCTSTR lpAppName,            // ����  
    //                              LPCTSTR lpKeyName,            // ��������ȡ�ü���ֵ  
    //                              INT nDefault,                 // ��ָ���ļ��������ڣ���ֵ��Ϊ��ȡ��Ĭ��ֵ  
    //                              LPCTSTR lpFileName            // ͬ��  
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
	//����ִ�ж�ȡ ----------------------------------  
    if (!::PathFileExists(szIniPath))  
    {  
        return FALSE;  
    }   
  
    //--------------------------------------------------------  
    //DWORD GetPrivateProfileString(  
    //                              LPCTSTR lpAppName,            // ����  
    //                              LPCTSTR lpKeyName,            // ��������ȡ�ü���ֵ  
    //                              LPCTSTR lpDefault,            // ��ָ���ļ������ڣ���ֵ��Ϊ��ȡ��Ĭ��ֵ  
    //                              LPTSTR lpReturnedString,      // һ��ָ�򻺳�����ָ�룬���ն�ȡ���ַ���  
    //                              DWORD nSize,                  // ָ��lpReturnedStringָ��Ļ������Ĵ�С  
    //                              LPCTSTR lpFileName            // ��ȡ��Ϣ���ļ���������ini�ļ��������ͬһ��Ŀ¼�£�  
    //                                                                Ҳ��ʹ�����·��,������Ҫ��������·��  
    //UINT GetPrivateProfileInt(  
    //                              LPCTSTR lpAppName,            // ����  
    //                              LPCTSTR lpKeyName,            // ��������ȡ�ü���ֵ  
    //                              INT nDefault,                 // ��ָ���ļ��������ڣ���ֵ��Ϊ��ȡ��Ĭ��ֵ  
    //                              LPCTSTR lpFileName            // ͬ��  
    //  
    //--------------------------------------------------------  
   
    nValue = ::GetPrivateProfileInt(lpAppName, 
									lpKeyName, 
									0, 
									szIniPath);  
	return TRUE;
}
