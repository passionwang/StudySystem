#include "Kernel.h"
#include <process.h>
#include <atlstr.h>
//数据库名称
#define DEF_DATABASE_NAME ("AppPlat")

//数据地址
#define DEF_DB_IP		("127.0.0.1,1433")

//用户信息表
#define DEF_USER_TABLE	("USER_INFO")
#define DEF_USER_ID		("user_id")
#define DEF_USER_PASS	("user_pass")
#define DEF_USER_LAST_TIME	("last_time")
#define DEF_USER_STATE	("user_state")

//每个文件传输线程的任务队列的长度
#define DEF_THREAD_QUEUE_LEN	(1000)

CKernel::CKernel() : m_pTcpNet(NULL), m_pUdpNet(NULL),m_iIndex(0)
{
	m_pDealFun[DEF_LOGIN_RQ - DEF_PROTOCOL_BASE] = &CKernel::OnDealLoginRq;
	//m_pDealFun[DEF_UPLOAD_RQ - DEF_PROTOCOL_BASE] = &CKernel::OnDealUploadRq;
	//m_pDealFun[DEF_DOWNLOAD_RQ - DEF_PROTOCOL_BASE] = &CKernel::OnDealDownloadRq;
	m_pDealFun[DEF_GET_APP_LIST_RQ - DEF_PROTOCOL_BASE] = &CKernel::OnDealGetAppListRq;
	m_pDealFun[DEF_INSTALL_RQ - DEF_PROTOCOL_BASE] = &CKernel::OnDealInstallRq;
	m_pDealFun[DEF_UNINSTALL_RQ - DEF_PROTOCOL_BASE] = &CKernel::OnDealUnInstallRq;
	m_pDealFun[DEF_LOGOUT_RQ - DEF_PROTOCOL_BASE] = &CKernel::OnDealLogOutRq;
	m_pDealFun[DEF_READY_PLAY_RQ - DEF_PROTOCOL_BASE] = &CKernel::OnReadyPlayRq;
	m_pDealFun[DEF_NOTICE_END_RQ - DEF_PROTOCOL_BASE] = &CKernel::OnNoticeEndRq;
	m_pDealFun[DEF_SEND_GAME_DATA_RQ - DEF_PROTOCOL_BASE] = &CKernel::OnSendGameDataRq;
	m_pDealFun[DEF_NOTICE_BEGIN_RS - DEF_PROTOCOL_BASE] = &CKernel::OnNoticeBrginRs;
	m_pDealFun[DEF_RECV_GAME_DATA_RS - DEF_PROTOCOL_BASE] = &CKernel::OnRecvGameDataRs;
	m_pDealFun[DEF_PLAY_STRANGER_RQ - DEF_PROTOCOL_BASE] = &CKernel::OnPlayStrangerRq;
	m_pDealFun[DEF_DOWNLOAD_GAME_RQ - DEF_PROTOCOL_BASE] = &CKernel::OnDownLoadGameRq;
}
CKernel::~CKernel()
{
	UnInit();
}

///////////////////////////////////////////////////////////////////////////////////////////
//实现基类模板方法模式静态多台的接口
//提供派生类去实现资源初始化和资源销毁的方法
BOOL CKernel::Init()
{
	//防止多次调用，先销毁
	UnInit();

	//定义一个网络工厂对象
	CFactoryeNet oFactoryNet;

	//初始化线程池对象
	m_oPool.CreatePool(5, 10, 10000);

	//获取系统信息
	SYSTEM_INFO sysInfo;
	::GetSystemInfo(&sysInfo);
	m_lThreadCount = sysInfo.dwNumberOfProcessors * 2;

	//创建文件传输线程的任务队列
	m_pFileTransQueue = new CCircleQueue<CFileTransTask>[m_lThreadCount];
	for (long i = 0; i < m_lThreadCount; ++i)
	{
		m_pFileTransQueue[i].InitQueue(DEF_THREAD_QUEUE_LEN);
	}

	//创建线程参数
	m_pThreadParam = new STRU_FILE_TRANS_PARAM[m_lThreadCount];

	//启动文件传输线程
	m_bStop = FALSE;
	for (long i = 0; i < m_lThreadCount; ++i)
	{
		m_pThreadParam[i].m_lIndex = i;
		m_pThreadParam[i].m_pThis = this;
		HANDLE hThread = (HANDLE)::_beginthreadex(NULL, 0, OnFileTrans, &m_pThreadParam[i], 0, NULL);
		::CloseHandle(hThread);
	}

	//创建TCP对象
	m_pTcpNet = oFactoryNet.CreateNet(enum_net_tcp);
	m_pTcpNet->InitNet(this);

	//创建UDP对象
	m_pUdpNet = oFactoryNet.CreateNet(enum_net_udp);
	m_pUdpNet->InitNet(this);

	//打开数据哭
	if (FALSE == m_oDao.OpenDateBase(DEF_DATABASE_NAME, DATEBASE_TYPE_SQL2005, "sa", "passion", DEF_DB_IP))
	{
		return FALSE;
	}

	////读取用户基本信息到内存中构建用户信息表
	ReadUserInfo();

	return TRUE;
}

