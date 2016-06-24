#pragma once 
#ifndef __INCLUDE_KERNEL_H__
#define __INCLUDE_KERNEL_H__
#include "FactoryNet.h"
#include "MyProtocol.h"
#include "INet.h"
#include "MyIni.h"
//Kernel To UI 回调接口
class IKernelToUI
{
public:
	virtual void NoticeUIBegin() = 0;
	virtual int NoticeUISetTimer() = 0;
	virtual void NoticeUIiMove(int nWays,int nWhich) = 0;
	virtual void NoticeUIEnd() = 0;
	virtual void NoticeUIFailedNet() = 0;
};

class CKernel : public INetToKernel
{
public:
	CKernel();
	~CKernel();
public:
	void InitKernel(IKernelToUI* pToUI);
	void UnInitKernel();
public:
	//发送数据接口，供UI使用
	void SendToServerEnd(BOOL IsWin);
	void SendToServerGameData(int nCountMove,int nSendData);
public:
	//收数据回调，kernel处理INetToKernel
	void OnRecvData(const char* szRecvBuf, long lRecvLen);
public:
	//定义处理函数指针类型，统一不同协议的调用方式
	typedef BOOL (CKernel::*DEAL_FUN)(const char* szRecvBuf,long lRecvLen);
	//定义函数指针数组
	DEAL_FUN m_pDealFun[DEF_PROTOCOL_COUNT];
	//定义处理函数
	BOOL OnHeartBeatRQ(const char* szRecvBuf,long lRecvLen);
	BOOL OnNoticeBeginRQ(const char* szRecvBuf,long lRecvLen);
	BOOL OnNoticeEndRS(const char* szRecvBuf,long lRecvLen);
	BOOL OnSendGameDataRS(const char* szRecvBuf,long lRecvLen);
	BOOL OnRecvGameDataRQ(const char* szRecvBuf,long lRecvLen);
public:
	//读配置文件
	void ReadIniFile();
public:
	IKernelToUI* m_pUI;
	INet* m_pTcp;
	INet* m_pUdp;
	CMyini* m_pIni;
public:
	//游戏信息
	INT64 m_i64UserID;    //配置文件
	INT64 m_i64VerifyCode;//配置文件
	INT64 m_i64OtherID;    //服务器通知
	INT64 m_i64OtherCode;//服务器通知
	int m_iGameID;
	int m_iIndex;
	WORD m_wResult;       //0成功
	BOOL m_bIsFirst;      //接收
	int m_iTime;          //服务器通知是否自己设置
	BOOL m_bIsWin;        //游戏结束通知服务器
	STRU_GAMEDATA m_sGameData;  //自己知道

};


#endif//__INCLUDE_KERNEL_H__