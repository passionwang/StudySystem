#pragma once

#ifndef __INCLUDE_FILE_OPR_H__
#define __INCLUDE_FILE_OPR_H__
#include <stdio.h>
#include "IFile.h"

class CFileOpr : public IFile
{
public:
	CFileOpr();
	~CFileOpr();

public:
	BOOL OpenFile(const char* pFilePath, enum ENUM_OPR_FILE_TYPE eMethod);
	void CloseFile();
	long ReadFile(char* pReadBuf, long lBufLen);
	long WriteFile(const char* pReadBuf, long lBufLen);
	BOOL SetPosition(DWORD dwPos);
	DWORD GetFileSize();

private:

private:
	//Ϊ������ļ���IO��дЧ�ʣ��������ʹ���ļ�ӳ��
	//FILE* m_pFile;
	HANDLE m_hFile;
	char m_szFilePath[MAX_PATH];
};


#endif //__INCLUDE_FILE_OPR_H__