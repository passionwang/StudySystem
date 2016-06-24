#pragma once

#ifndef __INCLUDE_FILE_H__
#define __INCLUDE_FILE_H__
#include <Windows.h>

//���ļ��ķ�ʽ
enum ENUM_OPR_FILE_TYPE
{
	enum_opr_read = 1,
	enum_opr_write = 2,
	enum_opr_append = 4
};

//�����ļ��Ľӿ�
class IFile
{
public:
	virtual BOOL OpenFile(const char* pFilePath, enum ENUM_OPR_FILE_TYPE eMethod) = 0;
	virtual void CloseFile() = 0;
	virtual long ReadFile(char* pReadBuf, long lBufLen) = 0;
	virtual long WriteFile(const char* pReadBuf, long lBufLen) = 0;
	virtual BOOL SetPosition(DWORD dwPos) = 0;
	virtual DWORD GetFileSize() = 0;
};

#endif //__INCLUDE_FILE_H__