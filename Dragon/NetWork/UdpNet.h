#pragma once

#ifndef __INCLUDE_UDP_NET_H__
#define __INCLUDE_UDP_NET_H__
#include<iostream>
#include<Winsock2.h>
#include "MyProtocol.h"
#include "INet.h"
using namespace std;

#pragma comment(lib,"Ws2_32.lib")
class CUdpNet : public INet
{
public:
	CUdpNet();
	~CUdpNet();

public:
	//ʵ�ֻ��෽��
	virtual BOOL InitNet(INetToKernel* pKernel);
	virtual long SendData(const char* pData, long lDataLen);
	virtual BOOL UnInitNet();
private:
	BOOL InitUdpNet();
	long GetHostIP();
	void CloseUdpNet();
	void RecvData(int& nNum);
	static DWORD WINAPI ThreadRecvProc(LPVOID lpParameter);
	void ThreadRecvProc();
public:
	INetToKernel* m_pKernel;
	//�����߳�����״̬
	BOOL m_bRun;
	//tcp�׽���
	SOCKET m_socket_client;
};

#endif //__INCLUDE_UDP_NET_H__