#include "Kernel.h"
#include <process.h>
#include <atlstr.h>
//���ݿ�����
#define DEF_DATABASE_NAME ("AppPlat")

//���ݵ�ַ
#define DEF_DB_IP		("127.0.0.1,1433")

//�û���Ϣ��
#define DEF_USER_TABLE	("USER_INFO")
#define DEF_USER_ID		("user_id")
#define DEF_USER_PASS	("user_pass")
#define DEF_USER_LAST_TIME	("last_time")
#define DEF_USER_STATE	("user_state")

//ÿ���ļ������̵߳�������еĳ���
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
//ʵ�ֻ���ģ�巽��ģʽ��̬��̨�Ľӿ�
//�ṩ������ȥʵ����Դ��ʼ������Դ���ٵķ���
BOOL CKernel::Init()
{
	//��ֹ��ε��ã�������
	UnInit();

	//����һ�����繤������
	CFactoryeNet oFactoryNet;

	//��ʼ���̳߳ض���
	m_oPool.CreatePool(5, 10, 10000);

	//��ȡϵͳ��Ϣ
	SYSTEM_INFO sysInfo;
	::GetSystemInfo(&sysInfo);
	m_lThreadCount = sysInfo.dwNumberOfProcessors * 2;

	//�����ļ������̵߳��������
	m_pFileTransQueue = new CCircleQueue<CFileTransTask>[m_lThreadCount];
	for (long i = 0; i < m_lThreadCount; ++i)
	{
		m_pFileTransQueue[i].InitQueue(DEF_THREAD_QUEUE_LEN);
	}

	//�����̲߳���
	m_pThreadParam = new STRU_FILE_TRANS_PARAM[m_lThreadCount];

	//�����ļ������߳�
	m_bStop = FALSE;
	for (long i = 0; i < m_lThreadCount; ++i)
	{
		m_pThreadParam[i].m_lIndex = i;
		m_pThreadParam[i].m_pThis = this;
		HANDLE hThread = (HANDLE)::_beginthreadex(NULL, 0, OnFileTrans, &m_pThreadParam[i], 0, NULL);
		::CloseHandle(hThread);
	}

	//����TCP����
	m_pTcpNet = oFactoryNet.CreateNet(enum_net_tcp);
	m_pTcpNet->InitNet(this);

	//����UDP����
	m_pUdpNet = oFactoryNet.CreateNet(enum_net_udp);
	m_pUdpNet->InitNet(this);

	//�����ݿ�
	if (FALSE == m_oDao.OpenDateBase(DEF_DATABASE_NAME, DATEBASE_TYPE_SQL2005, "sa", "passion", DEF_DB_IP))
	{
		return FALSE;
	}

	////��ȡ�û�������Ϣ���ڴ��й����û���Ϣ��
	ReadUserInfo();

	return TRUE;
}

void CKernel::UnInit()
{
	//TODO:
}

