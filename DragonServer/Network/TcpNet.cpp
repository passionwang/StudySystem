//Tcp����IP   �˿�12345
#include <process.h>
#include "TcpNet.h"
#pragma warning(disable:4554 4244)

CTcpNet::CTcpNet() : m_sockListen(NULL), m_lThreadCount(0), m_bRun(FALSE)
{
}

CTcpNet::~CTcpNet()
{
}

//////////////////////////////////////////////////////////////////////////////
//ʵ�ֻ���ӿ��ṩ�ⲿ����
BOOL CTcpNet::InitNet(IKernel* pKernel)
{
	//��ʼ������������(�ڲ���ʼ������)
	InnerInitNet();

	//����ص�����
	m_pKernel = pKernel;
	m_bRun = TRUE;

	//����һ���������ӵ��߳�
	HANDLE hThread = (HANDLE)::_beginthreadex(NULL, 0, AcceptThread, this, 0, NULL);
	if (hThread == NULL)
	{
		return FALSE;
	}
	::CloseHandle(hThread);
	//����һ�������ݵ��߳�
	hThread = (HANDLE)::_beginthreadex(NULL, 0, RecvThread, this, 0, NULL);
	if (hThread == NULL)
	{
		return FALSE;
	}
	::CloseHandle(hThread);

	return TRUE;
}

void CTcpNet::UnInitNet()
{
	//֪ͨ�߳��˳�
	m_bRun = FALSE;

	//�ȴ������߳��˳�
	while (m_lThreadCount > 0)
	{
		::Sleep(1);
	}

	//�ر�����ж�ؿ�
	if (m_sockListen != NULL)
	{
		::closesocket(m_sockListen);
		m_sockListen = NULL;

		//�رտͻ���
		std::map<SOCKET, STRU_SESSION*>::iterator it = m_mapSession.begin();
		while (it != m_mapSession.end())
		{
			delete (STRU_SESSION*)(it->second);
			++it;
		}
		m_mapSession.clear();

		//�رտ�
		::WSACleanup();
	}
}

long CTcpNet::SendData(STRU_SESSION* pSession, const char* pData, long lDataLen)
{
	return ::send(pSession->m_sock, pData, lDataLen, 0);
}

//////////////////////////////////////////////////////////////////////////////
//�����̺߳������������Ӻ������ݣ�
unsigned int WINAPI CTcpNet::AcceptThread(void* pParam)
{
	//ת������
	CTcpNet* pThis = (CTcpNet*)pParam;

	//������ͨ�̺߳���
	pThis->AcceptThread();
	
	return 1L;
}

void CTcpNet::AcceptThread()
{
	//�����߳�����
	::InterlockedIncrement(&(m_lThreadCount));

	//���ü����׽���Ϊ������
	u_long flag = 100;
	::ioctlsocket(m_sockListen, FIONBIO, &flag);

	//ѭ����������
	SOCKET client;
	int iErrorCode = 0;
	while (m_bRun)
	{
		client = ::accept(m_sockListen, NULL, NULL);
		if (SOCKET_ERROR == client)
		{
			//��ȡ������
			iErrorCode = ::WSAGetLastError();
			if (iErrorCode == WSAEWOULDBLOCK)
			{
				::Sleep(1);
				continue;
			}
			else
			{
				break;
			}
		}

		//�ɹ�����һ������
		//(1)�����ͻ���������Ϣ�ṹ
		STRU_SESSION* pSession = new STRU_TCP_SESSION;

		//(2)���ṹ
		pSession->m_sock = client;

		//(3)����������Ϣ
		m_oLock.Lock();
		(m_mapSession)[client] = pSession;
		m_oLock.UnLock();
	}

	//�����߳�����
	::InterlockedDecrement(&(m_lThreadCount));
}

unsigned int WINAPI CTcpNet::RecvThread(void* pParam)
{
	//ת������
	CTcpNet* pThis = (CTcpNet*)pParam;

	//������ͨ�̺߳���
	pThis->RecvThread();

	return 1L;
}

void CTcpNet::RecvThread()
{
	//�����߳�����
	::InterlockedIncrement(&(m_lThreadCount));

	//ѭ��������
	char *szRecvBuf = new char[1024*1024];
	int iRet = -1;
	timeval val;
	val.tv_sec = 0;
	val.tv_usec = 10;

	while (m_bRun)
	{
		fd_set readSet;

		//���
		FD_ZERO(&readSet);
		//���ͻ��˼���
		std::map<SOCKET, STRU_SESSION*>::iterator it = m_mapSession.begin();
		while (it != m_mapSession.end())
		{
			FD_SET((SOCKET)(it->first), &readSet);
			++it;
		}

		//����select
		iRet = ::select(0, &readSet, NULL, NULL, &val);
		if (iRet == 0)
		{
			continue;
		}
		else if (iRet == SOCKET_ERROR)
		{
			continue;
		}

		//��������(���ԭʼ�Ŀͻ��˼���˭���ڷ��ص�readSet�У�˭����Ҫ������)
		it = m_mapSession.begin();
		while (it != m_mapSession.end())
		{
			if (FD_ISSET(it->first, &readSet) > 0)
			{
				//recv
				iRet = ::recv(it->first, szRecvBuf, 1024*1024, 0);
				if (iRet == 0 || iRet == SOCKET_ERROR)
				{
					//�����Ч������
					RemoveSession(it->first);
				}
				else
				{
					//֪ͨ�ϲ�ĵ������յ�����
					if (m_pKernel)
					{
						m_pKernel->OnRecvData(it->second, szRecvBuf, iRet, enum_recv_tcp);
					}
				}
			}
			it++;
		}
	}

	//�����߳�����
	::InterlockedDecrement(&(m_lThreadCount));
}

//��ʼ������������(�ڲ���ʼ������)
BOOL CTcpNet::InnerInitNet()
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
	m_sockListen = ::socket(AF_INET, SOCK_STREAM, 0);

	//��ʼ����ַ
	SOCKADDR_IN addr;
	addr.sin_family = AF_INET;
	addr.sin_port = LOCAL_TCP_PORT;
	addr.sin_addr.S_un.S_addr = LOCAL_IP_TCP;

	//��
	::bind(m_sockListen, (sockaddr*)&addr, sizeof(addr));

	//����
	::listen(m_sockListen, 5);

	return TRUE;
}

//��ȡ����������Ч��ַ
long CTcpNet::GetHostIP()
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

//ɾ��ָ����������Ϣ
BOOL CTcpNet::RemoveSession(SOCKET sock)
{
	CAutoLock autoLock(m_oLock);
	//m_oLock.Lock();
	std::map<SOCKET, STRU_SESSION*>::iterator it = m_mapSession.find(sock);
	if (it == m_mapSession.end())
	{
		//m_oLock.UnLock();
		return FALSE;
	}
	else
	{
		::closesocket(it->first);
		delete (STRU_SESSION*)(it->second);
		m_mapSession.erase(it);
	}
	//m_oLock.UnLock();
	return TRUE;
}