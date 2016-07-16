#include "stdafx.h"
#include "Kernel.h"

#define DEF_THREAD_QUEUE_LEN	(1000)
#define DEF_MAX_CACHE    (1200)
CKernel::CKernel() : m_bRun(false), m_hDealThread(NULL), m_pUdp(NULL),m_pTcp(NULL),pMyIni(NULL),m_pFile(NULL)
{
	m_i64UserID = 0;
	m_i64VerifyCode = 0;
	m_iGameID = 0;
	m_pDealFun[DEF_HEARTBEAT_RQ - DEF_PROTOCOL_BASE]      = &CKernel::OnHeartBeatRQ;
	m_pDealFun[DEF_LOGIN_RS - DEF_PROTOCOL_BASE]   = &CKernel::OnLoginRS;
	m_pDealFun[DEF_REGISTER_RS - DEF_PROTOCOL_BASE]     = &CKernel::OnRegisterRS;
	m_pDealFun[DEF_GET_GAME_LIST_RS - DEF_PROTOCOL_BASE] = &CKernel::OnGetGameListRS;
	m_pDealFun[DEF_DOWNLOAD_GAME_RS - DEF_PROTOCOL_BASE] = &CKernel::OnDownloadGameRS;
	m_pDealFun[DEF_PLAY_STRANGER_RS - DEF_PROTOCOL_BASE] = &CKernel::OnPlayStrangerRS;
	m_pDealFun[DEF_GET_FRIEND_LIST_RS - DEF_PROTOCOL_BASE] = &CKernel::OnGetFriendListRS;
	m_pDealFun[DEF_INVITE_FRIEND_RS - DEF_PROTOCOL_BASE] = &CKernel::OnInviteFriendRS;
	m_pDealFun[DEF_BE_INVITE_FRIEND_RQ - DEF_PROTOCOL_BASE] = &CKernel::OnBeInviteFriendRS;
	m_pDealFun[DEF_ADD_FRIEND_RS - DEF_PROTOCOL_BASE] = &CKernel::OnAddFriendRS;
	m_pDealFun[DEF_DELETE_FRIEND_RS - DEF_PROTOCOL_BASE] = &CKernel::OnDeleteFriendRS;
}

CKernel::~CKernel()
{
}

//////////////////////////////////////////////////////////////////////////////////
//打开kernel
bool CKernel::InitKernel(IUIToKernel* pUI)
{
	UnInitKernel();//防止资源泄露	
	m_bRun = true;//设置运行标记
	m_pUI = pUI;//保存UI指针
	CFactoryNet oFactoryNet;//简单工厂拿到UDP
	m_pTcp = oFactoryNet.CreateNet(enum_net_tcp);
	m_pUdp = oFactoryNet.CreateNet(enum_net_udp);
	m_pUdp->InitNet(this);//Tcp/Udp初始化
	
	int idefine = DEF_LOGIN_RS;
	NotifyToUI((void*)&idefine);
	//启动一个线程，接收文件
	//创建文件传输线程的任务队列
	m_pFileTransQueue.InitQueue(DEF_THREAD_QUEUE_LEN);

	//创建线程参数
	m_pThreadParam = new STRU_FILE_TRANS_PARAM;

	//启动文件传输线程
	m_pThreadParam->m_pThis = this;
	HANDLE hThread = (HANDLE)::_beginthreadex(NULL, 0, OnFileTrans, m_pThreadParam, 0, NULL);
	::CloseHandle(hThread);

	return true;
}

void CKernel::UnInitKernel()
{
	//设置运行标记
	m_bRun = false;
	m_pUI = NULL;
	m_pUdp = NULL;
	m_pTcp = NULL;
}



//UI发送
long CKernel::SendData(char* pData,long lDataLen)
{
	return m_pUdp->SendData(pData,lDataLen);
}

void CKernel::UnInit()
{
	UnInitKernel();
}

/////////////////////////////////////////////////////////////////////////////////////////
// UI交互接口


