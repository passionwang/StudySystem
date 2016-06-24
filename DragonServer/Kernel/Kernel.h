#pragma once

#ifndef __INCLUDE_KERNEL_H__
#define __INCLUDE_KERNEL_H__
#include "FactoryNet.h"
#include "Control.h"
#include "MyProtocol.h"
#include "ThreadPool.h"
#include "FileOpr.h"
#include "MyDao.h"
#include "Circle_Queue.h"
#include "vector"

//���ݽ�����������֤��
#define DEF_UNSERIALIZE_VERIFY(TYPE) \
	/*�������ݣ�*/ \
	STRU_##TYPE##_RQ oDataRq; \
	if (0 == oDataRq.UnSerialize((char*)pData, lLen)) \
	{ \
		return FALSE; \
	} \
	/*�����û����ƻ�ȡ�û���Ϣ*/ \
	USER_MAP::iterator it = m_oUserMap.find(oDataRq.m_i64UserID); \
	if (it == m_oUserMap.end()) \
	{ \
		return FALSE; \
	} \
	else if (it->second->m_i64VerifyCode != oDataRq.m_i64VerifyCode) \
	{ \
		return FALSE; \
	}

//Ӧ�����������ͨ�ú�
#define DEF_RESPONSE() \
	char szBuf[DEF_MAX_CACHE]; \
	long lBufLen = 0; \
	lBufLen = oDataRs.Serialize(szBuf, DEF_MAX_CACHE); \
	m_pUdpNet->SendData(pSession, szBuf, lBufLen); 
#define SEND()\
	if(oDataRq.m_i64UserID == m_mapAttackGamePlayer[oDataRq.m_iGameID]->find(oDataRq.m_iIndex)->second->m_pGamePlayer1->m_i64UserID)\
		{\
			if(m_mapAttackGamePlayer[oDataRq.m_iGameID]->find(oDataRq.m_iIndex)->second->m_pGamePlayer2->m_pStruSessionGame)\
			{\
				while(0 > m_pUdpNet->SendData(m_mapAttackGamePlayer[oDataRq.m_iGameID]->find(oDataRq.m_iIndex)->second->m_pGamePlayer2->m_pStruSessionGame, szBuf, lBufLen)); \
			}\
		}\
		else\
		{\
			if(m_mapAttackGamePlayer[oDataRq.m_iGameID]->find(oDataRq.m_iIndex)->second->m_pGamePlayer1->m_pStruSessionGame)\
			{\
				while(0 > m_pUdpNet->SendData(m_mapAttackGamePlayer[oDataRq.m_iGameID]->find(oDataRq.m_iIndex)->second->m_pGamePlayer1->m_pStruSessionGame, szBuf, lBufLen)); \
			}\
		}
//����������ص�֪ͨ�ӿ�
class IKernelToTask
{
public:
	virtual BOOL DealData(STRU_SESSION* pSession, long lLen, const char* pData) = 0;
};

//�̳��̳߳�������ӿڣ�ʵ��kernel���������Ҫ���������ṹ
class CMyTask : public ITask
{
public:
	virtual bool RunTask()
	{
		//�ص�֪ͨkernel��������
		if (m_pKernel)
		{
			m_pKernel->DealData(m_pSession, m_lDataLen, m_szData);
		}
		return false;
	}
	CMyTask() : m_pSession(NULL), m_lDataLen(0), m_pKernel(NULL)
	{
	}
public:
	IKernelToTask* m_pKernel;
	STRU_SESSION* m_pSession;
	long m_lDataLen;
	char m_szData[DEF_MAX_CACHE];
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
		m_pSession = NULL;
	}

public:
	STRU_TCP_SESSION* m_pSession;
	INT64 m_i64UserID;
	INT64 m_i64FileKey;
	DWORD m_dwFileSize;
	WORD m_wFileNameLen;
	char m_szFileName[DEF_FILE_NAME_MAX_LEN];
	DWORD m_dwFileLength;
	char *m_pFileContent;
};

