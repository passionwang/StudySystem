//UDP����IP �˿�8888
#include <process.h>
#include "UdpNet.h"

#pragma warning(disable:4554 4244)
CUdpNet::CUdpNet() : m_sock(NULL), m_lThreadCount(0), m_bRun(FALSE), m_pKernel(NULL)
{

}
CUdpNet::~CUdpNet()
{

}

/////////////////////////////////////////////////////////////////////////////////
BOOL CUdpNet::InitNet(IKernel* pKernel)
{
	UnInitNet();
	InnerInitNet();
	m_pKernel = pKernel;
	m_bRun = TRUE;
	//����һ�������ݵ��߳�
	HANDLE hThread = (HANDLE)::_beginthreadex(NULL, 0, RecvThread, this, 0, NULL);
	if (hThread == NULL)
	{
		return FALSE;
	}
	::CloseHandle(hThread);
	return TRUE;
}

void CUdpNet::UnInitNet()
{
	//֪ͨ�߳��˳�
	m_bRun = FALSE;

	//�ȴ������߳��˳�
	while (m_lThreadCount > 0)
	{
		::Sleep(1);
	}

	//�ر�����ж�ؿ�
	if (m_sock != NULL)
	{
		::closesocket(m_sock);
		m_sock = NULL;

		//����������Ϣ
		std::map<INT64, STRU_SESSION*>::iterator it = m_mapSession.begin();
		while (it != m_mapSession.end())
		{
			delete (STRU_SESSION*)it->second;
			++it;
		}
		m_mapSession.clear();

		//�رտ�
		::WSACleanup();
	}
}

//�����͹㲥
long CUdpNet::SendData(STRU_SESSION* pSession, const char* pData, long lDataLen)
{
	//��ʼ����ַ
	SOCKADDR_IN addr;
	addr.sin_family = AF_INET;
	addr.sin_port = (WORD)(pSession->m_sock);
	addr.sin_addr.S_un.S_addr = (DWORD)(pSession->m_sock >> 32);

	return ::sendto(m_sock, pData, lDataLen, 0, (sockaddr*)&addr, sizeof(addr));
}

/////////////////////////////////////////////////////////////////////////////////
//��ʼ������������(�ڲ���ʼ������)
BOOL CUdpNet::InnerInitNet()
{
	//�򿪿�
	WORD wVersion = MAKEWORD(2, 2);
	WSADATA wsaData;
	::WSAStartup(wVersion, &wsaData);

	//У���
	if (2 != LOBYTE(wsaData.wVersion) || 2 != HIBYTE(wsaData.wVersion))
	{
		return FALSE;
	}

	//��������SOCKET
	m_sock = ::socket(AF_INET, SOCK_DGRAM, 0);

	//��ʼ����ַ
	SOCKADDR_IN addr;
	addr.sin_family = AF_INET;
	addr.sin_port = LOCAL_UDP_PORT;
	addr.sin_addr.S_un.S_addr = LOCAL_IP_UDP;

	//��
	::bind(m_sock, (sockaddr*)&addr, sizeof(addr));

	//����SOCKET֧�ֹ㲥
	BOOL flag = TRUE;
	::setsockopt(m_sock, SOL_SOCKET, SO_BROADCAST, (char*)&flag, sizeof(flag));

	return TRUE;
}

//��ȡ����������Ч��ַ
long CUdpNet::GetHostIP()
{
		long lValidIP = inet_addr("127.1.1.1");

	//��ȡ��������
	char szHostName[100] = "";
	::gethostname(szHostName, sizeof(szHostName) - 1);

	//��ȡ��ַ�б�
	struct hostent* pHostAddr = ::gethostbyname(szHostName);
	
	//������ЧIP
	if (pHostAddr != NULL)
	{
		if (pHostAddr->h_addr_list[0] != NULL && pHostAddr->h_length == 4)
		{
			lValidIP = *(long*)pHostAddr->h_addr_list[0];
		}
	}

	return lValidIP;
}

unsigned int WINAPI CUdpNet::RecvThread(void* pParam)
{
	//ת������
	CUdpNet* pThis = (CUdpNet*)pParam;

	//������ͨ�̺߳���
	pThis->RecvThread();

	return 1L;
}

void CUdpNet::RecvThread()
{
	//�����߳�����
	::InterlockedIncrement(&(m_lThreadCount));

	//ѭ��������
	char szRecvBuf[DEF_MAX_CACHE];
	int iRet = -1;
	timeval val;
	val.tv_sec = 0;
	val.tv_usec = 10;

	while (m_bRun)
	{
		fd_set readSet;

		//���
		FD_ZERO(&readSet);

		//��������SOCKET
		FD_SET(m_sock, &readSet);

		//����select
		iRet = ::select(0, &readSet, NULL, NULL, &val);
		if (iRet == 0)
		{
			continue;
		}
		else if (iRet == SOCKET_ERROR)
		{
			break;
		}

		//��������(���ԭʼ�Ŀͻ��˼���˭���ڷ��ص�readSet�У�˭����Ҫ������)
		SOCKADDR_IN clientAddr;
		int lClientAddrLen = sizeof(clientAddr);

		if (FD_ISSET(m_sock, &readSet) > 0)
		{
			iRet = ::recvfrom(m_sock, szRecvBuf, sizeof(szRecvBuf), 0, (sockaddr*)&clientAddr,
				&lClientAddrLen);
			if (iRet > 0)
			{
				//�ϳɶԶ���Ϣ
				INT64 i64Key = (((INT64)clientAddr.sin_addr.S_un.S_addr) << 32) + clientAddr.sin_port;
				//����������Ϣ
				std::map<INT64, STRU_SESSION*>::iterator it = m_mapSession.find(i64Key);
				STRU_SESSION* pSession = NULL;
				if (it != m_mapSession.end())
				{
					pSession = it->second;
				}
				else
				{
					pSession = new STRU_SESSION;
					pSession->m_sock = i64Key;
					m_mapSession[i64Key] = pSession;
				}

				//֪ͨ�ϲ�ĵ������յ�����
				if (m_pKernel)
				{
					m_pKernel->OnRecvData(pSession, szRecvBuf, iRet, enum_recv_udp);
				}
			}
		}
	}

	//�����߳�����
	::InterlockedDecrement(&(m_lThreadCount));
}