//通知UI的分类处理函数
bool CKernel::NotifyToUI(void* pNotify)
{
	//获取包类型
	int iType = *(int*)pNotify;

	//根据包类型分类处理
	switch (iType)
	{
	case DEF_LOGIN_RS:
		{
			//通知UI
			m_pUI->NotifyLogin();
			//m_pUI->NotifyLoginRs(((STRU_LOGIN_RS*)pNotify)->m_i64UserID, 
				//((STRU_LOGIN_RS*)pNotify)->m_wResult == enum_rs_success ? true : false);
		}
		break;
	case DEF_REGISTER_RS:
		{
			//通知UI
		//	m_pUI->NotifyRegisterRs(((STRU_REGISTER_RS*)pNotify)->m_i64UserID,
		//		((STRU_REGISTER_RS*)pNotify)->m_wResult == enum_rs_success ? true : false);
		}
		break;
	case DEF_GET_GAME_LIST_RS:
		{
			//通知UI
			//m_pUI->NotifyGetGameListRs(
			//	((STRU_GET_GAME_LIST_RS*)pNotify) == enum_rs_success ? true : false);

		}
		break;
	case DEF_DOWNLOAD_GAME_RS:
		{
			//TODO:	STRU_DOWNLOAD_GAME_RS
		}
		break;
	case DEF_PLAY_STRANGER_RS:
		{
			//STRU_PLAY_STRANGER_RS
		}
		break;
	//case DEF_RECV_GAME_DATA_RS:
	//	{
	//		
	//	}
	//	break;
	}
	return true;
}

bool CKernel::DeleteGame(long lGameID)
{

	return true;
}
bool CKernel::DownLoadGame(long lGameID)
{

	return true;
}
bool CKernel::GetGameList()
{

	return true;
}

bool CKernel::PlayGameWithStranger(long lUserID,long lGameID)
{

	return true;
}


bool CKernel::UserLogin(INT64 i64UserID, WORD wPasswordLen, char* szPassword)
{
	//TASK_QUEUE::iterator it = m_oTaskQueue.find(DEF_LOGIN_RQ);
	//if(it != m_oTaskQueue.end())
	//{
	//	return false;
	//}
	////填充登录请求
	//STRU_LOGIN_RQ * pRq = new STRU_LOGIN_RQ;
	//pRq->m_i64UserID = i64UserID;
	//pRq->m_wPasswordLen = wPasswordLen;
	//memset(pRq->m_szPassword, 0, DEF_MAX_PASSWORD_LEN);
	//memcpy(pRq->m_szPassword, szPassword, pRq->m_wPasswordLen);

	////创建任务
	//STRU_TASK *pTask = new STRU_TASK;
	//pTask->m_pRq = pRq;
	//pTask->m_dwLastSendTime = 0;

	////设置任务的状态
	//pTask->m_eState = enum_task_init;

	////投递任务
	//m_oTaskQueue[DEF_LOGIN_RQ] = pTask;

	return true;
}
//提供给网络回调数据的接口
void CKernel::OnRecvData(const char* pData, long lDataLen,ENUM_RECV_TYPE eType)
{
	if(eType == enum_recv_udp)
	{
		//获取协议类型
		WORD wType = *(WORD*)pData;
		//计算位置
		wType -= DEF_PROTOCOL_BASE;
		//统一调用
		if(wType < DEF_PROTOCOL_COUNT)
		{
			(this->*this->m_pDealFun[wType])(pData,lDataLen);
		}
	}
	else
	{
		//获取协议类型
		WORD wType = *(WORD*)pData;

		//是否为文件的第一块
		DWORD dwHeadLen = 0;
		//INT64 i64FileKey = 0;
		char szSerialize[DEF_MAX_BUF];
		if (m_oFileHead.m_pFileHeadDownload == NULL)
		{
			//获取文件头
			m_oFileHead.m_pFileHeadDownload = new STRU_FILE_HEAD_DOWNLOAD;
			m_oFileHead.m_pFileHeadDownload->UnSerialize(pData, lDataLen);
			dwHeadLen = m_oFileHead.m_pFileHeadDownload->Serialize(szSerialize, DEF_MAX_BUF);
		}
		//创建任务
		CFileTransTask* pTask = new CFileTransTask;
		//初始化任务
		pTask->m_i64UserID = m_oFileHead.m_pFileHeadDownload->m_i64UserID;
		pTask->m_pFileHead = &m_oFileHead;
		pTask->m_dwFileSize = m_oFileHead.m_pFileHeadDownload->m_dwFileLength;
		pTask->m_wFileNameLen = m_oFileHead.m_pFileHeadDownload->m_wAppNameLen;
		memcpy(pTask->m_szFileName, m_oFileHead.m_pFileHeadDownload->m_szAppName, pTask->m_wFileNameLen);
		pTask->m_dwFileLength = lDataLen - dwHeadLen;
		pTask->m_pFileContent = new char[pTask->m_dwFileLength];
		memcpy(pTask->m_pFileContent, &pData[dwHeadLen], pTask->m_dwFileLength);
		//投递
		m_pFileTransQueue.PushQueue(pTask);
	}
}


