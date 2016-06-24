#pragma once

#ifndef __INCLUDE_NET_H__
#define __INCLUDE_NET_H__
//#include <Windows.h>
#include <WinSock2.h>
#include "MyProtocol.h"
#pragma comment(lib, "ws2_32.lib")

//接收数据的缓冲区
#define DEF_MAX_CACHE (1200)

//IP地址定义
#define LOCAL_IP_TCP GetHostIP()
#define LOCAL_IP_UDP GetHostIP()
#define LOCAL_TCP_PORT htons(12345)
#define LOCAL_UDP_PORT htons(54321)

//为了解决使用网络的人知道应用层账号但是不认识SOCKET，而网络只认识SOCKET而不认识应用层变化的账号，
//所以将应用层认识的账号和网络层认识的SOCKET包装成为一个连接信息结构，对于网络和应用层只需要关心自己
//认识的东西即可
//字段m_dwAccount为应用层用户自己填充根据需要，只要能保证唯一性就可以
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

//收数据线程的类型（TCP、UDP）
enum ENUM_RECV_TYPE
{
	enum_recv_tcp, 
	enum_recv_udp
};

//数据回调接口（通知需要数据的人）
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

	//通知网络断开连接
	virtual BOOL BreadConn(STRU_SESSION* pSession) = 0;
};

#endif //__INCLUDE_NET_H__