#include "stdafx.h"
#include "Kernel.h"

//���ݽ�����������֤��
#define DEF_UNSERIALIZE_VERIFY(TYPE) \
	/*�������ݣ�*/ \
	STRU_##TYPE##_RQ oDataRq; \
	if (0 == oDataRq.UnSerialize((char*)szRecvBuf, lRecvLen)) \
	{ \
		return FALSE; \
	} 

//Ӧ�����������ͨ�ú�
#define DEF_RESPONSE() \
	char szBuf[1024]; \
	long lBufLen = 0; \
	lBufLen = oDataRs.Serialize(szBuf, 1024); \
	m_pUdp->SendData(szBuf, 1024); 

CKernel::CKernel() : m_pUI(NULL),m_pTcp(NULL),m_pUdp(NULL),m_pIni(NULL)
{
	m_i64UserID = 0;
	m_i64VerifyCode = 0;
	m_i64OtherID = 0;    //������֪ͨ
	m_i64OtherCode = 0;//������֪ͨ
	m_iGameID = 0;
	m_wResult = 0;
	m_bIsFirst = TRUE;
	m_iTime = 30;
	m_bIsWin = FALSE;
	
	m_sGameData.m_iWayCounts = 0;
	m_sGameData.m_iWhichWay = -1;

	m_pDealFun[DEF_HEARTBEAT_RQ - DEF_PROTOCOL_BASE]      = &CKernel::OnHeartBeatRQ;
	m_pDealFun[DEF_NOTICE_BEGIN_RQ - DEF_PROTOCOL_BASE]   = &CKernel::OnNoticeBeginRQ;
	m_pDealFun[DEF_NOTICE_END_RS - DEF_PROTOCOL_BASE]     = &CKernel::OnNoticeEndRS;
	m_pDealFun[DEF_SEND_GAME_DATA_RS - DEF_PROTOCOL_BASE] = &CKernel::OnSendGameDataRS;
	m_pDealFun[DEF_RECV_GAME_DATA_RQ - DEF_PROTOCOL_BASE] = &CKernel::OnRecvGameDataRQ;
}
CKernel::~CKernel()
{
}
//�������ļ�
void CKernel::ReadIniFile()
{
	int nValue;//��������Ϣ
	TCHAR szRecvBuf[MAX_PATH] = {0};
	m_pIni = new CMyini;//�õ������ļ�����ָ��
	m_pIni->GetIniFilePath();
	m_pIni->GetIniFileInt(TEXT("Dragon"),TEXT("m_i64UserID"),nValue);
	m_i64UserID = nValue;
	m_pIni->GetIniFileInt(TEXT("Dragon"),TEXT("m_i64OtherID"),nValue);
	m_i64OtherID = nValue;
	m_pIni->GetIniFileInt(TEXT("Dragon"),TEXT("m_bIsFirst"),nValue);
	m_bIsFirst = (nValue == 1) ? TRUE : FALSE;
	m_pIni->GetIniFileInt(TEXT("Dragon"),TEXT("m_iGameID"),nValue);
	m_iGameID = nValue;
	m_pIni->GetIniFileInt(TEXT("Dragon"),TEXT("m_iIndex"),nValue);
	m_iIndex = nValue;
}
void CKernel::InitKernel(IKernelToUI* pToUI)
{
	//�������ļ�
	ReadIniFile();
	CFactoryNet oFactoryNet;//�򵥹����õ�TCP
	m_pTcp = oFactoryNet.CreateNet(enum_net_tcp);
	m_pUdp = oFactoryNet.CreateNet(enum_net_udp);
	m_pUI = pToUI;//���洰�ڻص�ָ��
	if(FALSE == m_pUdp->InitNet(this))//��ʼ��TCP
	{
		m_pUI->NoticeUIFailedNet();
	}
	STRU_READY_PLAY_RQ oReady_RQ;//����׼����Ϣ
	oReady_RQ.m_i64UserID = m_i64UserID;//�ṹ��׼��
	oReady_RQ.m_i64VerifyCode = m_i64VerifyCode;
	oReady_RQ.m_iGameID = m_iGameID;
	oReady_RQ.m_bIsFirst = m_bIsFirst;
	oReady_RQ.m_iIndex = m_iIndex;
	char szbuf[1024];//���ͻ���
	long nLen = 0;
	nLen = oReady_RQ.Serialize(szbuf,1024);//���л�
	m_pUdp->SendData(szbuf,nLen);//����
}
void CKernel::UnInitKernel()
{
	
}
//�������ݽӿڣ���UIʹ��
void CKernel::SendToServerEnd(BOOL IsWin)
{
	STRU_NOTICE_END_RQ oDataRs;
	oDataRs.m_i64UserID = m_i64UserID;
	oDataRs.m_i64VerifyCode = m_i64VerifyCode;
	oDataRs.m_iGameID = m_iGameID;
	oDataRs.m_bIsWin = IsWin;
	//���л�
	DEF_RESPONSE() 
}
void CKernel::SendToServerGameData(int nCountMove,int nSendData)
{
	STRU_SEND_GAME_DATA_RQ oDataRs;
	oDataRs.m_i64UserID = m_i64UserID;
	oDataRs.m_i64VerifyCode = m_i64VerifyCode;
	oDataRs.m_iGameID = m_iGameID;
	oDataRs.m_iIndex = m_iIndex;
	oDataRs.m_sGameData.m_iWayCounts = nCountMove;
	oDataRs.m_sGameData.m_iWhichWay = nSendData;

	DEF_RESPONSE() 
}
//�����ݻص���kernel����INetToKernel
void CKernel::OnRecvData(const char* szRecvBuf, long lRecvLen)
{
	/*if(-1 == lRecvLen)
	{
		m_pUI->NoticeUIFailedNet();
		return;
	}*/
	//��ȡЭ������
	WORD wType = *(WORD*)szRecvBuf;
	//����λ��
	wType -= DEF_PROTOCOL_BASE;
	//ͳһ����
	if(wType < DEF_PROTOCOL_COUNT)
	{
		(this->*this->m_pDealFun[wType])(szRecvBuf,lRecvLen);
	}
}
//���崦����
BOOL CKernel::OnHeartBeatRQ(const char* szRecvBuf,long lRecvLen)
{
	STRU_HEARTBEAT_RS oDataRs;
	DEF_UNSERIALIZE_VERIFY(HEARTBEAT)//��������

	if(oDataRq.m_i64UserID == m_i64UserID
		&& oDataRq.m_i64VerifyCode == m_i64VerifyCode
		&& oDataRq.m_iGameID == m_iGameID)
	{
		oDataRs.m_i64UserID = m_i64UserID;
		oDataRs.m_i64VerifyCode = m_i64VerifyCode;
		oDataRs.m_iGameID = m_iGameID;

		oDataRs.m_wResult = 0;//֪ͨ�յ���Ϣ
	}
	else
	{
		oDataRs.m_i64UserID = m_i64UserID;
		oDataRs.m_i64VerifyCode = m_i64VerifyCode;
		oDataRs.m_iGameID = m_iGameID;
		oDataRs.m_wResult = 1;//֪ͨ�յ���Ϣ,��Ϣ�������޸�
	}
	DEF_RESPONSE()//��������
	return TRUE;
}
BOOL CKernel::OnNoticeBeginRQ(const char* szRecvBuf,long lRecvLen)
{
	STRU_NOTICE_BEGIN_RS oDataRs;
	DEF_UNSERIALIZE_VERIFY(NOTICE_BEGIN)//��������

	if(oDataRq.m_iGameID == m_iGameID)
	{
		//��¼�Է�����
		//m_i64OtherID = oDataRq.m_i64UserID;
		//m_i64OtherCode = oDataRq.m_i64VerifyCode;
		//m_bIsFirst = oDataRq.m_bIsFirst;
		
		if(FALSE == m_bIsFirst)//�������֣�����Ҫ������Ϸʱ�䣬����ֱ�ӻظ�
		{
			m_iTime = oDataRq.m_iTime;//��¼ʱ��

			//�ظ�����
			oDataRs.m_i64UserID = m_i64UserID;
			oDataRs.m_i64VerifyCode = m_i64VerifyCode;
			oDataRs.m_iGameID = m_iGameID;
			oDataRs.m_iTime = m_iTime;
		}
		else
		{
			oDataRs.m_i64UserID = m_i64UserID;
			oDataRs.m_i64VerifyCode = m_i64VerifyCode;
			oDataRs.m_iGameID = m_iGameID;
			oDataRs.m_iTime = m_pUI->NoticeUISetTimer();
			m_iTime = oDataRs.m_iTime;
			oDataRs.m_iIndex = m_iIndex;
			oDataRs.m_wResult = 1;
		}
		m_pUI->NoticeUIBegin();
	}
	DEF_RESPONSE()//��������
	return TRUE;
}
BOOL CKernel::OnNoticeEndRS(const char* szRecvBuf,long lRecvLen)
{
	//oNoticeEnd_RS.UnSerialize(szRecvBuf,lRecvLen);
	//if(oNoticeEnd_RS.m_i64UserID == m_i64UserID
	//	&& oNoticeEnd_RS.m_i64VerifyCode == m_i64VerifyCode
	//	&& oNoticeEnd_RS.m_iGameID == m_iGameID)
	//{
	//	if(0 != oNoticeEnd_RS.m_wResult)//�����û�յ������·���
	//	{

	//	}
	//}
	return TRUE;
}
BOOL CKernel::OnSendGameDataRS(const char* szRecvBuf,long lRecvLen)
{
	//oSendGameData_RS.UnSerialize(szRecvBuf,lRecvLen);
	//if(oSendGameData_RS.m_i64UserID == m_i64UserID
	//	&& oSendGameData_RS.m_i64VerifyCode == m_i64VerifyCode
	//	&& oSendGameData_RS.m_iGameID == m_iGameID)
	//{
	//	if(0 != oSendGameData_RS.m_wResult)//�����û�յ������·���
	//	{

	//	}
	//}
	return TRUE;
}
BOOL CKernel::OnRecvGameDataRQ(const char* szRecvBuf,long lRecvLen)
{
	STRU_RECV_GAME_DATA_RS oDataRs;
	DEF_UNSERIALIZE_VERIFY(RECV_GAME_DATA)//��������
	
	m_sGameData = oDataRq.m_sGameData;//��¼����������Ϸ��Ϣ

	if(m_sGameData.m_iWayCounts>0
		&& m_sGameData.m_iWhichWay != -1)
	{
		m_pUI->NoticeUIiMove(m_sGameData.m_iWayCounts,m_sGameData.m_iWhichWay);
		oDataRs.m_i64UserID = m_i64UserID;
		oDataRs.m_i64VerifyCode = m_i64VerifyCode;
		oDataRs.m_iGameID = m_iGameID;
		oDataRs.m_wResult = 0;
		
		DEF_RESPONSE()//��������
	}
	return TRUE;
}