//UDP消息处理
BOOL CKernel::OnHeartBeatRQ(const char* szRecvBuf,long lRecvLen)
{

	return TRUE;
}
BOOL CKernel::OnLoginRS(const char* szRecvBuf,long lRecvLen)
{
	int idefine = DEF_LOGIN_RS;
	
	STRU_LOGIN_RS oDataRs;

	char szBuf[1200];
	long lSendLen = 0;
	STRU_GET_APP_LIST_RQ oDataRq;
	STRU_CONNECT_MESSAGE_RQ oDataTcpRq;

	if(FALSE == oDataRs.UnSerialize(szRecvBuf,lRecvLen)) return FALSE;
	m_i64UserID = oDataRs.m_i64UserID;
	m_i64VerifyCode = oDataRs.m_i64VerifyCode;
	switch(oDataRs.m_wResult)
	{
	case enum_res_success:
		//发送获取App列表请求
		oDataRq.m_i64AppKey = 0;
		oDataRq.m_i64UserID = m_i64UserID;
		oDataRq.m_i64VerifyCode = oDataRs.m_i64VerifyCode;
		
		lSendLen = oDataRq.Serialize(szBuf, 1200);
		while(SendData(szBuf,lSendLen) < 0);
		//初始化TCP，发送TCP，让服务器保存
		m_pTcp->InitNet(this);
		
		oDataTcpRq.m_i64UserID = m_i64UserID;
		oDataTcpRq.m_i64VerifyCode = m_i64VerifyCode;
		lSendLen = oDataTcpRq.Serialize(szBuf, 1200);
		while(m_pTcp->SendData(szBuf,lSendLen) < 0);
		break;
	case enum_res_unexist: 
		NotifyToUI((void*)&idefine);
		break;
	case enum_res_password_error:
		NotifyToUI((void*)&idefine);
		break;
	case enum_res_fail:
		NotifyToUI((void*)&idefine);
		break;
	}
	return TRUE;
}
BOOL CKernel::OnRegisterRS(const char* szRecvBuf,long lRecvLen)
{

	return TRUE;
}
BOOL CKernel::OnGetGameListRS(const char* szRecvBuf,long lRecvLen)
{
	//收应答
	STRU_GET_APP_LIST_RS oDataAppListRs;
	if(FALSE == oDataAppListRs.UnSerialize(szRecvBuf, lRecvLen)) return FALSE;
	//保存游戏信息，方便以后启动，游戏路径
	//通知UI显示
	m_pUI->NotifyGetGameList(oDataAppListRs);
	
	return TRUE;
}
BOOL CKernel::OnDownloadGameRS(const char* szRecvBuf,long lRecvLen)
{

	return TRUE;
}
BOOL CKernel::OnPlayStrangerRS(const char* szRecvBuf,long lRecvLen)
{
	//解析数据
	STRU_PLAY_STRANGER_RS oDataRs;
	if(FALSE == oDataRs.UnSerialize(szRecvBuf,lRecvLen)) return FALSE;
	//判断是否成功
	if(enum_re_justwait == oDataRs.m_wResult || enum_re_fail == oDataRs.m_wResult)
	{

	}
	else
	{
		//写入配置文件
		WriteMyIniFile(oDataRs);
		//启动游戏
		NoticeExeGame(oDataRs);
	}
	return TRUE;
}
BOOL CKernel::OnGetFriendListRS(const char* szRecvBuf,long lRecvLen)
{

	return TRUE;
}
BOOL CKernel::OnInviteFriendRS(const char* szRecvBuf,long lRecvLen)
{

	return TRUE;
}
BOOL CKernel::OnBeInviteFriendRS(const char* szRecvBuf,long lRecvLen)
{

	return TRUE;
}
BOOL CKernel::OnAddFriendRS(const char* szRecvBuf,long lRecvLen)
{

	return TRUE;
}
BOOL CKernel::OnDeleteFriendRS(const char* szRecvBuf,long lRecvLen)
{

	return TRUE;
}

