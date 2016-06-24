#pragma once
#ifndef __INCLUDE_KERNEL_H__
#define __INCLUDE_KERNEL_H__
#include "FactoryNet.h"
#include "INet.h"
#include <list>
#include <iostream>
#include <queue>
#include "Common.h"
#include "PackDef.h"
#include "TcpNet.h"
#include "Lock.h"
#include "map"
#include "MyIni.h"
#include "vector"
#include "Circle_Queue.h"
#include "FileOpr.h"
#include <atlstr.h>
using namespace std;
//kernel�ṩ��UI���������������ݵĽӿ�
//kernelʵ�֣�����UI���ã��̳г���۲��ߣ�ʵ�����ٵķ�������֪ͨʱ�������Լ�
class IKernelToUI:public IObserver 
{
public:
	virtual void UnInit() = 0;
	virtual bool DeleteGame(long lGameID) = 0;
	virtual bool DownLoadGame(long lGameID) = 0;
	virtual bool GetGameList() = 0;
	virtual bool PlayGameWithStranger(long lUserID, long lGameID) = 0;
	virtual bool UserLogin(INT64 i64UserID, WORD wPasswordLen, char* szPassword) = 0;
};
//UI�ṩ��kernel���ûص�֪ͨӦ�����Ľӿ�
class IUIToKernel
{
public:
	virtual bool NotifyLogin() = 0;
	virtual bool NotifyGetGameList(STRU_GET_APP_LIST_RS oDataAppListRs) = 0;
	virtual bool NotifyLoginRs(INT64 m_i64UserID,bool bRes) = 0;
	virtual bool NotifyRegisterRs(long lUserID,bool bRes) = 0;
	virtual bool NotifyGetGameListRs(bool bRes) = 0;
};
//�ļ�ͷ
struct STRUFILEHEAD
{
public:
	STRUFILEHEAD()
	{
		m_pFileHeadUpload = NULL;
		m_pFileHeadDownload = NULL;
		m_i64FileKey = 0;
	}
public:
	STRU_FILE_HEAD_UPLOAD* m_pFileHeadUpload;
	STRU_FILE_HEAD_DOWNLOAD* m_pFileHeadDownload;
	INT64 m_i64FileKey;
};
//�ļ���������
class CFileTransTask
{
public:
	CFileTransTask()
	{
		m_i64UserID = 0;
		m_i64FileKey = 0;
		m_dwFileLength = 0;
		m_wFileNameLen = 0;
		m_pFileHead = NULL;
	}

public:
	STRUFILEHEAD* m_pFileHead;
	INT64 m_i64UserID;
	INT64 m_i64FileKey;
	DWORD m_dwFileSize;
	WORD m_wFileNameLen;
	char m_szFileName[DEF_FILE_NAME_MAX_LEN];
	DWORD m_dwFileLength;
	char *m_pFileContent;
};

//IKernelToUI   UI ֪ͨ  Kernel  �ӿ�   UI  ��������
//IObserver     ʵ�ֱ��۲���  �е�  UnInit    ����
//IKernel       ����  ֪ͨ Kernel ������  RecvData  ����   
class CKernel:public IKernelToUI,public INetToKernel
{
	//˽�л����죬�������Ⱥ���������أ���������
public:
	CKernel();
	~CKernel();
public:
	//��kernel
	bool InitKernel(IUIToKernel* pUI);
	void UnInitKernel();
public:
	//�ṩ������ص����ݵĽӿ�
	void OnRecvData(const char* pData, long lDataLen,ENUM_RECV_TYPE eType);
public:
	//֪ͨ���۲��ߵĽӿ�
	virtual void UnInit();
	//UI��������ʹ��
	long SendData(char* pData,long lDataLen);
public:
	 //UI�����ӿ�	
////////////////////////////////////////////////////////////////////////////////////////////////////
	virtual bool DeleteGame(long lGameID);
	virtual bool DownLoadGame(long lGameID);
	virtual bool GetGameList();
	virtual bool PlayGameWithStranger(long lUserID,long lGameID);
	virtual bool UserLogin(INT64 i64UserID, WORD wPasswordLen, char* szPassword);
/////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//Э��  �±�  ��Ӧ��  ����ָ��  ����
////////////////////////////////////////////////////////////////////////////////////////////////////////////
public:
	//���崦����ָ�����ͣ�ͳһ��ͬЭ��ĵ��÷�ʽ
	typedef BOOL (CKernel::*DEAL_FUN)(const char* szRecvBuf,long lRecvLen);
	//���庯��ָ������
	DEAL_FUN m_pDealFun[DEF_PROTOCOL_COUNT];
	//���崦����
	BOOL OnHeartBeatRQ(const char* szRecvBuf,long lRecvLen);
	BOOL OnLoginRS(const char* szRecvBuf,long lRecvLen);
	BOOL OnRegisterRS(const char* szRecvBuf,long lRecvLen);
	BOOL OnGetGameListRS(const char* szRecvBuf,long lRecvLen);
	BOOL OnDownloadGameRS(const char* szRecvBuf,long lRecvLen);
		BOOL OnPlayStrangerRS(const char* szRecvBuf,long lRecvLen);
	BOOL OnGetFriendListRS(const char* szRecvBuf,long lRecvLen);
	BOOL OnInviteFriendRS(const char* szRecvBuf,long lRecvLen);
		BOOL OnBeInviteFriendRS(const char* szRecvBuf,long lRecvLen);
	BOOL OnAddFriendRS(const char* szRecvBuf,long lRecvLen);
	BOOL OnDeleteFriendRS(const char* szRecvBuf,long lRecvLen);
private:
	//֪ͨUI�ķ��ദ����
	bool NotifyToUI(void* pNotify);


