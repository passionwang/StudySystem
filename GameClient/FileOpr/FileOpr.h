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
	virtual BOOL OpenFile(const char* pFilePath, enum ENUM_OPR_FILE_TYPE eMethod);
	virtual void CloseFile();
	virtual long ReadFile(char* pReadBuf, long lBufLen);
	virtual long WriteFile(const char* pReadBuf, long lBufLen);
	virtual BOOL SetPosition(DWORD dwPos);
	virtual DWORD GetFileSize();

private:

private:
	//Ϊ������ļ���IO��дЧ�ʣ��������ʹ���ļ�ӳ��
	//FILE* m_pFile;
	HANDLE m_hFile;
	char m_szFilePath[MAX_PATH];
};


#endif //__INCLUDE_FILE_OPR_H__