//TCP消息接收文件
//文件接收线程
unsigned int WINAPI CKernel::OnFileTrans(void* pParam)
{
	//转换参数
	STRU_FILE_TRANS_PARAM* pThis = (STRU_FILE_TRANS_PARAM*)pParam;

	//调用普通线程函数
	pThis->m_pThis->OnFileTrans();

	return 1L;
}
void CKernel::OnFileTrans()
{
	//循环处理自己队列中的任务
	while (m_bRun == TRUE)
	{
		CFileTransTask* pTask = NULL;
		while (m_pFileTransQueue.PopQueue(pTask) == TRUE)
		{
			//处理任务(写文件)
			if(NULL == m_pFile)
			{
				//查找已经打开的文件对象
				IsExistFile(pTask->m_i64UserID, pTask->m_szFileName, pTask->m_wFileNameLen);
				//创建文件对象 并打开文件
				char szPath[MAX_PATH];
				sprintf_s(szPath, MAX_PATH, "D:/GamePlat/%s",pTask->m_szFileName);
				//sprintf_s(szPath, MAX_PATH, "./%I64d/%s", pTask->m_i64UserID, pTask->m_szFileName);
				m_pFile = new CFileOpr;
				m_pFile->OpenFile(szPath, enum_opr_write);
				int Num = GetLastError();
				//保存路径
				GamePathInfo* oGamePath = NULL;
				oGamePath = new GamePathInfo;
				oGamePath->m_wAppPathLen = strlen(szPath);
				strcpy_s(oGamePath->m_szAppPath,1024,szPath);
				m_mapGamePath[m_iGameID] = oGamePath;
			}

			//写文件
			DWORD dwError = ::GetLastError();
			long lLen = m_pFile->WriteFile(pTask->m_pFileContent, pTask->m_dwFileLength);
			dwError = ::GetLastError();

			//查看当前文件实际大小是否等于文件的真实大小
			DWORD dwSize = m_pFile->GetFileSize();
			if (dwSize >= pTask->m_dwFileSize)
			{
				//关闭文件对象
				m_pFile->CloseFile();
				delete m_pFile;
				m_pFile = NULL;

				//清理文件头
				delete pTask->m_pFileHead->m_pFileHeadDownload;
				pTask->m_pFileHead->m_pFileHeadDownload = NULL;
				pTask->m_pFileHead->m_i64FileKey =0;
			}
		}
		//休息一会
		::Sleep(10);
	}
}