void CKernel::UnInit()
{
	//TODO:
}

//文件接收线程
unsigned int WINAPI CKernel::OnFileTrans(void* pParam)
{
	//转换参数
	STRU_FILE_TRANS_PARAM* pThis = (STRU_FILE_TRANS_PARAM*)pParam;

	//调用普通线程函数
	pThis->m_pThis->OnFileTrans(pThis->m_lIndex);

	return 1L;
}
void CKernel::OnFileTrans(long lIndex)
{
	//循环处理自己队列中的任务
	while (m_bStop == FALSE)
	{
		CFileTransTask* pTask = NULL;
		while (m_pFileTransQueue[lIndex].PopQueue(pTask) == TRUE)
		{
			//处理任务(写文件)
			//查找已经打开的文件对象
			IFile* pFile = NULL;
			FILE_OBJECT_MAP::iterator it = m_oFileMap.find(pTask->m_i64FileKey);
			if (it == m_oFileMap.end())
			{
				//判断对应的文件是否存在，不存在创建
				IsExistFile(pTask->m_i64UserID, pTask->m_szFileName, pTask->m_wFileNameLen);
				//创建文件对象 并打开文件
				char szPath[MAX_PATH];
				sprintf_s(szPath, MAX_PATH, "./%I64d/%s", pTask->m_i64UserID, pTask->m_szFileName);
				pFile = new CFileOpr;
				pFile->OpenFile(szPath, enum_opr_write);

				//2.保存打开的文件对象等待传送下一个文件
				m_oFileMap[pTask->m_i64FileKey] = pFile;
			}
			else
			{
				pFile = it->second;
			}

			//写文件
			DWORD dwError = ::GetLastError();
			long lLen = pFile->WriteFile(pTask->m_pFileContent, pTask->m_dwFileLength);
			dwError = ::GetLastError();

			//查看当前文件实际大小是否等于文件的真实大小
			DWORD dwSize = pFile->GetFileSize();
			if (dwSize >= pTask->m_dwFileSize)
			{
				//关闭文件对象
				pFile->CloseFile();
				delete pFile;

				//清理文件头
				if(UP_FILE == pTask->m_pSession->eType)
				{
					delete pTask->m_pSession->m_pFileHeadUp;
					pTask->m_pSession->m_pFileHeadUp = NULL;
					pTask->m_pSession->m_i64FileKey =0;
					//通知客户端可以传下一个文件了
					STRU_FILE_TRANS_COMPLETE_UPLOAD oFileTrans;
					oFileTrans.m_i64FileKey = pTask->m_i64FileKey;
					oFileTrans.m_i64UserID = pTask->m_i64UserID;
					oFileTrans.m_wAppNameLen = pTask->m_wFileNameLen;
					memcpy_s(oFileTrans.m_szAppName, 20,pTask->m_szFileName, oFileTrans.m_wAppNameLen);
					//发送
					char szBuf[DEF_MAX_BUF];
					long lSendLen = 0;
					lSendLen = oFileTrans.Serialize(szBuf, DEF_MAX_BUF);
					m_pTcpNet->SendData(pTask->m_pSession, szBuf, lSendLen);
				}
				else
				{
					delete pTask->m_pSession->m_pFileHeadDown;
					pTask->m_pSession->m_pFileHeadDown = NULL;
					pTask->m_pSession->m_i64FileKey =0;
					//通知客户端可以传下一个文件了
					STRU_FILE_TRANS_COMPLETE_DOWNLOAD oFileTrans;
					oFileTrans.m_i64FileKey = pTask->m_i64FileKey;
					oFileTrans.m_i64UserID = pTask->m_i64UserID;
					oFileTrans.m_wAppNameLen = pTask->m_wFileNameLen;
					memcpy_s(oFileTrans.m_szAppName,20, pTask->m_szFileName, oFileTrans.m_wAppNameLen);
					//发送
					char szBuf[DEF_MAX_BUF];
					long lSendLen = 0;
					lSendLen = oFileTrans.Serialize(szBuf, DEF_MAX_BUF);
					m_pTcpNet->SendData(pTask->m_pSession, szBuf, lSendLen);
				}
				

				
				
				
			}
		}

		//休息一会
		::Sleep(10);
	}

	//减少线程数量
	::InterlockedDecrement(&m_lThreadCount);
}