	//д�������ļ�
	BOOL WriteMyIniFile(STRU_PLAY_STRANGER_RS oDataRs);
	//������Ϸ
	BOOL NoticeExeGame(STRU_PLAY_STRANGER_RS oDataRs);
private://�ϴ������ļ�����
	//�ļ������߳�
	static unsigned int WINAPI OnFileTrans(void* pParam);
	void OnFileTrans();

	//�ж�ĳ·���µ��ļ��Ƿ���ڣ���������ڴ���
	BOOL IsExistFile(INT64 i64UserID, char* pFileName, WORD wNameLen)
	{
		int num = 0;
		//�ϳ�·��
		char szPath[MAX_PATH] = { 0 };
		char szDir[MAX_PATH] = { 0 };

		sprintf_s(szPath, MAX_PATH, "%s/%s", "D:/GamePlat", pFileName);
		sprintf_s(szDir, MAX_PATH, "%s/", "D:/GamePlat");
		TCHAR str[MAX_PATH] = {0};
		MultiByteToWideChar(CP_ACP, 0, szPath, -1, str, strlen(szPath));
		HANDLE hFile = ::CreateFile(str, FILE_READ_DATA, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		num = GetLastError();
		if (hFile == INVALID_HANDLE_VALUE)
		{
			//����Ŀ¼
			TCHAR str1[MAX_PATH] = {0};
			MultiByteToWideChar(CP_ACP, 0, szDir, -1, str1, strlen(szDir));
			::CreateDirectory(str1, NULL);
			num = GetLastError();
			//�����ļ�
			//MultiByteToWideChar(CP_ACP, 0, szPath, -1, str, strlen(szPath));
			hFile = ::CreateFile(str, FILE_READ_DATA, FILE_SHARE_READ, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
			num = GetLastError();
			::CloseHandle(hFile);
		}
		//����ļ����ڲ����Ѿ��򿪣���ô�رմ򿪵��ļ�
		else
		{
			::CloseHandle(hFile);
		}

		return TRUE;
	}
	//�����ļ�key�����ݿ����ļ���Ψһ��ʶ��
	/*inline INT64 GetFileKey(INT64 i64UserID, WORD wVersion, WORD wFileNameLen, const char* pFileName)
	{
		INT64 i64Key = i64UserID * HIBYTE(wVersion) / LOBYTE(wVersion) + wFileNameLen;

		return i64Key;
	}*/
	//����Ͷ�ݶ���
	/*inline long GetPostQueueNum(INT64 i64Key)
	{
		return i64Key % m_lThreadCount;
	}*/
public:
	void NoticeKernelSendLogin(INT64 i64UserID,WORD wPasswordLen,char szPassword[]);
	void NoticeKernelSendPlayStranger(int nIndex);
	void NoticeKernelSendDownLoadGame(int nIndex);
private:
	//��Ϸ��Ϣ�ṹ�壬·��-����
	struct GamePathInfo
	{
	public:
		GamePathInfo()
		{
			memset(m_szAppPath,0,1024);
			m_wAppPathLen = 0;
		}
	public:
		char m_szAppPath[1024];
		WORD m_wAppPathLen;
	};
	
	//�ļ������̲߳���
	struct STRU_FILE_TRANS_PARAM
	{
		STRU_FILE_TRANS_PARAM()
		{
			m_pThis = NULL;
		}
	public:
		CKernel* m_pThis;
	};
////////////////////////////////////////////////////////////////////////////////////////////////////////////
private:
//�ϴ������ļ�������
	IFile* m_pFile;
	//Ϊÿ�����ļ��߳�׼��һ���������
	CCircleQueue<CFileTransTask> m_pFileTransQueue;
	//�����ļ��߳�����
	//long m_lThreadCount;
	//�̲߳���
	STRU_FILE_TRANS_PARAM* m_pThreadParam;
	STRUFILEHEAD m_oFileHead;
//��¼��Ϸ·��
	map<int,GamePathInfo*> m_mapGamePath;
//д�����ļ�
	CMyini* pMyIni; 
//���б��
	bool m_bRun;			
//�����߳̾��
	HANDLE m_hDealThread;
//UI�ص����ݽӿ�
	IUIToKernel* m_pUI;				
	INet* m_pTcp;
	INet* m_pUdp;
	CLock m_oLock;
public:
	INT64 m_i64UserID;
	INT64 m_i64VerifyCode;
	int m_iGameID;
};
#endif //__INCLUDE_KERNEL_H__