//UI调用Kernel函数，通知Kernel发送登录的消息
void CKernel::NoticeKernelSendLogin(INT64 i64UserID,WORD wPasswordLen,char szPassword[])
{
	STRU_LOGIN_RQ oDataRq;
	oDataRq.m_i64UserID = i64UserID;
	oDataRq.m_wPasswordLen = wPasswordLen;
	memcpy(oDataRq.m_szPassword, szPassword, oDataRq.m_wPasswordLen);
	//序列化
	char szBuf[1000];
	long lSendLen = 0;
	lSendLen = oDataRq.Serialize(szBuf, 1000);

	while(SendData(szBuf,lSendLen) < 0);
}
//UI调用Kernel函数，通知Kernel发送与陌生人游戏的消息
void CKernel::NoticeKernelSendPlayStranger(int nIndex)
{
	STRU_PLAY_STRANGER_RQ oDataRq;
	oDataRq.m_i64UserID = m_i64UserID;
	oDataRq.m_i64VerifyCode = m_i64VerifyCode;
	oDataRq.m_iGameID = nIndex;
	//序列化
	char szBuf[1000];
	long lSendLen = 0;
	lSendLen = oDataRq.Serialize(szBuf, 1000);

	while(SendData(szBuf,lSendLen) < 0);
}
//UI调用Kernel函数，通知Kernel发送下载游戏的消息
void CKernel::NoticeKernelSendDownLoadGame(int nIndex)
{
	STRU_DOWNLOAD_GAME_RQ oDataRq;
	oDataRq.m_i64UserID = m_i64UserID;
	oDataRq.m_i64VerifyCode = m_i64VerifyCode;
	oDataRq.m_iGameID = nIndex;
	m_iGameID = nIndex;
	char szBuf[1000];//发送缓冲区
	long lSendLen = 0;//发送长度
	lSendLen = oDataRq.Serialize(szBuf, 1000);//序列化
	while(SendData(szBuf,lSendLen) < 0);//发送
}


//写入配置文件，启动游戏
BOOL CKernel::WriteMyIniFile(STRU_PLAY_STRANGER_RS oDataRs)
{
	pMyIni = new CMyini; 
	CString str1;
	CString str2;
	CString str3;
	CString str4;
	CString str5;
	str1.Format(L"%d",oDataRs.m_i64UserID);
	str2.Format(L"%d",oDataRs.m_i64StrangerID);
	if(TRUE == oDataRs.m_bIsFirst)
	{
		str3.Format(L"%d",1);
	}
	else
	{
		str3.Format(L"%d",0);
	}
	str4.Format(L"%d",oDataRs.m_iGameID);
	str5.Format(L"%d",oDataRs.m_iIndex);
	pMyIni->GetIniFilePath(m_mapGamePath[oDataRs.m_iGameID]->m_szAppPath);
	pMyIni->WriteIniFile(TEXT("Dragon"), 
								TEXT("m_i64UserID"), 
								str1);
	pMyIni->WriteIniFile(TEXT("Dragon"), 
								TEXT("m_i64OtherID"), 
								str2);
	pMyIni->WriteIniFile(TEXT("Dragon"), 
								TEXT("m_bIsFirst"), 
								str3);
	pMyIni->WriteIniFile(TEXT("Dragon"), 
								TEXT("m_iGameID"), 
								str4);
	pMyIni->WriteIniFile(TEXT("Dragon"), 
								TEXT("m_iIndex"), 
								str5);
	return TRUE;
}
BOOL CKernel::NoticeExeGame(STRU_PLAY_STRANGER_RS oDataRs)
{
	//启动游戏
	//HINSTANCE ivalue = ShellExecute(NULL,L"open",L"F:/科林明伦\科林项目/游戏对战平台设计/游戏端-王彪/2016-02-27-客户端继续/Client/Debug/Dragon.exe",L"字符内容",NULL,SW_SHOWNORMAL);
	//HINSTANCE ivalue = ShellExecute(NULL, L"open",L"F:/科林明伦/科林项目/游戏对战平台设计/游戏端-王彪/2016-02-27-客户端继续/Client/Debug/Dragon.exe", NULL, NULL, SW_SHOWNORMAL);
	TCHAR str[1024] = {0};
	
	MultiByteToWideChar(CP_ACP, 0, m_mapGamePath[oDataRs.m_iGameID]->m_szAppPath, -1, str, m_mapGamePath[oDataRs.m_iGameID]->m_wAppPathLen);
	HINSTANCE ivalue = ShellExecute(NULL, L"open",str, NULL, NULL, SW_SHOWNORMAL);
	if((int)ivalue<32)
	{
		return TRUE;
	}
	return FALSE;
}
