#pragma once

#ifndef __INCLUDE_TCP_NET_H__
#define __INCLUDE_TCP_NET_H__
#include <map>
#include "INet.h"
#include "Lock.h"

#pragma warning(disable:4244)

using namespace std;

class CTcpNet : public INet
{
public:
	CTcpNet();
	~CTcpNet();

	//实现基类接口提供外部调用
	BOOL InitNet(IKernel* pKernel);
	void UnInitNet();
	long SendData(STRU_SESSION* pSession, const char* pData, long lDataLen);
	//通知网络断开连接
	BOOL BreadConn(STRU_SESSION* pSession)
	{
		//查找会话
		std::map<SOCKET, STRU_SESSION*>::iterator it = m_mapSession.find(pSession->m_sock);
		if (it == m_mapSession.end())
		{
			return FALSE;
		}

		//关闭连接
		::closesocket(pSession->m_sock);

		//删除会话
		delete pSession;

		//从表中删除会话
		m_mapSession.erase(it);
		return TRUE;
	}
private:
	//处理线程函数（接受连接和收数据）
	static unsigned int WINAPI AcceptThread(void* pParam);
	void AcceptThread();
	static unsigned int WINAPI RecvThread(void* pParam);
	void RecvThread();

	//初始化基本的网络(内部初始化网络)
	BOOL InnerInitNet();

	//获取本地主机有效地址
	long GetHostIP();

	//删除指定的连接信息
	BOOL RemoveSession(SOCKET sock);

private:
	SOCKET m_sockListen;
	long m_lThreadCount;
	BOOL m_bRun;
	std::map<SOCKET, STRU_SESSION*> m_mapSession;
	IKernel* m_pKernel;
	CLock m_oLock;
};

#endif //__INCLUDE_TCP_NET_H__