class CKernel : public CControl<CKernel>, public IKernel, public IKernelToTask
{
public:
	CKernel();
	~CKernel();

public:
	//ʵ�ֻ���ģ�巽��ģʽ��̬��̨�Ľӿ�
	//�ṩ������ȥʵ����Դ��ʼ������Դ���ٵķ���
	BOOL Init();
	void UnInit();

public:
	//ʵ�ֽ����������ݵķ���
	BOOL OnRecvData(STRU_SESSION* pSession, const char* pData, long lDataLen, int iType);

public:
	//ʵ��Ϊ���������ݵķ���
	BOOL DealData(STRU_SESSION* pSession, long lLen, const char* pData);

private:
	//���崦����ָ�����ͣ�ͳһ��ͬЭ��ĵ��÷�ʽ
	typedef BOOL (CKernel::*DEAL_FUN)(STRU_SESSION* pSession, long lLen, const char* pData);

	//Ϊÿ���������Ӧ�Ĵ�����
	BOOL OnDealLoginRq(STRU_SESSION* pSession, long lLen, const char* pData);
	//BOOL OnDealUploadRq(STRU_SESSION* pSession, long lLen, const char* pData);
	//BOOL OnDealDownloadRq(STRU_SESSION* pSession, long lLen, const char* pData);
	BOOL OnDealGetAppListRq(STRU_SESSION* pSession, long lLen, const char* pData);
	BOOL OnDealInstallRq(STRU_SESSION* pSession, long lLen, const char* pData);
	BOOL OnDealUnInstallRq(STRU_SESSION* pSession, long lLen, const char* pData);
	BOOL OnDealLogOutRq(STRU_SESSION* pSession, long lLen, const char* pData);
	
	BOOL OnReadyPlayRq(STRU_SESSION* pSession, long lLen, const char* pData);
	BOOL OnNoticeEndRq(STRU_SESSION* pSession, long lLen, const char* pData);
	BOOL OnSendGameDataRq(STRU_SESSION* pSession, long lLen, const char* pData);
	BOOL OnNoticeBrginRs(STRU_SESSION* pSession, long lLen, const char* pData);
	BOOL OnRecvGameDataRs(STRU_SESSION* pSession, long lLen, const char* pData);
	BOOL OnPlayStrangerRq(STRU_SESSION* pSession, long lLen, const char* pData);
	BOOL OnDownLoadGameRq(STRU_SESSION* pSession, long lLen, const char* pData);
	
private:
	//�����û���Ϣ�ṹ
	struct STRU_USER_INFO
	{
	public:
		INT64 m_i64VerifyCode;						
		INT64 m_i64UserID;
		INT64 m_i64LastTime;
		WORD m_wState;
		WORD m_wPasswordLen;
		char m_szPassword[DEF_MAX_PASSWORD_LEN];
	};

	//�����û���Ϣ��
	typedef std::map<INT64, STRU_USER_INFO*> USER_MAP;

private:
	//�����ݿ޼����û���Ϣ
	BOOL ReadUserInfo();

	//��ȡapp�б�
	BOOL ReadAppList(INT64 i64AppKey, long lCount, STRU_GET_APP_LIST_RS& oDataRs);

	//ʹ���û�ID+��½ʱ��������֤��
	inline INT64 GetVerifyCode(INT64 i64UserID)
	{
		DWORD dwCurrentTime = ::GetTickCount();
		INT64 i64VerifyCode = (((INT64)dwCurrentTime) << 32) + ((i64UserID % dwCurrentTime) << 32 >> 32);

		return i64VerifyCode;
	}
private:
	//����Tcp�Ự
	BOOL SaveTcpSession(STRU_SESSION* pSession, long lLen, const char* pData);
	//�����ļ�key�����ݿ����ļ���Ψһ��ʶ��
	inline INT64 GetFileKey(INT64 i64UserID, WORD wVersion, WORD wFileNameLen, const char* pFileName)
	{
		INT64 i64Key = i64UserID * HIBYTE(wVersion) / LOBYTE(wVersion) + wFileNameLen;

		return i64Key;
	}

	//����Ͷ�ݶ���
	inline long GetPostQueueNum(INT64 i64Key)
	{
		return i64Key % m_lThreadCount;
	}

private:
	//�ļ������߳�
	static unsigned int WINAPI OnFileTrans(void* pParam);
	void OnFileTrans(long lIndex);

