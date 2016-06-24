//#include "stdafx.h"
#include "FileOpr.h"
#pragma warning(disable:4996)
//CFileOpr::CFileOpr() : m_pFile(NULL)
//{
//	memset(m_szFilePath, 0, sizeof(m_szFilePath));
//}
//
//CFileOpr::~CFileOpr()
//{
//	if (m_pFile)
//	{
//		CloseFile();
//	}
//}
//
//////////////////////////////////////////////////////////////////////////////////////////
//BOOL CFileOpr::OpenFile(const char* pFilePath, enum ENUM_OPR_FILE_TYPE eMethod)
//{
//	int iType = 0;
//	DWORD dwError = 0;
//	if (eMethod & enum_opr_append)
//	{
//		m_pFile = fopen(pFilePath, "ba");
//	}
//	else if (eMethod == enum_opr_read)
//	{
//		m_pFile = fopen(pFilePath, "br");
//	}
//	else if (eMethod == enum_opr_write)
//	{
//		m_pFile = fopen(pFilePath, "wb");
//		dwError = ::GetLastError();
//	}
//	else
//	{
//		m_pFile = fopen(pFilePath, "brw");
//
//	}
//	if (NULL == m_pFile)
//	{
//		return FALSE;
//	}
//	strcpy(m_szFilePath, pFilePath);
//
//	return TRUE;
//}
//void CFileOpr::CloseFile()
//{
//	if (m_pFile)
//	{
//		fclose(m_pFile);
//		m_pFile = NULL;
//	}
//}
//long CFileOpr::ReadFile(char* pReadBuf, long lBufLen)
//{
//	if (m_pFile == NULL)
//	{
//		return 0;
//	}
//	return fread(pReadBuf, lBufLen, 1, m_pFile);
//}
//long CFileOpr::WriteFile(const char* pReadBuf, long lBufLen)
//{
//	if (m_pFile == NULL)
//	{
//		return 0;
//	}
//	return fwrite(pReadBuf, lBufLen, 1, m_pFile);
//}
//
//BOOL CFileOpr::SetPosition(DWORD dwPos)
//{
//	DWORD dwSize = GetFileSize();
//	if (dwSize < dwPos)
//	{
//		return FALSE;
//	}
//	return fseek(m_pFile, dwPos, SEEK_SET);
//}
//
//DWORD CFileOpr::GetFileSize()
//{
//	if (m_pFile == NULL)
//	{
//		return 0;
//	}
//	
//	//获取当前位置
//	DWORD dwPos = ftell(m_pFile);
//
//	//移动到文件末尾
//	fseek(m_pFile, 0, SEEK_END);
//
//	//获取文件大小
//	DWORD dwSize = ftell(m_pFile);
//
//	//移动回原来位置
//	fseek(m_pFile, dwPos, SEEK_SET);
//	dwSize = ftell(m_pFile);
//	return dwSize;
//}

/////////////////////////////////////////////////////////////////////////////////////
CFileOpr::CFileOpr()
{
	m_hFile = NULL;
	memset(m_szFilePath, 0, sizeof(m_szFilePath));
}
CFileOpr::~CFileOpr()
{
	CloseFile();
}

BOOL CFileOpr::OpenFile(const char* pFilePath, enum ENUM_OPR_FILE_TYPE eMethod)
{
	//打开文件
	char wsz[128]={0};     
	//MultiByteToWideChar(CP_ACP,0,pFilePath,-1,wsz,strlen(pFilePath));  


	if (eMethod == enum_opr_write)
	{
		m_hFile = ::CreateFile(wsz, GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
		if (m_hFile == INVALID_HANDLE_VALUE)
		{
			m_hFile = NULL;
			return FALSE;
		}
	}

	//保存文件路径
	strcpy(m_szFilePath, pFilePath);
	return TRUE;
}

void CFileOpr::CloseFile()
{
	if (m_hFile != NULL)
	{
		::CloseHandle(m_hFile);
	}
	memset(m_szFilePath, 0, sizeof(m_szFilePath));
}

long CFileOpr::ReadFile(char* pReadBuf, long lBufLen)
{
	if (NULL == m_hFile)
	{
		return 0;
	}

	return ::ReadFile(m_hFile, pReadBuf, lBufLen, NULL, NULL);
}
long CFileOpr::WriteFile(const char* pReadBuf, long lBufLen)
{
	DWORD lWriteLen = 0;
	if (NULL == m_hFile)
	{
		return 0;
	}
	::WriteFile(m_hFile, pReadBuf, lBufLen, (DWORD*)&lWriteLen, NULL);
	return lWriteLen;
}

BOOL CFileOpr::SetPosition(DWORD dwPos)
{
	return ::SetFilePointer(m_hFile, dwPos, NULL, FILE_BEGIN);
}

DWORD CFileOpr::GetFileSize()
{
	return ::GetFileSize(m_hFile, NULL);
}