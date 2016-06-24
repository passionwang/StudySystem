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

//数据解析宏和身份验证宏
#define DEF_UNSERIALIZE_VERIFY(TYPE) \
	/*解析数据：*/ \
	STRU_##TYPE##_RQ oDataRq; \
	if (0 == oDataRq.UnSerialize((char*)pData, lLen)) \
	{ \
		return FALSE; \
	} \
	/*根据用户名称获取用户信息*/ \
	USER_MAP::iterator it = m_oUserMap.find(oDataRq.m_i64UserID); \
	if (it == m_oUserMap.end()) \
	{ \
		return FALSE; \
	} \
	else if (it->second->m_i64VerifyCode != oDataRq.m_i64VerifyCode) \
	{ \
		return FALSE; \
	}

//应答所有请求的通用宏
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
//定义任务处理回调通知接口
class IKernelToTask
{
public:
	virtual BOOL DealData(STRU_SESSION* pSession, long lLen, const char* pData) = 0;
};

//继承线程池中任务接口，实现kernel处理对象需要缓存的任务结构
class CMyTask : public ITask
{
public:
	virtual bool RunTask()
	{
		//回调通知kernel处理任务
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

//文件传输任务
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
	//实现基类模板方法模式静态多台的接口
	//提供派生类去实现资源初始化和资源销毁的方法
	BOOL Init();
	void UnInit();

public:
	//实现接收网络数据的方法
	BOOL OnRecvData(STRU_SESSION* pSession, const char* pData, long lDataLen, int iType);

public:
	//实现为任务处理数据的方法
	BOOL DealData(STRU_SESSION* pSession, long lLen, const char* pData);

private:
	//定义处理函数指针类型，统一不同协议的调用方式
	typedef BOOL (CKernel::*DEAL_FUN)(STRU_SESSION* pSession, long lLen, const char* pData);

	//为每个请求定义对应的处理方法
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
	//定义用户信息结构
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

	//定义用户信息表
	typedef std::map<INT64, STRU_USER_INFO*> USER_MAP;

private:
	//从数据哭加载用户信息
	BOOL ReadUserInfo();

	//读取app列表
	BOOL ReadAppList(INT64 i64AppKey, long lCount, STRU_GET_APP_LIST_RS& oDataRs);

	//使用用户ID+登陆时间生成验证码
	inline INT64 GetVerifyCode(INT64 i64UserID)
	{
		DWORD dwCurrentTime = ::GetTickCount();
		INT64 i64VerifyCode = (((INT64)dwCurrentTime) << 32) + ((i64UserID % dwCurrentTime) << 32 >> 32);

		return i64VerifyCode;
	}
private:
	//保存Tcp会话
	BOOL SaveTcpSession(STRU_SESSION* pSession, long lLen, const char* pData);
	//生成文件key（数据库中文件的唯一标识）
	inline INT64 GetFileKey(INT64 i64UserID, WORD wVersion, WORD wFileNameLen, const char* pFileName)
	{
		INT64 i64Key = i64UserID * HIBYTE(wVersion) / LOBYTE(wVersion) + wFileNameLen;

		return i64Key;
	}

	//计算投递队列
	inline long GetPostQueueNum(INT64 i64Key)
	{
		return i64Key % m_lThreadCount;
	}

private:
	//文件接收线程
	static unsigned int WINAPI OnFileTrans(void* pParam);
	void OnFileTrans(long lIndex);

	//判断某路径下的文件是否存在，如果不存在创建
	BOOL IsExistFile(INT64 i64UserID, char* pFileName, WORD wNameLen)
	{
		//合成路径
		//char szPath[MAX_PATH] = { 0 };
		//char szDir[MAX_PATH] = { 0 };
		//sprintf_s(szPath, MAX_PATH, "%I64d/%s", i64UserID, pFileName);
		//sprintf_s(szDir, MAX_PATH, "%I64d/", i64UserID);

		//HANDLE hFile = ::CreateFile(szPath, FILE_READ_DATA, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		//if (hFile == INVALID_HANDLE_VALUE)
		//{
		//	//创建目录
		//	::CreateDirectory(szDir, NULL);

		//	//创建文件
		//	hFile = ::CreateFile(szPath, FILE_READ_DATA, FILE_SHARE_READ, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
		//	::CloseHandle(hFile);
		//}
		////如果文件存在并且已经打开，那么关闭打开的文件
		//else
		//{
		//	::CloseHandle(hFile);
		//}

		return TRUE;
	}

	//文件传输线程参数
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

	//定义文件对象表<filekey, CFileOpr*>
	typedef std::map<INT64, IFile*> FILE_OBJECT_MAP;
private:
	//等待游戏的玩家信息
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
	//已经匹配的游戏玩家信息
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
	//游戏路径记录
	//游戏信息结构体，路径-长度
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
	INet* m_pTcpNet;				//使用简单网络工厂来创建
	INet* m_pUdpNet;
	//已匹配用户链表
	int m_iIndex;
	std::map<int,AttackGamePlayerStruct*> m_mapPlayers;
	//用户名关联的TCP会话
	std::map<INT64, STRU_SESSION*> m_mapTcpSession;
	//等待游戏的玩家信息--维护表----游戏ID为索引
	std::map<int, GamePlayerStruct*> m_mapGamePlayer;
	//已匹配用户--维护表----游戏ID为索引
	std::map<int, std::map<int,AttackGamePlayerStruct*>*> m_mapAttackGamePlayer;
	
	CMyDao m_oDao;
	CThreadPool m_oPool;

	//定义函数指针数组
	DEAL_FUN m_pDealFun[DEF_PROTOCOL_COUNT];
	//用户表
	USER_MAP m_oUserMap;
	//为每个传文件线程准备一个任务队列
	CCircleQueue<CFileTransTask> *m_pFileTransQueue;
	//传输文件线程数量
	long m_lThreadCount;
	//线程参数
	STRU_FILE_TRANS_PARAM* m_pThreadParam;
	//线程管理标记
	BOOL m_bStop;

	//保存文件对象的表
	FILE_OBJECT_MAP m_oFileMap;
};
#endif //__INCLUDE_KERNEL_H__