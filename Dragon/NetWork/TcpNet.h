#pragma once

#ifndef __INCLUDE_TCP_NET_H__
#define __INCLUDE_TCP_NET_H__
#include<iostream>
#include<Winsock2.h>
#include "MyProtocol.h"
#include "INet.h"
using namespace std;

#pragma comment(lib,"Ws2_32.lib")



class CTcpNet : public INet
{
public:
	CTcpNet();
	~CTcpNet();
public:
	//实现基类方法
	virtual BOOL InitNet(INetToKernel* pKernel);
	virtual BOOL UnInitNet();
	virtual long SendData(const char* pData, long lDataLen);
private:
	long GetHostIP();
	void CloseTcpNet();
	void RecvData(int& nNum);
	/*static DWORD WINAPI ThreadConnectProc(LPVOID lpParameter);
	void ThreadConnectProc();*/
	static DWORD WINAPI ThreadRecvProc(LPVOID lpParameter);
	void ThreadRecvProc();
	BOOL ConnectNet();
private:
	INetToKernel* m_pKernel;
	//接收线程运行状态
	BOOL m_bRun;
	BOOL m_bConnect;
	//tcp套接字
	SOCKET m_socket_client;

};

#endif //__INCLUDE_TCP_NET_H__