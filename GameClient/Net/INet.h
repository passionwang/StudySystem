#pragma once 

#ifndef __INCLUDE_INET_H__
#define __INCLUDE_INET_H__

//IP地址定义
//inet_addr("192.168.1.11");GetHostIP()
#define Server_IP_TCP inet_addr("192.168.1.11")/*GetHostIP()*/
#define Server_IP_UDP inet_addr("192.168.1.11")/*GetHostIP()*/
#define Server_TCP_PORT htons(12345)
#define Server_UDP_PORT htons(54321)
//收数据线程的类型（TCP、UDP）
enum ENUM_RECV_TYPE
{
	enum_recv_tcp, 
	enum_recv_udp
};
class INetToKernel
{
public:
	virtual void OnRecvData(const char* szRecvBuf, long lRecvLen,ENUM_RECV_TYPE eType) = 0;
};

class INet
{
public:
	virtual BOOL InitNet(INetToKernel* pKernel) = 0;

	virtual long SendData(const char* pData, long lDataLen) = 0;

	virtual BOOL UnInitNet() = 0;
};


#endif//__INCLUDE_INET_H__