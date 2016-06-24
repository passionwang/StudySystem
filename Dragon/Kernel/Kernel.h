#pragma once 
#ifndef __INCLUDE_KERNEL_H__
#define __INCLUDE_KERNEL_H__
#include "FactoryNet.h"
#include "MyProtocol.h"
#include "INet.h"
#include "MyIni.h"
//Kernel To UI �ص��ӿ�
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
	//�������ݽӿڣ���UIʹ��
	void SendToServerEnd(BOOL IsWin);
	void SendToServerGameData(int nCountMove,int nSendData);
public:
	//�����ݻص���kernel����INetToKernel
	void OnRecvData(const char* szRecvBuf, long lRecvLen);
public:
	//���崦����ָ�����ͣ�ͳһ��ͬЭ��ĵ��÷�ʽ
	typedef BOOL (CKernel::*DEAL_FUN)(const char* szRecvBuf,long lRecvLen);
	//���庯��ָ������
	DEAL_FUN m_pDealFun[DEF_PROTOCOL_COUNT];
	//���崦����
	BOOL OnHeartBeatRQ(const char* szRecvBuf,long lRecvLen);
	BOOL OnNoticeBeginRQ(const char* szRecvBuf,long lRecvLen);
	BOOL OnNoticeEndRS(const char* szRecvBuf,long lRecvLen);
	BOOL OnSendGameDataRS(const char* szRecvBuf,long lRecvLen);
	BOOL OnRecvGameDataRQ(const char* szRecvBuf,long lRecvLen);
public:
	//�������ļ�
	void ReadIniFile();
public:
	IKernelToUI* m_pUI;
	INet* m_pTcp;
	INet* m_pUdp;
	CMyini* m_pIni;
public:
	//��Ϸ��Ϣ
	INT64 m_i64UserID;    //�����ļ�
	INT64 m_i64VerifyCode;//�����ļ�
	INT64 m_i64OtherID;    //������֪ͨ
	INT64 m_i64OtherCode;//������֪ͨ
	int m_iGameID;
	int m_iIndex;
	WORD m_wResult;       //0�ɹ�
	BOOL m_bIsFirst;      //����
	int m_iTime;          //������֪ͨ�Ƿ��Լ�����
	BOOL m_bIsWin;        //��Ϸ����֪ͨ������
	STRU_GAMEDATA m_sGameData;  //�Լ�֪��

};


#endif//__INCLUDE_KERNEL_H__