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
//kernel提供给UI用来发送请求数据的接口
//kernel实现，留给UI调用，继承抽象观察者，实现销毁的方法，当通知时候，销毁自己
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
//UI提供给kernel调用回调通知应答结果的接口
class IUIToKernel
{
public:
	virtual bool NotifyLogin() = 0;
	virtual bool NotifyGetGameList(STRU_GET_APP_LIST_RS oDataAppListRs) = 0;
	virtual bool NotifyLoginRs(INT64 m_i64UserID,bool bRes) = 0;
	virtual bool NotifyRegisterRs(long lUserID,bool bRes) = 0;
	virtual bool NotifyGetGameListRs(bool bRes) = 0;
};
//文件头
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

//IKernelToUI   UI 通知  Kernel  接口   UI  交互方法
//IObserver     实现被观察者  中的  UnInit    方法
//IKernel       网络  通知 Kernel 收数据  RecvData  方法   
class CKernel:public IKernelToUI,public INetToKernel
{
	//私有化构造，析构，等号运算符重载，拷贝构造
public:
	CKernel();
	~CKernel();
public:
	//打开kernel
	bool InitKernel(IUIToKernel* pUI);
	void UnInitKernel();
public:
	//提供给网络回调数据的接口
	void OnRecvData(const char* pData, long lDataLen,ENUM_RECV_TYPE eType);
public:
	//通知被观察者的接口
	virtual void UnInit();
	//UI发送数据使用
	long SendData(char* pData,long lDataLen);
public:
	 //UI交互接口	
////////////////////////////////////////////////////////////////////////////////////////////////////
	virtual bool DeleteGame(long lGameID);
	virtual bool DownLoadGame(long lGameID);
	virtual bool GetGameList();
	virtual bool PlayGameWithStranger(long lUserID,long lGameID);
	virtual bool UserLogin(INT64 i64UserID, WORD wPasswordLen, char* szPassword);
/////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//协议  下标  对应的  函数指针  数组
////////////////////////////////////////////////////////////////////////////////////////////////////////////
public:
	//定义处理函数指针类型，统一不同协议的调用方式
	typedef BOOL (CKernel::*DEAL_FUN)(const char* szRecvBuf,long lRecvLen);
	//定义函数指针数组
	DEAL_FUN m_pDealFun[DEF_PROTOCOL_COUNT];
	//定义处理函数
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
	//通知UI的分类处理函数
	bool NotifyToUI(void* pNotify);


	//写入配置文件
	BOOL WriteMyIniFile(STRU_PLAY_STRANGER_RS oDataRs);
	//启动游戏
	BOOL NoticeExeGame(STRU_PLAY_STRANGER_RS oDataRs);
private://上传下载文件操作
	//文件接收线程
	static unsigned int WINAPI OnFileTrans(void* pParam);
	void OnFileTrans();

	//判断某路径下的文件是否存在，如果不存在创建
	BOOL IsExistFile(INT64 i64UserID, char* pFileName, WORD wNameLen)
	{
		int num = 0;
		//合成路径
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
			//创建目录
			TCHAR str1[MAX_PATH] = {0};
			MultiByteToWideChar(CP_ACP, 0, szDir, -1, str1, strlen(szDir));
			::CreateDirectory(str1, NULL);
			num = GetLastError();
			//创建文件
			//MultiByteToWideChar(CP_ACP, 0, szPath, -1, str, strlen(szPath));
			hFile = ::CreateFile(str, FILE_READ_DATA, FILE_SHARE_READ, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
			num = GetLastError();
			::CloseHandle(hFile);
		}
		//如果文件存在并且已经打开，那么关闭打开的文件
		else
		{
			::CloseHandle(hFile);
		}

		return TRUE;
	}
	//生成文件key（数据库中文件的唯一标识）
	/*inline INT64 GetFileKey(INT64 i64UserID, WORD wVersion, WORD wFileNameLen, const char* pFileName)
	{
		INT64 i64Key = i64UserID * HIBYTE(wVersion) / LOBYTE(wVersion) + wFileNameLen;

		return i64Key;
	}*/
	//计算投递队列
	/*inline long GetPostQueueNum(INT64 i64Key)
	{
		return i64Key % m_lThreadCount;
	}*/
public:
	void NoticeKernelSendLogin(INT64 i64UserID,WORD wPasswordLen,char szPassword[]);
	void NoticeKernelSendPlayStranger(int nIndex);
	void NoticeKernelSendDownLoadGame(int nIndex);
private:
	//游戏信息结构体，路径-长度
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
	
	//文件传输线程参数
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
//上传下载文件传输用
	IFile* m_pFile;
	//为每个传文件线程准备一个任务队列
	CCircleQueue<CFileTransTask> m_pFileTransQueue;
	//传输文件线程数量
	//long m_lThreadCount;
	//线程参数
	STRU_FILE_TRANS_PARAM* m_pThreadParam;
	STRUFILEHEAD m_oFileHead;
//记录游戏路径
	map<int,GamePathInfo*> m_mapGamePath;
//写配置文件
	CMyini* pMyIni; 
//运行标记
	bool m_bRun;			
//处理线程句柄
	HANDLE m_hDealThread;
//UI回调数据接口
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