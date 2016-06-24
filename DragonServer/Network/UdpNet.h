#pragma once

#ifndef __INCLUDE_UDP_NET_H__
#define __INCLUDE_UDP_NET_H__
#include <map>
#include "INet.h"

using namespace std;

class CUdpNet : public INet
{
public:
	CUdpNet();
	~CUdpNet();

public:
	virtual BOOL InitNet(IKernel* pKernel);
	virtual void UnInitNet();
	//单播和广播
	virtual long SendData(STRU_SESSION* pSession, const char* pData, long lDataLen);

	//组播

		//通知网络断开连接
	BOOL BreadConn(STRU_SESSION* pSession)
	{
		//查找会话
		std::map<INT64, STRU_SESSION*>::iterator it = m_mapSession.find(pSession->m_sock);
		if (it == m_mapSession.end())
		{
			return FALSE;
		}

		//删除会话
		delete pSession;

		//从表中删除会话
		m_mapSession.erase(it);

		return TRUE;
	}
private:
	//初始化基本的网络(内部初始化网络)
	BOOL InnerInitNet();

	//获取本地主机有效地址
	long GetHostIP();

	static unsigned int WINAPI RecvThread(void* pParam);
	void RecvThread();
private:
	SOCKET m_sock;
	long m_lThreadCount;
	BOOL m_bRun;
	std::map<INT64, STRU_SESSION*> m_mapSession;
	IKernel* m_pKernel;
};

#endif //__INCLUDE_UDP_NET_H__