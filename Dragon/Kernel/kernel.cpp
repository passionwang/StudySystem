#include "stdafx.h"
#include "Kernel.h"

//数据解析宏和身份验证宏
#define DEF_UNSERIALIZE_VERIFY(TYPE) \
	/*解析数据：*/ \
	STRU_##TYPE##_RQ oDataRq; \
	if (0 == oDataRq.UnSerialize((char*)szRecvBuf, lRecvLen)) \
	{ \
		return FALSE; \
	} 

//应答所有请求的通用宏
#define DEF_RESPONSE() \
	char szBuf[1024]; \
	long lBufLen = 0; \
	lBufLen = oDataRs.Serialize(szBuf, 1024); \
	m_pUdp->SendData(szBuf, 1024); 

CKernel::CKernel() : m_pUI(NULL),m_pTcp(NULL),m_pUdp(NULL),m_pIni(NULL)
{
	m_i64UserID = 0;
	m_i64VerifyCode = 0;
	m_i64OtherID = 0;    //服务器通知
	m_i64OtherCode = 0;//服务器通知
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
//读配置文件
void CKernel::ReadIniFile()
{
	int nValue;//存配置信息
	TCHAR szRecvBuf[MAX_PATH] = {0};
	m_pIni = new CMyini;//拿到配置文件操作指针
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
	//读配置文件
	ReadIniFile();
	CFactoryNet oFactoryNet;//简单工厂拿到TCP
	m_pTcp = oFactoryNet.CreateNet(enum_net_tcp);
	m_pUdp = oFactoryNet.CreateNet(enum_net_udp);
	m_pUI = pToUI;//保存窗口回调指针
	if(FALSE == m_pUdp->InitNet(this))//初始化TCP
	{
		m_pUI->NoticeUIFailedNet();
	}
	STRU_READY_PLAY_RQ oReady_RQ;//发送准备信息
	oReady_RQ.m_i64UserID = m_i64UserID;//结构体准备
	oReady_RQ.m_i64VerifyCode = m_i64VerifyCode;
	oReady_RQ.m_iGameID = m_iGameID;
	oReady_RQ.m_bIsFirst = m_bIsFirst;
	oReady_RQ.m_iIndex = m_iIndex;
	char szbuf[1024];//发送缓冲
	long nLen = 0;
	nLen = oReady_RQ.Serialize(szbuf,1024);//序列化
	m_pUdp->SendData(szbuf,nLen);//发送
}
void CKernel::UnInitKernel()
{
	
}
//发送数据接口，供UI使用
void CKernel::SendToServerEnd(BOOL IsWin)
{
	STRU_NOTICE_END_RQ oDataRs;
	oDataRs.m_i64UserID = m_i64UserID;
	oDataRs.m_i64VerifyCode = m_i64VerifyCode;
	oDataRs.m_iGameID = m_iGameID;
	oDataRs.m_bIsWin = IsWin;
	//序列化
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
//收数据回调，kernel处理INetToKernel
void CKernel::OnRecvData(const char* szRecvBuf, long lRecvLen)
{
	/*if(-1 == lRecvLen)
	{
		m_pUI->NoticeUIFailedNet();
		return;
	}*/
	//获取协议类型
	WORD wType = *(WORD*)szRecvBuf;
	//计算位置
	wType -= DEF_PROTOCOL_BASE;
	//统一调用
	if(wType < DEF_PROTOCOL_COUNT)
	{
		(this->*this->m_pDealFun[wType])(szRecvBuf,lRecvLen);
	}
}
//定义处理函数
BOOL CKernel::OnHeartBeatRQ(const char* szRecvBuf,long lRecvLen)
{
	STRU_HEARTBEAT_RS oDataRs;
	DEF_UNSERIALIZE_VERIFY(HEARTBEAT)//解析数据

	if(oDataRq.m_i64UserID == m_i64UserID
		&& oDataRq.m_i64VerifyCode == m_i64VerifyCode
		&& oDataRq.m_iGameID == m_iGameID)
	{
		oDataRs.m_i64UserID = m_i64UserID;
		oDataRs.m_i64VerifyCode = m_i64VerifyCode;
		oDataRs.m_iGameID = m_iGameID;

		oDataRs.m_wResult = 0;//通知收到信息
	}
	else
	{
		oDataRs.m_i64UserID = m_i64UserID;
		oDataRs.m_i64VerifyCode = m_i64VerifyCode;
		oDataRs.m_iGameID = m_iGameID;
		oDataRs.m_wResult = 1;//通知收到信息,信息不对请修改
	}
	DEF_RESPONSE()//发送数据
	return TRUE;
}
BOOL CKernel::OnNoticeBeginRQ(const char* szRecvBuf,long lRecvLen)
{
	STRU_NOTICE_BEGIN_RS oDataRs;
	DEF_UNSERIALIZE_VERIFY(NOTICE_BEGIN)//解析数据

	if(oDataRq.m_iGameID == m_iGameID)
	{
		//记录对方数据
		//m_i64OtherID = oDataRq.m_i64UserID;
		//m_i64OtherCode = oDataRq.m_i64VerifyCode;
		//m_bIsFirst = oDataRq.m_bIsFirst;
		
		if(FALSE == m_bIsFirst)//不是先手，不需要设置游戏时间，可以直接回复
		{
			m_iTime = oDataRq.m_iTime;//记录时间

			//回复数据
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
	DEF_RESPONSE()//发送数据
	return TRUE;
}
BOOL CKernel::OnNoticeEndRS(const char* szRecvBuf,long lRecvLen)
{
	//oNoticeEnd_RS.UnSerialize(szRecvBuf,lRecvLen);
	//if(oNoticeEnd_RS.m_i64UserID == m_i64UserID
	//	&& oNoticeEnd_RS.m_i64VerifyCode == m_i64VerifyCode
	//	&& oNoticeEnd_RS.m_iGameID == m_iGameID)
	//{
	//	if(0 != oNoticeEnd_RS.m_wResult)//错误或没收到，重新发送
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
	//	if(0 != oSendGameData_RS.m_wResult)//错误或没收到，重新发送
	//	{

	//	}
	//}
	return TRUE;
}
BOOL CKernel::OnRecvGameDataRQ(const char* szRecvBuf,long lRecvLen)
{
	STRU_RECV_GAME_DATA_RS oDataRs;
	DEF_UNSERIALIZE_VERIFY(RECV_GAME_DATA)//解析数据
	
	m_sGameData = oDataRq.m_sGameData;//记录发过来的游戏信息

	if(m_sGameData.m_iWayCounts>0
		&& m_sGameData.m_iWhichWay != -1)
	{
		m_pUI->NoticeUIiMove(m_sGameData.m_iWayCounts,m_sGameData.m_iWhichWay);
		oDataRs.m_i64UserID = m_i64UserID;
		oDataRs.m_i64VerifyCode = m_i64VerifyCode;
		oDataRs.m_iGameID = m_iGameID;
		oDataRs.m_wResult = 0;
		
		DEF_RESPONSE()//发送数据
	}
	return TRUE;
}