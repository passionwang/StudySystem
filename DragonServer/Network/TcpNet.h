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

	//ʵ�ֻ���ӿ��ṩ�ⲿ����
	BOOL InitNet(IKernel* pKernel);
	void UnInitNet();
	long SendData(STRU_SESSION* pSession, const char* pData, long lDataLen);
	//֪ͨ����Ͽ�����
	BOOL BreadConn(STRU_SESSION* pSession)
	{
		//���һỰ
		std::map<SOCKET, STRU_SESSION*>::iterator it = m_mapSession.find(pSession->m_sock);
		if (it == m_mapSession.end())
		{
			return FALSE;
		}

		//�ر�����
		::closesocket(pSession->m_sock);

		//ɾ���Ự
		delete pSession;

		//�ӱ���ɾ���Ự
		m_mapSession.erase(it);
		return TRUE;
	}
private:
	//�����̺߳������������Ӻ������ݣ�
	static unsigned int WINAPI AcceptThread(void* pParam);
	void AcceptThread();
	static unsigned int WINAPI RecvThread(void* pParam);
	void RecvThread();

	//��ʼ������������(�ڲ���ʼ������)
	BOOL InnerInitNet();

	//��ȡ����������Ч��ַ
	long GetHostIP();

	//ɾ��ָ����������Ϣ
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