//从数据哭加载用户信息
BOOL CKernel::ReadUserInfo()
{
	std::list<TCHAR*> columnList;
	columnList.push_back(DEF_USER_ID);
	columnList.push_back(DEF_USER_PASS);
	columnList.push_back(DEF_USER_LAST_TIME);
	columnList.push_back(DEF_USER_STATE);
	std::list<TCHAR*> resList;

	//读取
	if (FALSE == m_oDao.GetData("select * from USER_INFO", columnList, columnList.size(), resList))
	{
		return FALSE;
	}

	//构造用户信息表
	long lCount = resList.size() / columnList.size();
	std::list<TCHAR*>::iterator it = resList.begin();
	for (long i = 0; i < lCount; ++i)
	{
		//创建一条用户信息
		STRU_USER_INFO* pUserInfo = new STRU_USER_INFO;

		//填充一条用户信息
		pUserInfo->m_i64UserID = _atoi64(*it++);
		pUserInfo->m_wPasswordLen = strlen(*it);
		memcpy(pUserInfo->m_szPassword, *it++, pUserInfo->m_wPasswordLen);
		pUserInfo->m_i64LastTime = _atoi64(*it++);
		pUserInfo->m_wState = atoi(*it++);

		//保存一条用户信息
		m_oUserMap[pUserInfo->m_i64UserID] = pUserInfo;
	}

	return TRUE;
}