//�ļ������߳�
unsigned int WINAPI CKernel::OnFileTrans(void* pParam)
{
	//ת������
	STRU_FILE_TRANS_PARAM* pThis = (STRU_FILE_TRANS_PARAM*)pParam;

	//������ͨ�̺߳���
	pThis->m_pThis->OnFileTrans(pThis->m_lIndex);

	return 1L;
}
void CKernel::OnFileTrans(long lIndex)
{
	//ѭ�������Լ������е�����
	while (m_bStop == FALSE)
	{
		CFileTransTask* pTask = NULL;
		while (m_pFileTransQueue[lIndex].PopQueue(pTask) == TRUE)
		{
			//��������(д�ļ�)
			//�����Ѿ��򿪵��ļ�����
			IFile* pFile = NULL;
			FILE_OBJECT_MAP::iterator it = m_oFileMap.find(pTask->m_i64FileKey);
			if (it == m_oFileMap.end())
			{
				//�ж϶�Ӧ���ļ��Ƿ���ڣ������ڴ���
				IsExistFile(pTask->m_i64UserID, pTask->m_szFileName, pTask->m_wFileNameLen);
				//�����ļ����� �����ļ�
				char szPath[MAX_PATH];
				sprintf_s(szPath, MAX_PATH, "./%I64d/%s", pTask->m_i64UserID, pTask->m_szFileName);
				pFile = new CFileOpr;
				pFile->OpenFile(szPath, enum_opr_write);

				//2.����򿪵��ļ�����ȴ�������һ���ļ�
				m_oFileMap[pTask->m_i64FileKey] = pFile;
			}
			else
			{
				pFile = it->second;
			}

			//д�ļ�
			DWORD dwError = ::GetLastError();
			long lLen = pFile->WriteFile(pTask->m_pFileContent, pTask->m_dwFileLength);
			dwError = ::GetLastError();

			//�鿴��ǰ�ļ�ʵ�ʴ�С�Ƿ�����ļ�����ʵ��С
			DWORD dwSize = pFile->GetFileSize();
			if (dwSize >= pTask->m_dwFileSize)
			{
				//�ر��ļ�����
				pFile->CloseFile();
				delete pFile;

				//�����ļ�ͷ
				if(UP_FILE == pTask->m_pSession->eType)
				{
					delete pTask->m_pSession->m_pFileHeadUp;
					pTask->m_pSession->m_pFileHeadUp = NULL;
					pTask->m_pSession->m_i64FileKey =0;
					//֪ͨ�ͻ��˿��Դ���һ���ļ���
					STRU_FILE_TRANS_COMPLETE_UPLOAD oFileTrans;
					oFileTrans.m_i64FileKey = pTask->m_i64FileKey;
					oFileTrans.m_i64UserID = pTask->m_i64UserID;
					oFileTrans.m_wAppNameLen = pTask->m_wFileNameLen;
					memcpy_s(oFileTrans.m_szAppName, 20,pTask->m_szFileName, oFileTrans.m_wAppNameLen);
					//����
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
					//֪ͨ�ͻ��˿��Դ���һ���ļ���
					STRU_FILE_TRANS_COMPLETE_DOWNLOAD oFileTrans;
					oFileTrans.m_i64FileKey = pTask->m_i64FileKey;
					oFileTrans.m_i64UserID = pTask->m_i64UserID;
					oFileTrans.m_wAppNameLen = pTask->m_wFileNameLen;
					memcpy_s(oFileTrans.m_szAppName,20, pTask->m_szFileName, oFileTrans.m_wAppNameLen);
					//����
					char szBuf[DEF_MAX_BUF];
					long lSendLen = 0;
					lSendLen = oFileTrans.Serialize(szBuf, DEF_MAX_BUF);
					m_pTcpNet->SendData(pTask->m_pSession, szBuf, lSendLen);
				}
				

				
				
				
			}
		}

		//��Ϣһ��
		::Sleep(10);
	}

	//�����߳�����
	::InterlockedDecrement(&m_lThreadCount);
}

//�����ݿ޼����û���Ϣ
BOOL CKernel::ReadUserInfo()
{
	std::list<TCHAR*> columnList;
	columnList.push_back(DEF_USER_ID);
	columnList.push_back(DEF_USER_PASS);
	columnList.push_back(DEF_USER_LAST_TIME);
	columnList.push_back(DEF_USER_STATE);
	std::list<TCHAR*> resList;

	//��ȡ
	if (FALSE == m_oDao.GetData("select * from USER_INFO", columnList, columnList.size(), resList))
	{
		return FALSE;
	}

	//�����û���Ϣ��
	long lCount = resList.size() / columnList.size();
	std::list<TCHAR*>::iterator it = resList.begin();
	for (long i = 0; i < lCount; ++i)
	{
		//����һ���û���Ϣ
		STRU_USER_INFO* pUserInfo = new STRU_USER_INFO;

		//���һ���û���Ϣ
		pUserInfo->m_i64UserID = _atoi64(*it++);
		pUserInfo->m_wPasswordLen = strlen(*it);
		memcpy(pUserInfo->m_szPassword, *it++, pUserInfo->m_wPasswordLen);
		pUserInfo->m_i64LastTime = _atoi64(*it++);
		pUserInfo->m_wState = atoi(*it++);

		//����һ���û���Ϣ
		m_oUserMap[pUserInfo->m_i64UserID] = pUserInfo;
	}

	return TRUE;
}