	//�ж�ĳ·���µ��ļ��Ƿ���ڣ���������ڴ���
	BOOL IsExistFile(INT64 i64UserID, char* pFileName, WORD wNameLen)
	{
		//�ϳ�·��
		//char szPath[MAX_PATH] = { 0 };
		//char szDir[MAX_PATH] = { 0 };
		//sprintf_s(szPath, MAX_PATH, "%I64d/%s", i64UserID, pFileName);
		//sprintf_s(szDir, MAX_PATH, "%I64d/", i64UserID);

		//HANDLE hFile = ::CreateFile(szPath, FILE_READ_DATA, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		//if (hFile == INVALID_HANDLE_VALUE)
		//{
		//	//����Ŀ¼
		//	::CreateDirectory(szDir, NULL);

		//	//�����ļ�
		//	hFile = ::CreateFile(szPath, FILE_READ_DATA, FILE_SHARE_READ, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
		//	::CloseHandle(hFile);
		//}
		////����ļ����ڲ����Ѿ��򿪣���ô�رմ򿪵��ļ�
		//else
		//{
		//	::CloseHandle(hFile);
		//}

		return TRUE;
	}

	//�ļ������̲߳���
	struct STRU_FILE_TRANS_PARAM
	{
		STRU_FILE_TRANS_PARAM()
		{
			m_pThis = NULL;
			m_lIndex = 0;
		}
	public:
		CKernel* m_pThis;
		long m_lIndex;
	};

	//�����ļ������<filekey, CFileOpr*>
	typedef std::map<INT64, IFile*> FILE_OBJECT_MAP;
private:
	//�ȴ���Ϸ�������Ϣ
	struct GamePlayerStruct
	{
	public:
		GamePlayerStruct()
		{
			m_i64UserID = 0;
			m_pStruSession = NULL;
			m_pStruSessionGame = NULL;
			m_iGameID = -1;
		}
	public:
		INT64 m_i64UserID;
		STRU_SESSION* m_pStruSession;
		STRU_SESSION* m_pStruSessionGame;
		int m_iGameID;
	};
	//�Ѿ�ƥ�����Ϸ�����Ϣ
	struct AttackGamePlayerStruct
	{
	public:
		AttackGamePlayerStruct()
		{
			m_pGamePlayer1 = NULL;
			m_pGamePlayer2 = NULL;
			m_iGameID = 0;
		}
	public:
		GamePlayerStruct* m_pGamePlayer1;
		GamePlayerStruct* m_pGamePlayer2;
		int m_iGameID;
	};
	//��Ϸ·����¼
	//��Ϸ��Ϣ�ṹ�壬·��-����
	struct GamePathInfo
	{
	public:
		GamePathInfo()
		{
			memset(m_szPath,0,1024);
			m_wAppPathLen = 0;
			memset(m_szAppName,0,1024);
			m_wAppNameLen = 0;
		}
	public:
		char m_szPath[1024];
		WORD m_wAppPathLen;
		char m_szAppName[1024];
		WORD m_wAppNameLen;
	}; 
private:
	vector<GamePathInfo*> m_vecGamePath;
	//MatchInformation m_oMatch;
	INet* m_pTcpNet;				//ʹ�ü����繤��������
	INet* m_pUdpNet;
	//��ƥ���û�����
	int m_iIndex;
	std::map<int,AttackGamePlayerStruct*> m_mapPlayers;
	//�û���������TCP�Ự
	std::map<INT64, STRU_SESSION*> m_mapTcpSession;
	//�ȴ���Ϸ�������Ϣ--ά����----��ϷIDΪ����
	std::map<int, GamePlayerStruct*> m_mapGamePlayer;
	//��ƥ���û�--ά����----��ϷIDΪ����
	std::map<int, std::map<int,AttackGamePlayerStruct*>*> m_mapAttackGamePlayer;
	
	CMyDao m_oDao;
	CThreadPool m_oPool;

	//���庯��ָ������
	DEAL_FUN m_pDealFun[DEF_PROTOCOL_COUNT];
	//�û���
	USER_MAP m_oUserMap;
	//Ϊÿ�����ļ��߳�׼��һ���������
	CCircleQueue<CFileTransTask> *m_pFileTransQueue;
	//�����ļ��߳�����
	long m_lThreadCount;
	//�̲߳���
	STRU_FILE_TRANS_PARAM* m_pThreadParam;
	//�̹߳�����
	BOOL m_bStop;

	//�����ļ�����ı�
	FILE_OBJECT_MAP m_oFileMap;
};
#endif //__INCLUDE_KERNEL_H__