//读取app列表
BOOL CKernel::ReadAppList(INT64 i64AppKey, long lCount, STRU_GET_APP_LIST_RS& oDataRs)
{
	//获取
	//select APP_INFO.app_key, APP_INFO.shared_user, APP_INFO.down_times, APP_INFO.up_time, 
	//APP_INFO.app_info, APP_KEY.app_name, APP_KEY.app_version 
	//from APP_INFO, APP_KEY where APP_INFO.app_key = APP_KEY.app_key
	std::list<TCHAR*> columnList;
	columnList.push_back("app_key");
	columnList.push_back("shared_user");
	columnList.push_back("down_times");
	columnList.push_back("up_time");
	columnList.push_back("app_info");
	columnList.push_back("icon_path");
	columnList.push_back("app_path");
	columnList.push_back("app_name");

	std::list<TCHAR*> resList;

	//读取
	if (FALSE == m_oDao.GetData(" \
	select APP_INFO.app_key, APP_INFO.shared_user, APP_INFO.down_times, APP_INFO.up_time,\
	APP_INFO.app_info,APP_INFO.icon_path, APP_INFO.app_path, APP_KEY.app_name \
	from APP_INFO, APP_KEY where APP_INFO.app_key = APP_KEY.app_key", columnList, columnList.size(), resList))
	{
		return FALSE;
	}

	//遍历填充应答
	long lItemCount = resList.size() / columnList.size();
	std::list<TCHAR*>::iterator it = resList.begin();
	for (long i = 0; i < lItemCount; ++i)
	{
		oDataRs.m_oAppList[i].m_i64AppKey = _atoi64(*it++);
		oDataRs.m_oAppList[i].m_i64UserID = _atoi64(*it++);
		oDataRs.m_oAppList[i].m_dwDownTimes = atoi(*it++);
		oDataRs.m_oAppList[i].m_dwSharedTime = _atoi64(*it++);
		strcpy(oDataRs.m_oAppList[i].m_szAppDescript, *it);
		oDataRs.m_oAppList[i].m_wAppDesLen = strlen(oDataRs.m_oAppList[i].m_szAppDescript);
		it++;
		strcpy(oDataRs.m_oAppList[i].m_szIconPath, *it);
		oDataRs.m_oAppList[i].m_wAppIconPathLen = strlen(oDataRs.m_oAppList[i].m_szIconPath);
		it++;
		strcpy(oDataRs.m_oAppList[i].m_szAppPath, *it);
		oDataRs.m_oAppList[i].m_wAppPathLen= strlen(oDataRs.m_oAppList[i].m_szAppPath);
		it++;
		strcpy(oDataRs.m_oAppList[i].m_szAppName, *it);
		oDataRs.m_oAppList[i].m_wAppNameLen = strlen(oDataRs.m_oAppList[i].m_szAppName);
		it++;
		oDataRs.m_wAppItemCount++;
		if (i >= lCount)
		{
			break;
		}
	}

	return TRUE;
}
///////////////////////////////////////////////////////////////////////////////////////////
//实现接收网络数据的方法
BOOL CKernel::OnRecvData(STRU_SESSION* pSession, const char* pData, long lDataLen, int iType)
{
	if (iType == enum_recv_udp)
	{
		//创建任务
		CMyTask* pTask = new CMyTask;

		//填充任务
		pTask->m_pSession = pSession;
		memcpy_s(pTask->m_szData,1200, pData, lDataLen);
		pTask->m_lDataLen = lDataLen;
		pTask->m_pKernel = this;

		//投递任务到线程池中
		int iTimes = 3;
		while (m_oPool.PushTask(pTask) == FALSE)
		{
			::Sleep(1);
			if (--iTimes <= 0)
			{
				delete pTask;
				return FALSE;
			}
		}
	}
	else if (iType == enum_recv_tcp)
	{
		//获取协议类型
		WORD wType = *(WORD*)pData;
		switch (wType)
		{
		case DEF_DOWNLOAD_RQ:
			{
				//TODO:
			}
			break;
		case DEF_CONNECT_MESSAGE_RQ:
			{
				//TODO:
				SaveTcpSession(pSession, lDataLen,pData);
			}
			break;
		case DEF_UPLOAD_RQ:
			{
				//转换会话为TCP会话
				STRU_TCP_SESSION* pTcpSession = (STRU_TCP_SESSION*)pSession;

				//是否为文件的第一块
				DWORD dwHeadLen = 0;
				INT64 i64FileKey = 0;
				char szSerialize[DEF_MAX_BUF];
				if (pTcpSession->m_pFileHeadUp == NULL)
				{
					//获取文件头
					pTcpSession->m_pFileHeadUp = new STRU_FILE_HEAD_UPLOAD;
					pTcpSession->m_pFileHeadUp->UnSerialize(pData, lDataLen);
					dwHeadLen = pTcpSession->m_pFileHeadUp->Serialize(szSerialize, DEF_MAX_BUF);
					pTcpSession->eType = UP_FILE;
					//生成文件key
					pTcpSession->m_i64FileKey = GetFileKey(pTcpSession->m_pFileHeadUp->m_i64UserID, 
					pTcpSession->m_pFileHeadUp->m_wVersion, 
					pTcpSession->m_pFileHeadUp->m_wAppNameLen, 
					pTcpSession->m_pFileHeadUp->m_szAppName);
				}

				//计算投递的队列编号
				i64FileKey = pTcpSession->m_i64FileKey;
				long lIndex = GetPostQueueNum(i64FileKey);

				//创建任务
				CFileTransTask* pTask = new CFileTransTask;

				//初始化任务
				pTask->m_i64UserID = pTcpSession->m_pFileHeadUp->m_i64UserID;
				pTask->m_i64FileKey = i64FileKey;
				pTask->m_pSession = (STRU_TCP_SESSION*)pSession;
				pTask->m_dwFileSize = pTcpSession->m_pFileHeadUp->m_dwFileLength;
				pTask->m_wFileNameLen = pTcpSession->m_pFileHeadUp->m_wAppNameLen;
				memcpy_s(pTask->m_szFileName,20, pTcpSession->m_pFileHeadUp->m_szAppName, pTask->m_wFileNameLen);
				pTask->m_dwFileLength = lDataLen - dwHeadLen;
				pTask->m_pFileContent = new char[pTask->m_dwFileLength];
				memcpy_s(pTask->m_pFileContent,2*1024*1024, &pData[dwHeadLen], pTask->m_dwFileLength);

				//投递
				m_pFileTransQueue[lIndex].PushQueue(pTask);
			}
			break;
		default:
			
			break;
		}

	}

	return TRUE;
}
BOOL CKernel::SaveTcpSession(STRU_SESSION* pSession, long lLen, const char* pData)
{
	//解析数据验证身份
	DEF_UNSERIALIZE_VERIFY(CONNECT_MESSAGE);
	m_mapTcpSession[oDataRq.m_i64UserID] = pSession;
	return TRUE;
}
//实现为任务处理数据的方法
BOOL CKernel::DealData(STRU_SESSION* pSession, long lLen, const char* pData)
{
	//获取协议类型
	WORD wType = *(WORD*)pData;

	//计算位置
	wType -= DEF_PROTOCOL_BASE;

	//统一调用
	(this->*this->m_pDealFun[wType])(pSession, lLen, pData);

	//由于switch打破了程序的开闭原则，所以，通过仿写MFC消息映射表的实现，来去掉switch，统一以后协议的变化
	//(*m_pFun[wType])();
	//
	////根据类型做处理
	//switch (wType)
	//{
	//case DEF_LOGIN_RQ:
	//	{
	//		DealLoginRq();
	//		////定义对应协议结构
	//		//STRU_LOGIN_RQ oDataRq;

	//		////解析收到的数据
	//		//oDataRq.UnSerialize(pData, lLen);

	//		////处理
	//		////查找该用户信息

	//		////比较密码

	//	}
	//	break;

	//case DEF_UPLOAD_RQ:
	//	{
	//	
	//	}
	//	break;
	//}
	return TRUE;
}
//BOOL CKernel::OnDealUploadRq(STRU_SESSION* pSession, long lLen, const char* pData)
//{
//	return TRUE;
//}
//BOOL CKernel::OnDealDownloadRq(STRU_SESSION* pSession, long lLen, const char* pData)
//{
//	return TRUE;
//}
//为每个请求定义对应的处理方法
BOOL CKernel::OnDealLoginRq(STRU_SESSION* pSession, long lLen, const char* pData)
{
	//解析数据：
	STRU_LOGIN_RQ oDataRq;
	STRU_LOGIN_RS oDataRs;
	if (0 == oDataRq.UnSerialize((char*)pData, lLen))
	{
		return FALSE;
	}

	//根据用户名称获取用户信息
	USER_MAP::iterator it = m_oUserMap.find(oDataRq.m_i64UserID);

	oDataRs.m_i64UserID = oDataRq.m_i64UserID;
	//如果没有获取到直接应答用户不存在
	if (it == m_oUserMap.end())
	{
		oDataRs.m_wResult = enum_res_unexist;
	}
	else
	{
		//比较密码长度
		if (it->second->m_wPasswordLen != oDataRq.m_wPasswordLen)
		{
			oDataRs.m_wResult = enum_res_password_error;
		}
		//比较密码是否正确
		else
		{
			if (memcmp(oDataRq.m_szPassword, it->second->m_szPassword, oDataRq.m_wPasswordLen) == 0)
			{
				//如果正确，应答登陆成功
				oDataRs.m_wResult = enum_res_success;

				//补全用户会话信息
				pSession->m_dwAccount = oDataRq.m_i64UserID;

				//生成以后用来验证用户身份的验证码，病保存
				oDataRs.m_i64VerifyCode = GetVerifyCode(oDataRq.m_i64UserID);
				it->second->m_i64VerifyCode = oDataRs.m_i64VerifyCode;
			}
			else
			{
				//如果不正确，应答登陆密码错误
				oDataRs.m_wResult = enum_res_password_error;
			}
		}
	}

	//序列化发送
	DEF_RESPONSE();

	return TRUE;
}
BOOL CKernel::OnDealGetAppListRq(STRU_SESSION* pSession, long lLen, const char* pData)
{
	//解析数据验证身份
	DEF_UNSERIALIZE_VERIFY(GET_APP_LIST);

	//读取数据库
	STRU_GET_APP_LIST_RS oDataRs;
	oDataRs.m_i64UserID = oDataRq.m_i64UserID;
	ReadAppList(oDataRq.m_i64AppKey, DEF_GET_APP_LIST_COUNT, oDataRs);
	//保存游戏信息，方便以后启动，游戏路径
	GamePathInfo* oGamePath = NULL;
	for(int i=0;i<oDataRs.m_wAppItemCount;i++)
	{
		oGamePath = new GamePathInfo;
		//路径信息
		oGamePath->m_wAppPathLen = oDataRs.m_oAppList[i].m_wAppPathLen;
		oDataRs.m_oAppList[i].m_szAppPath[oDataRs.m_oAppList[i].m_wAppPathLen] = '\0';
		strcpy_s(oGamePath->m_szPath,oGamePath->m_wAppPathLen+1,oDataRs.m_oAppList[i].m_szAppPath);
		//名字信息
		oGamePath->m_wAppNameLen = oDataRs.m_oAppList[i].m_wAppNameLen;
		oDataRs.m_oAppList[i].m_szAppName[oDataRs.m_oAppList[i].m_wAppNameLen] = '\0';
		strcpy_s(oGamePath->m_szAppName,oGamePath->m_wAppNameLen+1,oDataRs.m_oAppList[i].m_szAppName);
		m_vecGamePath.push_back(oGamePath);
	}
	//发送
	DEF_RESPONSE();
	return TRUE;
}
BOOL CKernel::OnDealInstallRq(STRU_SESSION* pSession, long lLen, const char* pData)
{
	//解析数据验证身份
	DEF_UNSERIALIZE_VERIFY(INSTALL);

	//写入安装者信息
	/*char szSql[100];
	sprintf(szSql, "insert into install_info values(%I64d, %I64d, %I64d)", 
		oDataRq.m_i64UserID, ::GetTickCount(), oDataRq.m_i64AppKey);
	m_oDao.EditData(szSql);*/
	return TRUE;
}
BOOL CKernel::OnDealUnInstallRq(STRU_SESSION* pSession, long lLen, const char* pData)
{
	//解析数据验证身份
	DEF_UNSERIALIZE_VERIFY(UNINSTALL);

	////写入安装者信息
	//char szSql[100];
	//sprintf(szSql, "insert into uninstall_info values(%I64d, %I64d, %I64d)", 
	//	oDataRq.m_i64UserID, ::GetTickCount(), oDataRq.m_i64AppKey);
	//m_oDao.EditData(szSql);
	return TRUE;
}
BOOL CKernel::OnDealLogOutRq(STRU_SESSION* pSession, long lLen, const char* pData)
{
	//解析数据验证身份
	DEF_UNSERIALIZE_VERIFY(LOGOUT);

	//通知网络关闭连接
	m_pUdpNet->BreadConn(pSession);

	//清空-用户信息中的用户验证码和登陆状态
	it->second->m_i64LastTime = ::GetTickCount();
	it->second->m_i64VerifyCode = 0;
	it->second->m_wState = 0;

	//更新数据库中用户最后登陆的时间和在线状态
	/*char szSql[100];
	sprintf(szSql, "update USER_INFO set last_time = %I64d, user_state = %d where user_id = %I64d", 
		it->second->m_i64LastTime, it->second->m_wState, it->second->m_i64UserID);
	m_oDao.EditData(szSql);*/

	return TRUE;
}
BOOL CKernel::OnReadyPlayRq(STRU_SESSION* pSession, long lLen, const char* pData)
{
	STRU_READY_PLAY_RQ oDataRq; 
	if (0 == oDataRq.UnSerialize((char*)pData, lLen)) 
	{ 
		return FALSE; 
	} 
	//存放当前会话，方便以后的交流
	if(oDataRq.m_i64UserID == m_mapAttackGamePlayer[oDataRq.m_iGameID]->find(oDataRq.m_iIndex)->second->m_pGamePlayer1->m_i64UserID)
	{
		m_mapAttackGamePlayer[oDataRq.m_iGameID]->find(oDataRq.m_iIndex)->second->m_pGamePlayer1->m_pStruSessionGame = pSession;
	}
	else
	{
		m_mapAttackGamePlayer[oDataRq.m_iGameID]->find(oDataRq.m_iIndex)->second->m_pGamePlayer2->m_pStruSessionGame = pSession;
	}
	//根据先后手发送不同信息
	if(TRUE == oDataRq.m_bIsFirst)
	{
		STRU_NOTICE_BEGIN_RQ oDataRs;
		oDataRs.m_i64UserID = oDataRq.m_i64UserID;
		oDataRs.m_iGameID = oDataRq.m_iGameID;
		DEF_RESPONSE()
	}
	return TRUE;
}
BOOL CKernel::OnNoticeBrginRs(STRU_SESSION* pSession, long lLen, const char* pData)
{
	STRU_NOTICE_BEGIN_RS oDataRq; 
	if (0 == oDataRq.UnSerialize((char*)pData, lLen)) 
	{ 
		return FALSE; 
	} 
	if(1 == oDataRq.m_wResult)
	{
		STRU_NOTICE_BEGIN_RQ oDataRs;
		oDataRs.m_iTime = oDataRq.m_iTime;
		oDataRs.m_iGameID = oDataRq.m_iGameID;
		char szBuf[DEF_MAX_CACHE]; 
		long lBufLen = 0; 
		lBufLen = oDataRs.Serialize(szBuf, DEF_MAX_CACHE); 
		SEND()
	}
	return TRUE;
}
BOOL CKernel::OnSendGameDataRq(STRU_SESSION* pSession, long lLen, const char* pData)
{
	STRU_SEND_GAME_DATA_RQ oDataRq; 
	if (0 == oDataRq.UnSerialize((char*)pData, lLen)) 
	{ 
		return FALSE; 
	} 

	STRU_RECV_GAME_DATA_RQ oDataRs;
	oDataRs.m_sGameData = oDataRq.m_sGameData;
		
	char szBuf[DEF_MAX_CACHE]; 
	long lBufLen = 0; 
	lBufLen = oDataRs.Serialize(szBuf, DEF_MAX_CACHE); 
	SEND()
	
	return TRUE;
}
BOOL CKernel::OnRecvGameDataRs(STRU_SESSION* pSession, long lLen, const char* pData)
{

	return TRUE;
}
BOOL CKernel::OnNoticeEndRq(STRU_SESSION* pSession, long lLen, const char* pData)
{

	return TRUE;
}
BOOL CKernel::OnPlayStrangerRq(STRU_SESSION* pSession, long lLen, const char* pData)
{
	//解析数据
	DEF_UNSERIALIZE_VERIFY(PLAY_STRANGER);
	//保存等待游戏
	GamePlayerStruct* oGamePlayer = new GamePlayerStruct;
	oGamePlayer->m_i64UserID = oDataRq.m_i64UserID;
	oGamePlayer->m_iGameID = oDataRq.m_iGameID;
	oGamePlayer->m_pStruSession = pSession;
	//判断等待游戏集合中是否能够游戏
	if(NULL == m_mapGamePlayer[oDataRq.m_iGameID])
	{
		//加入维护表中
		m_mapGamePlayer[oDataRq.m_iGameID] = oGamePlayer;
		//回复数据
		STRU_PLAY_STRANGER_RS oDataRs;
		oDataRs.m_wResult = enum_re_justwait;
		DEF_RESPONSE()
		/*char szBuf[DEF_MAX_CACHE]; 
		long lBufLen = 0; 
		lBufLen = oDataRs.Serialize(szBuf, DEF_MAX_CACHE); 
		m_pUdpNet->SendData(pSession, szBuf, lBufLen); */
	}
	else
	{
		AttackGamePlayerStruct* oAttackGamePlayer = new AttackGamePlayerStruct;
		//保存游戏ID
		oAttackGamePlayer->m_iGameID = oDataRq.m_iGameID;
		//玩家
		oAttackGamePlayer->m_pGamePlayer1 = oGamePlayer;
		oAttackGamePlayer->m_pGamePlayer2 = m_mapGamePlayer[oDataRq.m_iGameID];
		//维护表中删除之前的等待
		m_mapGamePlayer.erase(m_mapGamePlayer.find(oDataRq.m_iGameID));
		//加入表  会不会存在多线程并发问题？？？
		m_mapPlayers[m_iIndex] = oAttackGamePlayer;
		
		//加入维护表中
		m_mapAttackGamePlayer[oDataRq.m_iGameID] = &m_mapPlayers;

		//回复数据给玩家1
		STRU_PLAY_STRANGER_RS oDataRs;
		oDataRs.m_i64StrangerID = oAttackGamePlayer->m_pGamePlayer1->m_i64UserID;
		oDataRs.m_i64UserID = oAttackGamePlayer->m_pGamePlayer2->m_i64UserID;
		oDataRs.m_iGameID = oAttackGamePlayer->m_iGameID;
		oDataRs.m_iIndex = m_iIndex;
		oDataRs.m_bIsFirst = FALSE;
		oDataRs.m_wResult = enum_re_success;
		char szBuf[DEF_MAX_CACHE]; 
		long lBufLen = 0; 
		lBufLen = oDataRs.Serialize(szBuf, DEF_MAX_CACHE); 
		m_pUdpNet->SendData(oAttackGamePlayer->m_pGamePlayer2->m_pStruSession, szBuf, lBufLen); 

		//睡一会
		Sleep(2000);
		//回复数据给玩家2
		oDataRs.m_i64StrangerID = oAttackGamePlayer->m_pGamePlayer2->m_i64UserID;
		oDataRs.m_i64UserID = oAttackGamePlayer->m_pGamePlayer1->m_i64UserID;
		oDataRs.m_iGameID = oAttackGamePlayer->m_iGameID;
		oDataRs.m_iIndex = m_iIndex;
		oDataRs.m_bIsFirst = TRUE;
		oDataRs.m_wResult = enum_re_success;
		
		lBufLen = oDataRs.Serialize(szBuf, DEF_MAX_CACHE); 
		m_pUdpNet->SendData(oAttackGamePlayer->m_pGamePlayer1->m_pStruSession, szBuf, lBufLen); 
		m_iIndex++;
	}
	return TRUE;
}
BOOL CKernel::OnDownLoadGameRq(STRU_SESSION* pSession, long lLen, const char* pData)
{
	//解析数据
	DEF_UNSERIALIZE_VERIFY(DOWNLOAD_GAME)
	//从记录表中拿到游戏在电脑上的路径

	//开始发送给客户端

	//打开文件(r),并获取文件大小
	DWORD dwSize = 0;
	FILE* fp = NULL;
	//TCHAR strPath[1024] = {0};
	//TCHAR strPath[1024] = {0};
	
	//MultiByteToWideChar(CP_ACP, 0, m_vecGamePath[oDataRq.m_iGameID]->m_szPath, -1, (LPWSTR)strPath, m_vecGamePath[oDataRq.m_iGameID]->m_wAppPathLen);
	
	fopen_s(&fp, m_vecGamePath[oDataRq.m_iGameID]->m_szPath, "rb");
	
	int nNum = fseek(fp, 0, SEEK_END);
	int nError = GetLastError();
	dwSize = ftell(fp);
	fclose(fp);
	fp = NULL;
	fp = fopen(m_vecGamePath[oDataRq.m_iGameID]->m_szPath, "rb");
	//HANDLE hFile = ::CreateFile(strPath, GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL);

	//第一块文件附带文件头信息
	STRU_FILE_HEAD_DOWNLOAD oFileInfo;
	oFileInfo.m_i64UserID = oDataRq.m_i64UserID;
	strcpy(oFileInfo.m_szAppName,m_vecGamePath[oDataRq.m_iGameID]->m_szAppName);
	oFileInfo.m_wAppNameLen = strlen(oFileInfo.m_szAppName) + 1;
	oFileInfo.m_dwFileLength = dwSize;
	oFileInfo.m_wVersion = MAKEWORD(2,3);
	char* pSendBuf = new char[2*1024*1024];
	long lFileHeadLen = oFileInfo.Serialize(pSendBuf, 2*1024*1024);

	//第一次读取病发送带有文件头的信息
	DWORD dwReadSize = fread(&pSendBuf[lFileHeadLen], 1, 2*1024*1024 - lFileHeadLen, fp);
	long lNum = m_pTcpNet->SendData(m_mapTcpSession[oDataRq.m_i64UserID], pSendBuf, dwReadSize + lFileHeadLen); 
	nError = GetLastError();
	//::send(sock, pSendBuf, dwReadSize + lFileHeadLen, 0);
	//循环发送文件信息（读->发）
	while (dwReadSize = fread(pSendBuf, 1, 2*1024*1024, fp))
	{
		m_pTcpNet->SendData(m_mapTcpSession[oDataRq.m_i64UserID], pSendBuf, dwReadSize); 
		//::send(sock, pSendBuf, dwReadSize, 0);
	}
	delete[] pSendBuf;
	fclose(fp);

	//接收完成通知继续发送下一个文件或者断开连接
	/*STRU_FILE_TRANS_COMPLETE oComplete;
	char szRecvBuf[100];
	long lRecvLen;
	lRecvLen = ::recv(sock, szRecvBuf, 100, 0);
	oComplete.UnSerialize(szRecvBuf, lRecvLen);*/

	//继续发送图标

	//如果没有发送的了，关闭连接
	//::closesocket(sock);
	return TRUE;
}