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
	//�����͹㲥
	virtual long SendData(STRU_SESSION* pSession, const char* pData, long lDataLen);

	//�鲥

		//֪ͨ����Ͽ�����
	BOOL BreadConn(STRU_SESSION* pSession)
	{
		//���һỰ
		std::map<INT64, STRU_SESSION*>::iterator it = m_mapSession.find(pSession->m_sock);
		if (it == m_mapSession.end())
		{
			return FALSE;
		}

		//ɾ���Ự
		delete pSession;

		//�ӱ���ɾ���Ự
		m_mapSession.erase(it);

		return TRUE;
	}
private:
	//��ʼ������������(�ڲ���ʼ������)
	BOOL InnerInitNet();

	//��ȡ����������Ч��ַ
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