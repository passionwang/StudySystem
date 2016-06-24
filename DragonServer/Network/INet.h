#pragma once

#ifndef __INCLUDE_NET_H__
#define __INCLUDE_NET_H__
//#include <Windows.h>
#include <WinSock2.h>
#include "MyProtocol.h"
#pragma comment(lib, "ws2_32.lib")

//�������ݵĻ�����
#define DEF_MAX_CACHE (1200)

//IP��ַ����
#define LOCAL_IP_TCP GetHostIP()
#define LOCAL_IP_UDP GetHostIP()
#define LOCAL_TCP_PORT htons(12345)
#define LOCAL_UDP_PORT htons(54321)

//Ϊ�˽��ʹ���������֪��Ӧ�ò��˺ŵ��ǲ���ʶSOCKET��������ֻ��ʶSOCKET������ʶӦ�ò�仯���˺ţ�
//���Խ�Ӧ�ò���ʶ���˺ź��������ʶ��SOCKET��װ��Ϊһ��������Ϣ�ṹ�����������Ӧ�ò�ֻ��Ҫ�����Լ�
//��ʶ�Ķ�������
//�ֶ�m_dwAccountΪӦ�ò��û��Լ���������Ҫ��ֻҪ�ܱ�֤Ψһ�ԾͿ���
struct STRU_SESSION
{
	STRU_SESSION()
	{
		m_sock = NULL;
		m_dwAccount = 0;
	}
public:
	INT64 m_sock;
	DWORD m_dwAccount;
};
enum ENUM_DOWN_UP_TYPE
{
	DOWN_FILE,
	UP_FILE
};
struct STRU_TCP_SESSION : public STRU_SESSION
{
	STRU_TCP_SESSION()
	{
		m_pFileHeadDown = NULL;
		m_pFileHeadUp = NULL;
		m_i64FileKey = 0;
	}
public:
	ENUM_DOWN_UP_TYPE eType;
	STRU_FILE_HEAD_DOWNLOAD* m_pFileHeadDown;
	STRU_FILE_HEAD_UPLOAD* m_pFileHeadUp;
	INT64 m_i64FileKey;
};

//�������̵߳����ͣ�TCP��UDP��
enum ENUM_RECV_TYPE
{
	enum_recv_tcp, 
	enum_recv_udp
};

//���ݻص��ӿڣ�֪ͨ��Ҫ���ݵ��ˣ�
class IKernel
{
public:
	virtual BOOL OnRecvData(STRU_SESSION* pSession, const char* pData, long lDataLen, int iType) = 0;
};

class INet
{
public:
	virtual BOOL InitNet(IKernel* pKernel) = 0;
	virtual void UnInitNet() = 0;

	//write
	virtual long SendData(STRU_SESSION* pSession, const char* pData, long lDataLen) = 0;

	//֪ͨ����Ͽ�����
	virtual BOOL BreadConn(STRU_SESSION* pSession) = 0;
};

#endif //__INCLUDE_NET_H__