//��ȡapp�б�
BOOL CKernel::ReadAppList(INT64 i64AppKey, long lCount, STRU_GET_APP_LIST_RS& oDataRs)
{
	//��ȡ
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

	//��ȡ
	if (FALSE == m_oDao.GetData(" \
	select APP_INFO.app_key, APP_INFO.shared_user, APP_INFO.down_times, APP_INFO.up_time,\
	APP_INFO.app_info,APP_INFO.icon_path, APP_INFO.app_path, APP_KEY.app_name \
	from APP_INFO, APP_KEY where APP_INFO.app_key = APP_KEY.app_key", columnList, columnList.size(), resList))
	{
		return FALSE;
	}

	//�������Ӧ��
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
//ʵ�ֽ����������ݵķ���
BOOL CKernel::OnRecvData(STRU_SESSION* pSession, const char* pData, long lDataLen, int iType)
{
	if (iType == enum_recv_udp)
	{
		//��������
		CMyTask* pTask = new CMyTask;

		//�������
		pTask->m_pSession = pSession;
		memcpy_s(pTask->m_szData,1200, pData, lDataLen);
		pTask->m_lDataLen = lDataLen;
		pTask->m_pKernel = this;

		//Ͷ�������̳߳���
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
		//��ȡЭ������
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
				//ת���ỰΪTCP�Ự
				STRU_TCP_SESSION* pTcpSession = (STRU_TCP_SESSION*)pSession;

				//�Ƿ�Ϊ�ļ��ĵ�һ��
				DWORD dwHeadLen = 0;
				INT64 i64FileKey = 0;
				char szSerialize[DEF_MAX_BUF];
				if (pTcpSession->m_pFileHeadUp == NULL)
				{
					//��ȡ�ļ�ͷ
					pTcpSession->m_pFileHeadUp = new STRU_FILE_HEAD_UPLOAD;
					pTcpSession->m_pFileHeadUp->UnSerialize(pData, lDataLen);
					dwHeadLen = pTcpSession->m_pFileHeadUp->Serialize(szSerialize, DEF_MAX_BUF);
					pTcpSession->eType = UP_FILE;
					//�����ļ�key
					pTcpSession->m_i64FileKey = GetFileKey(pTcpSession->m_pFileHeadUp->m_i64UserID, 
					pTcpSession->m_pFileHeadUp->m_wVersion, 
					pTcpSession->m_pFileHeadUp->m_wAppNameLen, 
					pTcpSession->m_pFileHeadUp->m_szAppName);
				}

				//����Ͷ�ݵĶ��б��
				i64FileKey = pTcpSession->m_i64FileKey;
				long lIndex = GetPostQueueNum(i64FileKey);

				//��������
				CFileTransTask* pTask = new CFileTransTask;

				//��ʼ������
				pTask->m_i64UserID = pTcpSession->m_pFileHeadUp->m_i64UserID;
				pTask->m_i64FileKey = i64FileKey;
				pTask->m_pSession = (STRU_TCP_SESSION*)pSession;
				pTask->m_dwFileSize = pTcpSession->m_pFileHeadUp->m_dwFileLength;
				pTask->m_wFileNameLen = pTcpSession->m_pFileHeadUp->m_wAppNameLen;
				memcpy_s(pTask->m_szFileName,20, pTcpSession->m_pFileHeadUp->m_szAppName, pTask->m_wFileNameLen);
				pTask->m_dwFileLength = lDataLen - dwHeadLen;
				pTask->m_pFileContent = new char[pTask->m_dwFileLength];
				memcpy_s(pTask->m_pFileContent,2*1024*1024, &pData[dwHeadLen], pTask->m_dwFileLength);

				//Ͷ��
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
	//����������֤���
	DEF_UNSERIALIZE_VERIFY(CONNECT_MESSAGE);
	m_mapTcpSession[oDataRq.m_i64UserID] = pSession;
	return TRUE;
}
//ʵ��Ϊ���������ݵķ���
BOOL CKernel::DealData(STRU_SESSION* pSession, long lLen, const char* pData)
{
	//��ȡЭ������
	WORD wType = *(WORD*)pData;

	//����λ��
	wType -= DEF_PROTOCOL_BASE;

	//ͳһ����
	(this->*this->m_pDealFun[wType])(pSession, lLen, pData);

	//����switch�����˳���Ŀ���ԭ�����ԣ�ͨ����дMFC��Ϣӳ����ʵ�֣���ȥ��switch��ͳһ�Ժ�Э��ı仯
	//(*m_pFun[wType])();
	//
	////��������������
	//switch (wType)
	//{
	//case DEF_LOGIN_RQ:
	//	{
	//		DealLoginRq();
	//		////�����ӦЭ��ṹ
	//		//STRU_LOGIN_RQ oDataRq;

	//		////�����յ�������
	//		//oDataRq.UnSerialize(pData, lLen);

	//		////����
	//		////���Ҹ��û���Ϣ

	//		////�Ƚ�����

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
//Ϊÿ���������Ӧ�Ĵ�����
BOOL CKernel::OnDealLoginRq(STRU_SESSION* pSession, long lLen, const char* pData)
{
	//�������ݣ�
	STRU_LOGIN_RQ oDataRq;
	STRU_LOGIN_RS oDataRs;
	if (0 == oDataRq.UnSerialize((char*)pData, lLen))
	{
		return FALSE;
	}

	//�����û����ƻ�ȡ�û���Ϣ
	USER_MAP::iterator it = m_oUserMap.find(oDataRq.m_i64UserID);

	oDataRs.m_i64UserID = oDataRq.m_i64UserID;
	//���û�л�ȡ��ֱ��Ӧ���û�������
	if (it == m_oUserMap.end())
	{
		oDataRs.m_wResult = enum_res_unexist;
	}
	else
	{
		//�Ƚ����볤��
		if (it->second->m_wPasswordLen != oDataRq.m_wPasswordLen)
		{
			oDataRs.m_wResult = enum_res_password_error;
		}
		//�Ƚ������Ƿ���ȷ
		else
		{
			if (memcmp(oDataRq.m_szPassword, it->second->m_szPassword, oDataRq.m_wPasswordLen) == 0)
			{
				//�����ȷ��Ӧ���½�ɹ�
				oDataRs.m_wResult = enum_res_success;

				//��ȫ�û��Ự��Ϣ
				pSession->m_dwAccount = oDataRq.m_i64UserID;

				//�����Ժ�������֤�û���ݵ���֤�룬������
				oDataRs.m_i64VerifyCode = GetVerifyCode(oDataRq.m_i64UserID);
				it->second->m_i64VerifyCode = oDataRs.m_i64VerifyCode;
			}
			else
			{
				//�������ȷ��Ӧ���½�������
				oDataRs.m_wResult = enum_res_password_error;
			}
		}
	}

	//���л�����
	DEF_RESPONSE();

	return TRUE;
}
BOOL CKernel::OnDealGetAppListRq(STRU_SESSION* pSession, long lLen, const char* pData)
{
	//����������֤���
	DEF_UNSERIALIZE_VERIFY(GET_APP_LIST);

	//��ȡ���ݿ�
	STRU_GET_APP_LIST_RS oDataRs;
	oDataRs.m_i64UserID = oDataRq.m_i64UserID;
	ReadAppList(oDataRq.m_i64AppKey, DEF_GET_APP_LIST_COUNT, oDataRs);
	//������Ϸ��Ϣ�������Ժ���������Ϸ·��
	GamePathInfo* oGamePath = NULL;
	for(int i=0;i<oDataRs.m_wAppItemCount;i++)
	{
		oGamePath = new GamePathInfo;
		//·����Ϣ
		oGamePath->m_wAppPathLen = oDataRs.m_oAppList[i].m_wAppPathLen;
		oDataRs.m_oAppList[i].m_szAppPath[oDataRs.m_oAppList[i].m_wAppPathLen] = '\0';
		strcpy_s(oGamePath->m_szPath,oGamePath->m_wAppPathLen+1,oDataRs.m_oAppList[i].m_szAppPath);
		//������Ϣ
		oGamePath->m_wAppNameLen = oDataRs.m_oAppList[i].m_wAppNameLen;
		oDataRs.m_oAppList[i].m_szAppName[oDataRs.m_oAppList[i].m_wAppNameLen] = '\0';
		strcpy_s(oGamePath->m_szAppName,oGamePath->m_wAppNameLen+1,oDataRs.m_oAppList[i].m_szAppName);
		m_vecGamePath.push_back(oGamePath);
	}
	//����
	DEF_RESPONSE();
	return TRUE;
}
BOOL CKernel::OnDealInstallRq(STRU_SESSION* pSession, long lLen, const char* pData)
{
	//����������֤���
	DEF_UNSERIALIZE_VERIFY(INSTALL);

	//д�밲װ����Ϣ
	/*char szSql[100];
	sprintf(szSql, "insert into install_info values(%I64d, %I64d, %I64d)", 
		oDataRq.m_i64UserID, ::GetTickCount(), oDataRq.m_i64AppKey);
	m_oDao.EditData(szSql);*/
	return TRUE;
}
BOOL CKernel::OnDealUnInstallRq(STRU_SESSION* pSession, long lLen, const char* pData)
{
	//����������֤���
	DEF_UNSERIALIZE_VERIFY(UNINSTALL);

	////д�밲װ����Ϣ
	//char szSql[100];
	//sprintf(szSql, "insert into uninstall_info values(%I64d, %I64d, %I64d)", 
	//	oDataRq.m_i64UserID, ::GetTickCount(), oDataRq.m_i64AppKey);
	//m_oDao.EditData(szSql);
	return TRUE;
}
BOOL CKernel::OnDealLogOutRq(STRU_SESSION* pSession, long lLen, const char* pData)
{
	//����������֤���
	DEF_UNSERIALIZE_VERIFY(LOGOUT);

	//֪ͨ����ر�����
	m_pUdpNet->BreadConn(pSession);

	//���-�û���Ϣ�е��û���֤��͵�½״̬
	it->second->m_i64LastTime = ::GetTickCount();
	it->second->m_i64VerifyCode = 0;
	it->second->m_wState = 0;

	//�������ݿ����û�����½��ʱ�������״̬
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
	//��ŵ�ǰ�Ự�������Ժ�Ľ���
	if(oDataRq.m_i64UserID == m_mapAttackGamePlayer[oDataRq.m_iGameID]->find(oDataRq.m_iIndex)->second->m_pGamePlayer1->m_i64UserID)
	{
		m_mapAttackGamePlayer[oDataRq.m_iGameID]->find(oDataRq.m_iIndex)->second->m_pGamePlayer1->m_pStruSessionGame = pSession;
	}
	else
	{
		m_mapAttackGamePlayer[oDataRq.m_iGameID]->find(oDataRq.m_iIndex)->second->m_pGamePlayer2->m_pStruSessionGame = pSession;
	}
	//�����Ⱥ��ַ��Ͳ�ͬ��Ϣ
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
	//��������
	DEF_UNSERIALIZE_VERIFY(PLAY_STRANGER);
	//����ȴ���Ϸ
	GamePlayerStruct* oGamePlayer = new GamePlayerStruct;
	oGamePlayer->m_i64UserID = oDataRq.m_i64UserID;
	oGamePlayer->m_iGameID = oDataRq.m_iGameID;
	oGamePlayer->m_pStruSession = pSession;
	//�жϵȴ���Ϸ�������Ƿ��ܹ���Ϸ
	if(NULL == m_mapGamePlayer[oDataRq.m_iGameID])
	{
		//����ά������
		m_mapGamePlayer[oDataRq.m_iGameID] = oGamePlayer;
		//�ظ�����
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
		//������ϷID
		oAttackGamePlayer->m_iGameID = oDataRq.m_iGameID;
		//���
		oAttackGamePlayer->m_pGamePlayer1 = oGamePlayer;
		oAttackGamePlayer->m_pGamePlayer2 = m_mapGamePlayer[oDataRq.m_iGameID];
		//ά������ɾ��֮ǰ�ĵȴ�
		m_mapGamePlayer.erase(m_mapGamePlayer.find(oDataRq.m_iGameID));
		//�����  �᲻����ڶ��̲߳������⣿����
		m_mapPlayers[m_iIndex] = oAttackGamePlayer;
		
		//����ά������
		m_mapAttackGamePlayer[oDataRq.m_iGameID] = &m_mapPlayers;

		//�ظ����ݸ����1
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

		//˯һ��
		Sleep(2000);
		//�ظ����ݸ����2
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
	//��������
	DEF_UNSERIALIZE_VERIFY(DOWNLOAD_GAME)
	//�Ӽ�¼�����õ���Ϸ�ڵ����ϵ�·��

	//��ʼ���͸��ͻ���

	//���ļ�(r),����ȡ�ļ���С
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

	//��һ���ļ������ļ�ͷ��Ϣ
	STRU_FILE_HEAD_DOWNLOAD oFileInfo;
	oFileInfo.m_i64UserID = oDataRq.m_i64UserID;
	strcpy(oFileInfo.m_szAppName,m_vecGamePath[oDataRq.m_iGameID]->m_szAppName);
	oFileInfo.m_wAppNameLen = strlen(oFileInfo.m_szAppName) + 1;
	oFileInfo.m_dwFileLength = dwSize;
	oFileInfo.m_wVersion = MAKEWORD(2,3);
	char* pSendBuf = new char[2*1024*1024];
	long lFileHeadLen = oFileInfo.Serialize(pSendBuf, 2*1024*1024);

	//��һ�ζ�ȡ�����ʹ����ļ�ͷ����Ϣ
	DWORD dwReadSize = fread(&pSendBuf[lFileHeadLen], 1, 2*1024*1024 - lFileHeadLen, fp);
	long lNum = m_pTcpNet->SendData(m_mapTcpSession[oDataRq.m_i64UserID], pSendBuf, dwReadSize + lFileHeadLen); 
	nError = GetLastError();
	//::send(sock, pSendBuf, dwReadSize + lFileHeadLen, 0);
	//ѭ�������ļ���Ϣ����->����
	while (dwReadSize = fread(pSendBuf, 1, 2*1024*1024, fp))
	{
		m_pTcpNet->SendData(m_mapTcpSession[oDataRq.m_i64UserID], pSendBuf, dwReadSize); 
		//::send(sock, pSendBuf, dwReadSize, 0);
	}
	delete[] pSendBuf;
	fclose(fp);

	//�������֪ͨ����������һ���ļ����߶Ͽ�����
	/*STRU_FILE_TRANS_COMPLETE oComplete;
	char szRecvBuf[100];
	long lRecvLen;
	lRecvLen = ::recv(sock, szRecvBuf, 100, 0);
	oComplete.UnSerialize(szRecvBuf, lRecvLen);*/

	//��������ͼ��

	//���û�з��͵��ˣ��ر�����
	//::closesocket(sock);
	return TRUE;
}